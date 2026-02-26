/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_STATE_H
#define EULER_APP_NATIVE_STATE_H

#include <filesystem>
#include <vector>

#include <SDL3/SDL_events.h>

#include "euler/app/native/logger.h"
#include "euler/app/native/ruby_state.h"
#include "euler/app/native/window.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/vulkan/renderer.h"

namespace euler::app::native {
class Logger;

class State : public util::State {
public:
	static constexpr nthread_t DEFAULT_THREAD_COUNT = 6;
	using Severity = util::Logger::Severity;
	struct Arguments {
		std::string progname;
		std::vector<std::filesystem::path> include;
		std::optional<std::filesystem::path> config;
		nthread_t thread_count = DEFAULT_THREAD_COUNT;
		Severity log_level = Severity::Info;
		std::filesystem::path file;
		static Arguments parse(int argc, char **argv);
	};
	~State() override;
	explicit State(const Arguments &args);
	bool initialize() override;


	[[nodiscard]] util::Reference<util::RubyState>
	mrb() const override
	{
		return _mrb;
	}
	[[nodiscard]] util::Reference<util::Logger> log() const override;
	[[nodiscard]] util::Reference<util::Storage>
	user_storage() const override;
	[[nodiscard]] util::Reference<util::Storage>
	title_storage() const override;
	[[nodiscard]] util::Reference<util::Image> load_image(
	    const char *path) override;
	void upload_image(const char *label,
	    const util::Reference<util::Image> &img) override;
	[[nodiscard]] tick_t ticks() const override;

	const std::string &
	progname() const override
	{
		return _progname;
	}

	const std::string &title() const override;

	bool loop(int &exit_code);
	float dt() const override;

private:
	bool update(int &exit_code);
	bool app_update(float dt);
	bool app_input(const SDL_Event &event);
	bool app_draw();
	bool app_load();
	bool app_quit();
	void set_ivs();
	bool check_global_state();
	bool load_text(std::string_view source, std::string_view data);


	struct HaveMethod {
		bool input : 1;
		bool update : 1;
		bool load : 1;
		bool draw : 1;
		bool quit : 1;
		HaveMethod()
		    : input(false)
		    , update(false)
		    , load(false)
		    , draw(false)
		    , quit(false)
		{
		}
	};

	struct {
		mrb_value self = mrb_nil_value();
		mrb_value log = mrb_nil_value();
		mrb_value user_storage = mrb_nil_value();
		mrb_value title_storage = mrb_nil_value();
		mrb_value window = mrb_nil_value();
	} _attributes;

	HaveMethod _methods;
	util::Reference<RubyState> _mrb;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _user_storage;
	util::Reference<util::Storage> _title_storage;
	util::Reference<vulkan::Renderer> _renderer;
	util::Reference<Window> _window;
	std::string _progname;
	std::string _title;
	std::vector<std::filesystem::path> _include;
	nthread_t _thread_count = DEFAULT_THREAD_COUNT;
	Severity _log_level = Severity::Info;
	std::filesystem::path _file;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_STATE_H */
