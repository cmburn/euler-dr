/* SPDX-License-Identifier: ISC */

#include "euler/app/native/state.h"

#include <iostream>

#include <physfs.h>
#include <SDL3/SDL_timer.h>

#define OPTPARSE_API [[maybe_unused]] static
#define OPTPARSE_IMPLEMENTATION
#include "euler/app/native/logger.h"
#include "euler/app/native/optparse.h"
#include "euler/app/native/storage.h"
#include "euler/app/native/window.h"
#include "euler/event/event.h"
#include "euler/graphics/ext.h"
#include "euler/gui/ext.h"
#include "euler/physics/ext.h"
#include "euler/util/image.h"
#include "euler/util/version.h"

using euler::app::native::State;

static auto state_count = std::binary_semaphore(1);
static std::thread::id main_thread_id;

State::~State() = default;

/* ReSharper disable once CppDFAConstantFunctionResult */
static constexpr int
sdl_init_flags()
{
	int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS
	    | SDL_INIT_GAMEPAD;
#ifdef EULER_USE_JOYSTICK
	flags |= SDL_INIT_JOYSTICK;
#endif
#ifdef EULER_USE_HAPTIC
	flags |= SDL_INIT_HAPTIC;
#endif
#ifdef EULER_USE_SENSOR
	flags |= SDL_INIT_SENSOR;
#endif
#ifdef EULER_USE_CAMERA
	flags |= SDL_INIT_CAMERA;
#endif
	return flags;
}

// static mrb_value
// state_log(mrb_state *mrb, const mrb_value self)
// {
// 	// auto state = State::get(mrb);
// 	// state->mrb()->iv_get(self, EULER_IVSYM(log));
// 	return State::get(mrb)->mrb()->iv_get(self, EULER_IVSYM(log));
// }

#define ATTR_IV_READER(SYM)                                                    \
	[](mrb_state *mrb, const mrb_value self) {                             \
		return State::get(mrb)->mrb()->iv_get(self, EULER_IVSYM(SYM)); \
	}

static constexpr auto state_log = [](mrb_state *mrb, const mrb_value self) {
	const auto value = State::get(mrb)->mrb()->iv_get(self, EULER_IVSYM(log));
	assert(!mrb_nil_p(value));
	return value;
};
static constexpr auto state_user_storage = ATTR_IV_READER(user_storage);
static constexpr auto state_title_storage = ATTR_IV_READER(title_storage);
static constexpr auto state_window = ATTR_IV_READER(window);

[[noreturn]] static void
usage(std::string_view progname, const bool is_error = true)
{
	auto &out = is_error ? std::cerr : std::cout;
	out << std::format(R"EOF(
Euler Game Engine {}
usage: {} [options] <file>
Options:
	-I, --include <path>    Add a directory to the load path. '../lib',
				relative to <file>, is added by default if it
				exists. Note that the load path must also
				include assets that are to be loaded by any
				game scripts.
	-V, --version           Print version information and exit
	-c, --config <file>     Load configuration from file
	-h, --help              Show this help message and exit
	-j, --jobs <n>          Set number of threads/jobs used (default: {})
	-l, --log-level <level> Set the log level. (default: info)
				May be one of:
				- debug
				- info
				- warn
				- error
				- fatal
	-q, --quiet             Decrease log level by one
	-v, --verbose           Increase log level by one
Notes:
	<file> should be the entry point of the game. It is expected to create
	an object that inherits from `Euler::Game::State` and assign it to
	`$state`.
)EOF",
	    euler::util::version().to_string(), progname,
	    State::DEFAULT_THREAD_COUNT)
	    << std::endl;
	exit(is_error ? EXIT_FAILURE : EXIT_SUCCESS);
}

static constexpr std::array OPTIONS = {
	(struct optparse_long) {
	    .longname = "include",
	    .shortname = 'I',
	    .argtype = OPTPARSE_REQUIRED,
	},
	(struct optparse_long) {
	    .longname = "version",
	    .shortname = 'V',
	    .argtype = OPTPARSE_NONE,
	},
	(struct optparse_long) {
	    .longname = "config",
	    .shortname = 'c',
	    .argtype = OPTPARSE_REQUIRED,
	},
	(struct optparse_long) {
	    .longname = "help",
	    .shortname = 'h',
	    .argtype = OPTPARSE_NONE,
	},
	(struct optparse_long) {
	    .longname = "jobs",
	    .shortname = 'j',
	    .argtype = OPTPARSE_REQUIRED,
	},
	(struct optparse_long) {
	    .longname = "log-level",
	    .shortname = 'l',
	    .argtype = OPTPARSE_REQUIRED,
	},
	(struct optparse_long) {
	    .longname = "quiet",
	    .shortname = 'q',
	    .argtype = OPTPARSE_NONE,
	},
	(struct optparse_long) {
	    .longname = "verbose",
	    .shortname = 'v',
	    .argtype = OPTPARSE_NONE,
	},
	/* optparse needs a zeroed out sentinel option last */
	(struct optparse_long) {
	    .longname = nullptr,
	    .shortname = 0,
	    .argtype = OPTPARSE_NONE,
	},
};

