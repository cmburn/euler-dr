/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_STATE_H
#define EULER_APP_DRAGONRUBY_STATE_H

#include <dragonruby.h>

#include "euler/app/dragonruby/logger.h"
#include "euler/app/dragonruby/ruby_state.h"
#include "euler/util/image.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "window.h"
#ifdef EULER_GUI_BUILD
#include "euler/gui/context.h"
#endif


namespace euler::app::dragonruby {
class State : public util::State {
public:
	struct Arguments {
		mrb_state *state;
		drb_api_t *api;
	};

	~State() override;
	explicit State(const Arguments &args);
	bool initialize() override;

	[[nodiscard]] util::Reference<util::RubyState>
	mrb() const override
	{
		return _ruby_state;
	}
	[[nodiscard]] util::Reference<util::Logger> log() const override;

	[[nodiscard]] util::Reference<util::Image> load_image(
	    const char *path) override;
	util::Reference<util::Image> create_image(const char *label, int16_t w,
	    int16_t h, util::Color) override;
	util::Reference<util::Window> window() override;
#ifdef EULER_GUI
	[[nodiscard]] util::Reference<gui::Context> gui() const override;
#endif
	[[nodiscard]] tick_t ticks() const override;
	float dt() const override;
	const std::string &progname() const override;
	const std::string &title() const override;
	bool preinit() override;
	void upload_image(const char *label,
	    const util::Reference<util::Image> &img) override;


private:
	util::Reference<RubyState> _ruby_state;
	util::Reference<Logger> _log;
#ifdef EULER_GUI
	util::Reference<gui::Context> _gui;
#endif
	util::Reference<Window> _window;
	drb_api_t *_api;
};
} /* namespace euler::app::dragonruby */

#endif /* EULER_APP_DRAGONRUBY_STATE_H */
