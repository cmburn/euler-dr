/* SPDX-License-Identifier: ISC */

#include <cassert>

#include "euler/app/dragonruby/state.h"

#include "euler/app/dragonruby/image.h"
#include "euler/app/dragonruby/window.h"

#ifdef EULER_GUI
#include "euler/gui/context.h"
#endif

using euler::app::dragonruby::State;

State::~State() = default;

State::State(const Arguments &args)
    : _api(args.api)
{
	printf("State::State: %p\n", args.state);
	_ruby_state
	    = euler::util::make_reference<RubyState>(args.state, args.api);
	_log = euler::util::make_reference<Logger>("euler",
	    args.api->drb_log_write);
	args.state->ud = util::Reference(this).wrap();
}

bool
State::initialize()
{
	_window = util::make_reference<Window>(util::Reference(this));
	return true;
}

euler::util::Reference<euler::util::Logger>
State::log() const
{
	return _log;
}

euler::util::Reference<euler::util::Image>
State::load_image(const char *)
{
	return nullptr;
}

euler::util::Reference<euler::util::Image>
State::create_image(const char *, int16_t, int16_t, util::Color)
{
	return nullptr;
}

euler::util::Reference<euler::util::Window>
State::window()
{
	return _window;
}
#ifdef EULER_GUI
euler::util::Reference<euler::gui::Context>
State::gui() const
{
	return _gui;
}
#endif

State::tick_t
State::ticks() const
{
	return _api->SDL_GetTicks();
}
float
State::dt() const
{
	static constexpr float FPS = 1.0 / 60.0;
	return FPS;
}

const std::string &
State::progname() const
{
	static const std::string name = "dragonruby";
	return name;
}

const std::string &
State::title() const
{
	static const std::string title = "DragonRuby";
	return title;
}

bool
State::preinit()
{
	return true;
}

#ifdef EULER_GUI_BUILD
euler::util::Reference<euler::gui::Context>
State::gui() const
{
	return _gui;
}
#endif

void
State::upload_image(const char *label, const util::Reference<util::Image> &img)
{
	const auto dr_img = img.cast_to<Image>();
	_api->drb_upload_pixel_array(label, dr_img->_width, dr_img->_height,
	    static_cast<const uint32_t *>(dr_img->_data));
}

mrb_value
State::args() const
{
	return _args;
}

euler::util::Reference<euler::graphics::Target>
State::renderer() const
{
	return _renderer;
}

State::Runtime
State::runtime() const
{
	return Runtime::DragonRuby;
}
