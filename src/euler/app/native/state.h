/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_STATE_H
#define EULER_APP_NATIVE_STATE_H

#include "euler/app/native/ruby_state.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/util/logger.h"
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
	util::Reference<RubyState> _mrb_state;
	util::Reference<util::Logger> _log;
	util::Reference<util::Storage> _user_storage;
	util::Reference<util::Storage> _title_storage;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_STATE_H */
