/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_STATE_H
#define EULER_APP_NATIVE_STATE_H

#include "euler/util/state.h"

namespace euler::app::native {
class State : public util::State {
public:
	struct Arguments {

	};

	State(const Arguments &);
	[[nodiscard]] util::Reference<graphics::Target>
	renderer() const override;
	[[nodiscard]] Runtime runtime() const override;
	[[nodiscard]] util::Reference<util::RubyState> mrb() const override;
	[[nodiscard]] RClass *object_class() const override;
	[[nodiscard]] util::Reference<util::Logger> log() const override;
	[[nodiscard]] nthread_t available_threads() const override;
	[[nodiscard]] tick_t ticks() const override;
	[[nodiscard]] tick_t last_tick() const override;
	[[nodiscard]] float fps() const override;
	[[nodiscard]] float dt() const override;
	[[nodiscard]] tick_t total_ticks() const override;
	[[nodiscard]] mrb_value gv_state() const override;

	[[nodiscard]] util::Reference<util::Window> window() override;
	[[nodiscard]] Phase phase() const override;
	void set_phase(Phase phase) override;
	[[nodiscard]] const std::string &progname() const override;
	[[nodiscard]] const std::string &title() const override;
	void upload_image(const char *label,
	    const util::Reference<util::Image> &img) override;
	bool preinit() override;
	bool initialize() override;
	void tick() override;
	mrb_value self_value() const override;
	[[nodiscard]] const Modules &modules() const override;
	[[nodiscard]] Modules &modules() override;
	[[nodiscard]] void *unwrap(mrb_value value,
	    const mrb_data_type *type) const override;

	bool loop(int &exit_code);


protected:
	const mrb_data_type *data_type() const override;

public:
	[[nodiscard]] util::Reference<util::ImageLoader>
	image_loader() override;

private:

};

util::Reference<State> make_state(const int argc, const char **argv);

} /* namespace euler::app::native */


#endif /* EULER_APP_NATIVE_STATE_H */

