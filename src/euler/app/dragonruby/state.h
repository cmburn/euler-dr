/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_STATE_H
#define EULER_APP_DRAGONRUBY_STATE_H

#include <dragonruby.h>

#include "euler/app/dragonruby/logger.h"
#include "euler/app/dragonruby/ruby_state.h"
#include "euler/util/image.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/util/storage.h"

namespace euler::app::dragonruby {
class State : public util::State {
public:
	struct Arguments {
		mrb_state *state;
		drb_api_t *api;
	};

	~State() override = default;
	explicit State(const Arguments &args);
	bool initialize() override;

	[[nodiscard]] util::Reference<util::RubyState>
	mrb() const override
	{
		return _mrb_state;
	}
	[[nodiscard]] util::Reference<util::Logger> log() const override;

	[[nodiscard]] util::Reference<util::Image> load_image(
	    const char *path) override;
	[[nodiscard]] tick_t ticks() const override;
	float dt() const override;
	const std::string &progname() const override;
	const std::string &title() const override;
	bool preinit() override;

private:
	util::Reference<RubyState> _mrb_state;
	util::Reference<Logger> _log;
	drb_api_t *_api;
};
} /* namespace euler::app::dragonruby */

#endif /* EULER_APP_DRAGONRUBY_STATE_H */
