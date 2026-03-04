/* SPDX-License-Identifier: ISC */

#ifndef EULER_EVENT_EVENT_H
#define EULER_EVENT_EVENT_H

#include "euler/util/state.h"

#include <SDL3/SDL.h>

namespace euler::event {

class Event {
public:
};

class WindowEvent : public Event {};
class DisplayEvent : public Event {};
class KeyboardEvent : public Event {};
class TextInputEvent : public Event {};
class KeyboardDeviceEvent : public Event {};
class TextEditingEvent : public Event {};
class TextEditingCandidatesEvent : public Event {};
class MouseMotionEvent : public Event {};
class MouseButtonEvent : public Event {};
class MouseWheelEvent : public Event {};
class MouseDeviceEvent : public Event {};
class JoystickAxisMotionEvent : public Event {};
class JoystickBallMotionEvent : public Event {};
class JoystickHatMotionEvent : public Event {};
class JoystickButtonEvent : public Event {};
class GamepadAxisMotionEvent : public Event {};
class GamepadMotionEvent : public Event {};
class GamepadDeviceEvent : public Event {};
class GamepadTouchpadEvent : public Event {};
class GamepadSensorEvent : public Event {};
class TouchFingerEvent : public Event {};
class ClipboardEvent : public Event {};
class DropEvent : public Event {};
class AudioDeviceEvent : public Event {};
class SensorEvent : public Event {};
class PenProximityEvent : public Event {};
class PenTouchEvent : public Event {};
class PenButtonEvent : public Event {};
class PenMotionEvent : public Event {};
class PenAxisEvent : public Event {};
class CameraDeviceEvent : public Event {};
class RenderEvent : public Event {};


mrb_value sdl_event_to_mrb(const util::Reference<util::State> &state,
    const SDL_Event &event);
RClass *init(const util::Reference<util::State> &state, RClass *mod, RClass * = nullptr);

} /* namespace euler::app */

#endif /* EULER_EVENT_EVENT_H */
