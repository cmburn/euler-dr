/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STATE_H
#define EULER_UTIL_STATE_H

#include <thread>

#include "euler/util/error.h"
#include "euler/util/object.h"
#include "euler/util/ruby_state.h"

namespace euler::util {
class Error;
class Logger;
class Storage;
class Image;

class State : public Object {
public:
	~State() override;
	State();
	typedef uint64_t tick_t;
	typedef decltype(std::thread::hardware_concurrency()) nthread_t;

	enum class Runtime : bool {
		Native,
		DragonRuby,
	};

	enum class Phase {
		Load,
		Input,
		Update,
		Draw,
		Quit,
	};

	struct Modules {
		RClass *mod = nullptr;
		struct {
			RClass *mod = nullptr;
			RClass *state = nullptr;
#ifdef EULER_NATIVE
			RClass *window = nullptr;
#endif
		} app;

		struct {
			RClass *mod = nullptr;
			RClass *base = nullptr;
			RClass *display = nullptr;
			RClass *window = nullptr;
			RClass *keyboard_device = nullptr;
			RClass *keyboard = nullptr;
			RClass *text_editing = nullptr;
			RClass *text_editing_candidates = nullptr;
			RClass *text_input = nullptr;
			RClass *mouse_device = nullptr;
			RClass *mouse_motion = nullptr;
			RClass *mouse_button = nullptr;
			RClass *mouse_wheel = nullptr;
			RClass *joystick_device = nullptr;
			RClass *joystick_axis_motion = nullptr;
			RClass *joystick_ball_motion = nullptr;
			RClass *joystick_hat_motion = nullptr;
			RClass *joystick_button = nullptr;
			RClass *joystick_battery_updated = nullptr;
			RClass *gamepad_device = nullptr;
			RClass *gamepad_axis_motion = nullptr;
			RClass *gamepad_button = nullptr;
			RClass *gamepad_touchpad = nullptr;
			RClass *gamepad_sensor = nullptr;
			RClass *audio_device = nullptr;
			RClass *camera_device = nullptr;
			RClass *sensor = nullptr;
			RClass *quit = nullptr;
			RClass *user = nullptr;
			RClass *touch_finger = nullptr;
			RClass *pen_proximity = nullptr;
			RClass *pen_touch = nullptr;
			RClass *pen_motion = nullptr;
			RClass *pen_button = nullptr;
			RClass *pen_axis = nullptr;
			RClass *render = nullptr;
			RClass *drop = nullptr;
			RClass *clipboard = nullptr;
		} event;

		struct {
			RClass *mod = nullptr;
			RClass *button = nullptr;
			RClass *checkbox = nullptr;
			RClass *color_picker = nullptr;
			RClass *dropdown = nullptr;
			RClass *element = nullptr;
			RClass *knob = nullptr;
			RClass *progress_bar = nullptr;
			RClass *radio_button = nullptr;
			RClass *row = nullptr;
			RClass *selectable = nullptr;
			RClass *slider = nullptr;
			RClass *style = nullptr;
			RClass *text = nullptr;
			RClass *widget = nullptr;
			RClass *window = nullptr;
		} gui;

		struct {
			RClass *mod = nullptr;
			RClass *animated_sprite = nullptr;
			RClass *basic_sprite = nullptr;
			RClass *camera = nullptr;
			RClass *color = nullptr;
			RClass *composite_sprite = nullptr;
			RClass *font = nullptr;
			RClass *image = nullptr;
			RClass *polygon = nullptr;
			RClass *sprite = nullptr;
			RClass *text = nullptr;
			RClass *window = nullptr;
		} graphics;

		struct {
			RClass *mod = nullptr;
			RClass *color = nullptr;
			RClass *version = nullptr;
			RClass *logger = nullptr;
			RClass *storage = nullptr;
		} util;

#ifdef EULER_MATH
		struct {
			RClass *mod = nullptr;
			RClass *nonscalar = nullptr;
			RClass *cube = nullptr;
			RClass *matrix = nullptr;
			RClass *row_vector = nullptr;
			RClass *running_stat = nullptr;
			RClass *sparse_matrix = nullptr;
			RClass *sparse_solve_factorizer = nullptr;
			RClass *size = nullptr;
			RClass *vector = nullptr;
		} math;
#endif

#ifdef EULER_PHYSICS
		struct {
			RClass *mod = nullptr;
			RClass *body = nullptr;
			RClass *chain = nullptr;
			RClass *contact = nullptr;
			RClass *distance_joint = nullptr;
			RClass *filter_joint = nullptr;
			RClass *joint = nullptr;
			RClass *motor_joint = nullptr;
			RClass *prismatic_joint = nullptr;
			RClass *revolute_joint = nullptr;
			RClass *shape = nullptr;
			RClass *weld_joint = nullptr;
			RClass *wheel_joint = nullptr;
			RClass *world = nullptr;
			RClass *capsule = nullptr;
			RClass *circle = nullptr;
			RClass *polygon = nullptr;
			RClass *segment = nullptr;
		} physics;
#endif
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
	[[nodiscard]] virtual float dt() const = 0;
	[[nodiscard]] virtual tick_t total_ticks() const = 0;
	[[nodiscard]] virtual mrb_value gv_state() const = 0;
	[[nodiscard]] static Reference<State> get(const mrb_state *mrb);
	[[nodiscard]] virtual Reference<Image> load_image(const char *path) = 0;
	[[nodiscard]] virtual Phase phase() const = 0;
	virtual void set_phase(Phase phase) = 0;
	[[nodiscard]] virtual const std::string &progname() const = 0;
	[[nodiscard]] virtual const std::string &title() const = 0;
	virtual void upload_image(const char *label,
	    const Reference<Image> &img)
	    = 0;
	virtual bool initialize() = 0;
	virtual void tick() = 0;
	virtual mrb_value self_value() const = 0;
	[[nodiscard]] virtual const Modules &modules() const = 0;
	[[nodiscard]] virtual Modules &modules() = 0;
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
		auto self = util::Reference(const_cast<State *>(this));
		return obj.wrap(mrb()->mrb(), T::fetch_class(self), &T::TYPE);
	}

protected:
	virtual const mrb_data_type *data_type() const = 0;
};

#define EULER_SYM_LIT(LIT)                                                     \
	((::euler::util::State::get(mrb)->mrb()->intern_static(LIT,            \
	    sizeof(LIT) - 1)))

#define EULER_SYM(SYM) EULER_SYM_LIT(#SYM)

#define EULER_IVSYM(SYM) EULER_SYM_LIT("@" #SYM)

#define EULER_SYM_VAL(SYM) (mrb_symbol_value(EULER_SYM(SYM)))

} /* namespace euler::util */

#endif /* EULER_UTIL_STATE_H */