static inline bool
next_opt(struct optparse &options, int &option)
{
	const auto next = optparse_long(&options, OPTIONS.data(), nullptr);
	if (next == -1) return false;
	option = next;
	return true;
}

static void
increment_severity(State::Arguments &args, const int inc)
{
	if (args.log_level == State::Severity::Unknown) return;
	int new_level = static_cast<int>(args.log_level) + inc;
	if (new_level < static_cast<int>(State::Severity::Debug))
		new_level = static_cast<int>(State::Severity::Debug);
	else if (new_level > static_cast<int>(State::Severity::Fatal))
		new_level = static_cast<int>(State::Severity::Fatal);
	args.log_level = static_cast<State::Severity>(new_level);
}

State::Arguments
State::Arguments::parse(int argc, char **argv)
{
	Arguments out = {
		.progname = std::filesystem::path(argv[0]).filename().string(),
		.include = {},
		.config = std::nullopt,
		.thread_count = DEFAULT_THREAD_COUNT,
		.log_level = Severity::Info,
		.file = "",
	};
	struct optparse options;
	optparse_init(&options, argv);
	int option;
	while (next_opt(options, option)) {
		switch (option) {
		case 'I': out.include.emplace_back(options.optarg); break;
		case 'V':
			std::cout << "Euler Game Engine "
				  << util::version().to_string() << std::endl;
			exit(EXIT_SUCCESS);
		case 'c': out.config = options.optarg; break;
		case 'h': usage(out.progname, false);
		case 'j': //
			try {
				out.thread_count = std::stoul(options.optarg);
			} catch (const std::exception &e) {
				std::cerr << "Invalid thread count: "
					  << options.optarg << std::endl
					  << "Error: " << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
			break;
		case 'l':
			out.log_level
			    = util::Logger::parse_severity(options.optarg);
			break;
		case 'q': increment_severity(out, -1); break;
		case 'v': increment_severity(out, 1); break;
		default: usage(out.progname, true);
		}
	}
	if (options.optind >= argc) {
		std::cerr << "No input file specified" << std::endl;
		exit(EXIT_FAILURE);
	}
	out.file = options.argv[options.optind];
	return out;
}

State::State(const Arguments &args)
{
	_mrb = util::make_reference<RubyState>();
	_progname = args.progname;
	_include = args.include;
	_thread_count = args.thread_count;
	_log_level = args.log_level;
	_file = args.file;
}

bool
State::initialize()
{
	_log = euler::util::make_reference<Logger>("euler",
	    Logger::stderr_sink(), Logger::stdout_sink());
	//_log->set_severity(_log_level);
	_log->set_severity(Severity::Debug);
	const auto weak = util::WeakReference(this);
	_mrb->mrb()->ud = util::unsafe_cast<void *>(weak);
	_log->info("Initializing state with file '{}'", _file.string());
	static std::once_flag once;
	std::call_once(once, [&]() {
		log()->debug("Initializing global state...");
		main_thread_id = std::this_thread::get_id();
		log()->debug("Initializing global storage...");
		// init_fs(_config.progname.c_str());
		PHYSFS_init(progname().c_str());
		log()->debug("Initializing global SDL...");
		SDL_Init(sdl_init_flags());
		log()->debug("Global initialization complete");
	});
	log()->debug("Creating window");
	_window = util::make_reference<Window>(util::Reference(this));
	log()->debug("Initializing Vulkan");
	_renderer = util::make_reference<vulkan::Renderer>(log());
	_renderer->initialize(_window);

	log()->debug("Initializing core modules");
	/* TODO: proper physfs limiting for module load */
	auto self = util::Reference(this);
	auto &mods = modules();
	assert(mods.mod != nullptr);
	mods.event.mod = event::init(self, mods.mod);
	mods.graphics.mod = graphics::init(self, mods.mod);
	mods.gui.mod = gui::init(self, mods.mod);
#ifdef EULER_MATH
	mods.math.mod = math::init(self, mods.mod);
#endif
#ifdef EULER_PHYSICS
	mods.physics.mod = physics::init(self, mods.mod);
#endif
	mods.util.mod = util::init(self, mods.mod);
	_log->debug("Core modules initialized");
	set_ivs();
	auto cls = mods.app.state;
	mrb()->define_method(cls, "log", state_log, MRB_ARGS_NONE());
	mrb()->define_method(cls, "user_storage", state_user_storage,
	    MRB_ARGS_NONE());
	mrb()->define_method(cls, "title_storage", state_title_storage,
	    MRB_ARGS_NONE());
	mrb()->define_method(cls, "window", state_window, MRB_ARGS_NONE());

	if (!std::filesystem::exists(_file)) {
		log()->error("File '{}' does not exist", _file.string());
		return false;
	}
	log()->debug("Loading entry file '{}'", _file.string());
	std::ifstream file(_file);
	auto filename = _file.string();
	if (!file.is_open()) {
		log()->error("Failed to open file '{}'", filename);
		return false;
	}
	std::string code((std::istreambuf_iterator(file)),
	    std::istreambuf_iterator<char>());
	if (!load_text(filename, code)) {
		log()->error("Failed to load entry file '{}'", filename);
		return false;
	}
	auto gv = gv_state();
	set_ivs();
	if (mrb_nil_p(gv)) {
		log()->error("File '{}' did not set $state", filename);
		return false;
	}
	log()->info("$app = {}", mrb()->string_cstr(mrb()->inspect(gv)));
	if (!check_global_state()) return false;
	/* ReSharper disable once CppDFAConstantConditions */
	/* ReSharper disable once CppDFAUnreachableCode */

	if (!app_load()) return false;
	return true;
}

euler::util::Reference<euler::util::Logger>
State::log() const
{
	return _log;
}

euler::util::Reference<euler::util::Storage>
State::user_storage() const
{
	return _user_storage;
}

euler::util::Reference<euler::util::Storage>
State::title_storage() const
{
	return _title_storage;
}

euler::util::Reference<euler::util::Image>
State::load_image(const char *)
{
	return nullptr;
}

void
State::upload_image(const char *, const util::Reference<util::Image> &)
{
}

State::tick_t
State::ticks() const
{
	return SDL_GetTicks();
}

const std::string &
State::title() const
{
	return _title;
}
bool
State::loop(int &exit_code)
{
	try {
		if (update(exit_code)) return true;
		set_phase(Phase::Quit);
		if (_methods.quit) app_quit();
		return false;
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in loop: {}", e.what());
	} catch (...) {
		_log->error("Unknown exception in loop");
	}
	exit_code = EXIT_FAILURE;
	return false;
}

float
State::dt() const
{
	return (ticks() - last_tick()) / 1000.0f;
}

bool
State::update(int &exit_code)
{
	set_phase(Phase::Input);
	const auto gc_idx = mrb_gc_arena_save(mrb()->mrb());
	tick();
	auto fn = [&](const SDL_Event &ev) {
		if (_methods.input) return app_input(ev);
		return ev.type != SDL_EVENT_QUIT;
	};
	if (SDL_Event e; !_window->poll_event(e, fn)) return false;
	set_phase(Phase::Update);
	if (_methods.update && !app_update(dt())) return false;
	mrb_gc_arena_restore(mrb()->mrb(), gc_idx);
	set_phase(Phase::Draw);
	return _window->draw(exit_code, [&](int &retval) {
		if (!_methods.draw || app_draw()) return true;
		retval = EXIT_FAILURE;
		return false;
	});
}

bool
State::app_update(float dt)
{
	assert(_methods.update);
	const auto arg = mrb()->float_value(dt);
	auto mrb = this->mrb()->mrb();
	this->mrb()->funcall_argv(gv_state(), EULER_SYM(update), 1, &arg);
	return true;
}

bool
State::app_input(const SDL_Event &event)
{
	const auto type = static_cast<SDL_EventType>(event.type);
	const auto is_quit = type == SDL_EVENT_QUIT;
	if (!_methods.input) return is_quit;
	auto mrb = this->mrb()->mrb();
	const auto arg = event::sdl_event_to_mrb(util::Reference(this), event);
	assert(!mrb_nil_p(arg));
	this->mrb()->funcall_argv(gv_state(), EULER_SYM(input), 1, &arg);
	return !is_quit;
}

bool
State::app_draw()
{
	if (!_methods.draw) return true;
	try {
		const auto mrb = this->mrb()->mrb();
		this->mrb()->funcall_argv(gv_state(), EULER_SYM(draw));
		tick();
		return true;
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in draw: {}", e.what());
		return false;
	} catch (...) {
		_log->error("Unknown exception in draw");
		return false;
	}
}

bool
State::app_load()
{
	tick();
	if (!_methods.load) return true;
	const auto mrb = this->mrb()->mrb();
	auto gvs = gv_state();
	auto inspect = this->mrb()->inspect(gvs);
	log()->info("loading with class {}", this->mrb()->string_cstr(inspect));
	this->mrb()->funcall_argv(gvs, EULER_SYM(load));
	tick();
	return true;
}

bool
State::app_quit()
{
	if (!_methods.quit) return true;
	const auto mrb = this->mrb()->mrb();
	this->mrb()->funcall_argv(gv_state(), EULER_SYM(quit));
	return true;
}

void
State::set_ivs()
{
	auto mrb = this->mrb()->mrb();
#define REGISTER_IV(SYM, TYPE, RCLASS, EXPR)                                   \
	do {                                                                   \
		if (!mrb_nil_p(_attributes.SYM)) {                             \
			mrb_gc_unregister(this->mrb()->mrb(),                  \
			    _attributes.SYM);                                  \
		}                                                              \
		const auto data                                                \
		    = this->mrb()->data_object_alloc(RCLASS, (EXPR), &TYPE);   \
		_attributes.SYM = mrb_obj_value(data);                         \
		mrb_gc_register(this->mrb()->mrb(), _attributes.SYM);          \
		this->mrb()->iv_set(this->self_value(), EULER_IVSYM(SYM),      \
		    _attributes.SYM);                                          \
	} while (0)
	REGISTER_IV(log, util::Logger::TYPE, modules().util.logger,
	    log().wrap());
	REGISTER_IV(user_storage, util::Storage::TYPE, modules().util.storage,
	    user_storage().wrap());
	REGISTER_IV(title_storage, util::Storage::TYPE, modules().util.storage,
	    title_storage().wrap());
	// REGISTER_IV(window, Window::TYPE, modules().app.window,
	// window().wrap());
#undef REGISTER_IV
}

bool
State::check_global_state()
{
	const auto var = gv_state();
	if (mrb_nil_p(var)) {
		log()->error("Global variable for state is not defined");
		return false;
	}
	if (!mrb()->obj_is_kind_of(var, modules().app.state)) {
		log()->error("Global variable for state does not inherit from "
			     "Euler::Util::State");
		return false;
	}
	const auto mrb = _mrb->mrb();
#define CHECK_EXISTS(NAME)                                                     \
	do {                                                                   \
		if (this->mrb()->respond_to(var, EULER_SYM(NAME))) {           \
			_methods.NAME = true;                                  \
			log()->debug(                                          \
			    "Found optional method :" #NAME " for state");     \
		} else {                                                       \
			log()->debug("No :" #NAME " method found for state");  \
		}                                                              \
	} while (0)
	CHECK_EXISTS(input);
	CHECK_EXISTS(update);
	CHECK_EXISTS(load);
	CHECK_EXISTS(draw);
	CHECK_EXISTS(quit);
#undef CHECK_EXISTS
	return true;
}

bool
State::load_text(std::string_view source, std::string_view data)
{
	auto cxt = mrb_ccontext_new(_mrb->mrb());
	cxt->capture_errors = true;
	const auto idx = mrb_gc_arena_save(mrb()->mrb());
#define CLEANUP()                                                              \
	do {                                                                   \
		mrb_ccontext_free(mrb()->mrb(), cxt);                          \
		mrb_gc_arena_restore(mrb()->mrb(), idx);                       \
	} while (0)
	try {
		mrb()->load_nstring_cxt(data.data(), data.size(), cxt);
		CLEANUP();
		return true;
	} catch (const std::exception &e) {
		log()->error("Failed to load source '{}': {}", source,
		    e.what());
		CLEANUP();
		return false;
	} catch (...) {
		log()->error("Unknown error while loading source '{}'", source);
		CLEANUP();
		return false;
	}
#undef CLEANUP
}
