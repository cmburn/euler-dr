/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STATE_H
#define EULER_UTIL_STATE_H

#include <thread>

#include "euler/util/mruby_state.h"
#include "euler/util/object.h"

namespace euler::util {
class Logger;
class Storage;
class Image;
class State : public Object {
public:
	~State() override = default;
	State();
	typedef uint64_t tick_t;
	typedef decltype(std::thread::hardware_concurrency()) nthread_t;

	struct Module {
		RClass *mod;
		struct {
			RClass *mod;
		} util;
		struct {
			RClass *mod;
			RClass *cube;
			RClass *matrix;
			RClass *row_vector;
			RClass *sparse_matrix;
			RClass *vector;
		} math;
	};

	[[nodiscard]] virtual Reference<MRubyState> mrb() const = 0;
	[[nodiscard]] virtual RClass *object_class() const = 0;
	[[nodiscard]] virtual Reference<Logger> log() const = 0;
	[[nodiscard]] virtual Reference<Storage> user_storage() const = 0;
	[[nodiscard]] virtual Reference<Storage> title_storage() const = 0;
	[[nodiscard]] virtual nthread_t available_threads() const = 0;
	[[nodiscard]] virtual tick_t ticks() const = 0;
	[[nodiscard]] virtual tick_t last_tick() const = 0;
	[[nodiscard]] virtual float fps() const = 0;
	[[nodiscard]] virtual tick_t total_ticks() const = 0;
	[[nodiscard]] virtual mrb_value gv_state() const = 0;
	[[nodiscard]] static Reference<State> get(const mrb_state *mrb);
	[[nodiscard]] virtual Reference<Image> load_image(const char *path) = 0;
	virtual void upload_image(const char *label,
	    const Reference<Image> &img) = 0;
	virtual void initialize() = 0;
	virtual void tick() const = 0;
	[[nodiscard]] virtual const Module &modules() const = 0;
	[[nodiscard]] virtual Module &modules() = 0;
};

} /* namespace euler::util */

#endif /* EULER_UTIL_STATE_H */
