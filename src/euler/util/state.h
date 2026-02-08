/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STATE_H
#define EULER_UTIL_STATE_H

#include <thread>

#include "euler/util/object.h"
#include "euler/util/ruby_state.h"

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

	enum class Runtime {
		Native,
		DragonRuby,
	};

	struct Module {
		RClass *mod;

		struct {
			RClass *mod;
			RClass *state;
		} app;

		struct {
			RClass *mod;
		} util;

		struct {
			RClass *mod;
			RClass *nonscalar;
			RClass *cube;
			RClass *matrix;
			RClass *row_vector;
			RClass *sparse_matrix;
			RClass *size;
			RClass *vector;
		} math;

		struct {
			RClass *mod;
			RClass *body;
			RClass *chain;
			RClass *contact;
			RClass *distance_joint;
			RClass *filter_joint;
			RClass *joint;
			RClass *motor_joint;
			RClass *prismatic_joint;
			RClass *revolute_joint;
			RClass *shape;
			RClass *weld_joint;
			RClass *wheel_joint;
			RClass *world;
			RClass *capsule;
			RClass *circle;
			RClass *polygon;
			RClass *segment;
		} physics;
	};

	[[nodiscard]] virtual Reference<RubyState> mrb() const = 0;
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
	    const Reference<Image> &img)
	    = 0;
	virtual void initialize() = 0;
	virtual void tick() const = 0;
	[[nodiscard]] virtual const Module &modules() const = 0;
	[[nodiscard]] virtual Module &modules() = 0;
	[[nodiscard]] virtual void *unwrap(mrb_value value,
	    const mrb_data_type *type) const
	    = 0;

	template <typename T>
	[[nodiscard]] Reference<T>
	unwrap(mrb_value value) const
	{
		auto ptr = unwrap(value, &T::TYPE);
		if (ptr == nullptr) return Reference<T>(nullptr);
		return Reference<T>::unwrap(ptr);
	}

	template <typename T>
	[[nodiscard]] mrb_value
	wrap(Reference<T> &obj) const
	{
		if (obj == nullptr) return mrb_nil_value();
		return obj.wrap(mrb()->mrb(), object_class(), &T::TYPE);
	}
};

#define EULER_SYM(SYM)                                                         \
	(mrb_symbol_value(                                                     \
	    (::euler::util::State::get(mrb)->mrb()->intern_static(#SYM,        \
		sizeof(#SYM) - 1))))

} /* namespace euler::util */

#endif /* EULER_UTIL_STATE_H */
