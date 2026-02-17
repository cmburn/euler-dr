/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_STATE_H
#define EULER_APP_NATIVE_STATE_H

#include "SDL3/SDL_events.h"
#include "euler/app/native/ruby_state.h"
#include "euler/util/ext.h"
#include "euler/util/logger.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/util/storage.h"

namespace euler::app::native {
class Logger;

class State : public util::State {
public:
	struct Arguments { };
	~State() override;
	explicit State(const Arguments &args);
	static Arguments parse_args(int argc, char **argv);
	bool initialize() override;

	[[nodiscard]] util::Reference<util::RubyState>
	mrb() const override
	{
		return _mrb_state;
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

	bool loop(int &exit_code);

private:
	bool update(int &exit_code);
	bool app_update(float dt);
	bool app_input(const SDL_Event &event);
	bool app_draw();
	bool app_load();
	bool app_quit();
	void set_ivs();

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
		mrb_value system = mrb_nil_value();
		mrb_value log = mrb_nil_value();
		mrb_value user_storage = mrb_nil_value();
		mrb_value title_storage = mrb_nil_value();
		mrb_value window = mrb_nil_value();
	} _attributes;

	Phase _phase = Phase::Load;
	HaveMethod _methods;
	util::Reference<RubyState> _mrb_state;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _user_storage;
	util::Reference<util::Storage> _title_storage;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_STATE_H */
