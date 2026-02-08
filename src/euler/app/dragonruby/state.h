/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_STATE_H
#define EULER_APP_DRAGONRUBY_STATE_H

#include <dragonruby.h>

#include "euler/app/dragonruby/logger.h"
#include "euler/app/dragonruby/ruby_state.h"
#include "euler/app/dragonruby/storage.h"
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
	[[nodiscard]] const Modules &
	modules() const override
	{
		return _modules;
	}
	[[nodiscard]] Modules &
	modules() override
	{
		return _modules;
	}

private:
	util::Reference<RubyState> _mrb_state;
	util::Reference<Logger> _log;
	util::Reference<Storage> _user_storage;
	util::Reference<Storage> _title_storage;
	drb_api_t *_api;
	Modules _modules = {};
};
} /* namespace euler::app::dragonruby */

#endif /* EULER_APP_DRAGONRUBY_STATE_H */
