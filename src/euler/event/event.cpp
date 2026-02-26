/* SPDX-License-Identifier: ISC */

#include "euler/event/event.h"

#include <cassert>

#include <SDL3/SDL.h>

#include "euler/util/state.h"

static mrb_sym
sdl_event_sym([[maybe_unused]] mrb_state *mrb, const SDL_Event &e)
{
	switch (static_cast<SDL_EventType>(e.type)) {
	case SDL_EVENT_QUIT: return EULER_SYM(quit);
	case SDL_EVENT_DISPLAY_ORIENTATION:
		return EULER_SYM(display_orientation);
	case SDL_EVENT_DISPLAY_ADDED: return EULER_SYM(display_added);
	case SDL_EVENT_DISPLAY_REMOVED: return EULER_SYM(display_removed);
	case SDL_EVENT_DISPLAY_MOVED: return EULER_SYM(display_moved);
	case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
		return EULER_SYM(display_desktop_mode_changed);
	case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
		return EULER_SYM(display_current_mode_changed);
	case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
		return EULER_SYM(display_content_scale_changed);
	case SDL_EVENT_WINDOW_SHOWN: return EULER_SYM(window_shown);
	case SDL_EVENT_WINDOW_HIDDEN: return EULER_SYM(window_hidden);
	case SDL_EVENT_WINDOW_EXPOSED: return EULER_SYM(window_exposed);
	case SDL_EVENT_WINDOW_MOVED: return EULER_SYM(window_moved);
	case SDL_EVENT_WINDOW_RESIZED: return EULER_SYM(window_resized);
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		return EULER_SYM(window_pixel_size_changed);
	case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
		return EULER_SYM(window_metal_view_resized);
	case SDL_EVENT_WINDOW_MINIMIZED: return EULER_SYM(window_minimized);
	case SDL_EVENT_WINDOW_MAXIMIZED: return EULER_SYM(window_maximized);
	case SDL_EVENT_WINDOW_RESTORED: return EULER_SYM(window_restored);
	case SDL_EVENT_WINDOW_MOUSE_ENTER: return EULER_SYM(window_mouse_enter);
	case SDL_EVENT_WINDOW_MOUSE_LEAVE: return EULER_SYM(window_mouse_leave);
	case SDL_EVENT_WINDOW_FOCUS_GAINED:
		return EULER_SYM(window_focus_gained);
	case SDL_EVENT_WINDOW_FOCUS_LOST: return EULER_SYM(window_focus_lost);
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		return EULER_SYM(window_close_requested);
	case SDL_EVENT_WINDOW_HIT_TEST: return EULER_SYM(window_hit_test);
	case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
		return EULER_SYM(window_iccprof_changed);
	case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
		return EULER_SYM(window_display_changed);
	case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
		return EULER_SYM(window_display_scale_changed);
	case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
		return EULER_SYM(window_safe_area_changed);
	case SDL_EVENT_WINDOW_OCCLUDED: return EULER_SYM(window_occluded);
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		return EULER_SYM(window_enter_fullscreen);
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		return EULER_SYM(window_leave_fullscreen);
	case SDL_EVENT_WINDOW_DESTROYED: return EULER_SYM(window_destroyed);
	case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
		return EULER_SYM(window_hdr_state_changed);
	case SDL_EVENT_KEY_DOWN: return EULER_SYM(key_down);
	case SDL_EVENT_KEY_UP: return EULER_SYM(key_up);
	case SDL_EVENT_TEXT_EDITING: return EULER_SYM(text_editing);
	case SDL_EVENT_TEXT_INPUT: return EULER_SYM(text_input);
	case SDL_EVENT_KEYMAP_CHANGED: return EULER_SYM(keymap_changed);
	case SDL_EVENT_KEYBOARD_ADDED: return EULER_SYM(keyboard_added);
	case SDL_EVENT_KEYBOARD_REMOVED: return EULER_SYM(keyboard_removed);
	case SDL_EVENT_TEXT_EDITING_CANDIDATES:
		return EULER_SYM(text_editing_candidates);
	case SDL_EVENT_MOUSE_MOTION: return EULER_SYM(mouse_motion);
	case SDL_EVENT_MOUSE_BUTTON_DOWN: return EULER_SYM(mouse_button_down);
	case SDL_EVENT_MOUSE_BUTTON_UP: return EULER_SYM(mouse_button_up);
	case SDL_EVENT_MOUSE_WHEEL: return EULER_SYM(mouse_wheel);
	case SDL_EVENT_MOUSE_ADDED: return EULER_SYM(mouse_added);
	case SDL_EVENT_MOUSE_REMOVED: return EULER_SYM(mouse_removed);
	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		return EULER_SYM(joystick_axis_motion);
	case SDL_EVENT_JOYSTICK_BALL_MOTION:
		return EULER_SYM(joystick_ball_motion);
	case SDL_EVENT_JOYSTICK_HAT_MOTION:
		return EULER_SYM(joystick_hat_motion);
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
		return EULER_SYM(joystick_button_down);
	case SDL_EVENT_JOYSTICK_BUTTON_UP: return EULER_SYM(joystick_button_up);
	case SDL_EVENT_JOYSTICK_ADDED: return EULER_SYM(joystick_added);
	case SDL_EVENT_JOYSTICK_REMOVED: return EULER_SYM(joystick_removed);
	case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		return EULER_SYM(joystick_battery_updated);
	case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
		return EULER_SYM(joystick_update_complete);
	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		return EULER_SYM(gamepad_axis_motion);
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		return EULER_SYM(gamepad_button_down);
	case SDL_EVENT_GAMEPAD_BUTTON_UP: return EULER_SYM(gamepad_button_up);
	case SDL_EVENT_GAMEPAD_ADDED: return EULER_SYM(gamepad_added);
	case SDL_EVENT_GAMEPAD_REMOVED: return EULER_SYM(gamepad_removed);
	case SDL_EVENT_GAMEPAD_REMAPPED: return EULER_SYM(gamepad_remapped);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
		return EULER_SYM(gamepad_touchpad_down);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
		return EULER_SYM(gamepad_touchpad_motion);
	case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
		return EULER_SYM(gamepad_touchpad_up);
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
		return EULER_SYM(gamepad_sensor_update);
	case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
		return EULER_SYM(gamepad_update_complete);
	case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
		return EULER_SYM(gamepad_steam_handle_updated);
	case SDL_EVENT_FINGER_DOWN: return EULER_SYM(finger_down);
	case SDL_EVENT_FINGER_UP: return EULER_SYM(finger_up);
	case SDL_EVENT_FINGER_MOTION: return EULER_SYM(finger_motion);
	case SDL_EVENT_FINGER_CANCELED: return EULER_SYM(finger_canceled);
	case SDL_EVENT_CLIPBOARD_UPDATE: return EULER_SYM(clipboard_update);
	case SDL_EVENT_DROP_FILE: return EULER_SYM(drop_file);
	case SDL_EVENT_DROP_TEXT: return EULER_SYM(drop_text);
	case SDL_EVENT_DROP_BEGIN: return EULER_SYM(drop_begin);
	case SDL_EVENT_DROP_COMPLETE: return EULER_SYM(drop_complete);
	case SDL_EVENT_DROP_POSITION: return EULER_SYM(drop_position);
	case SDL_EVENT_AUDIO_DEVICE_ADDED: return EULER_SYM(audio_device_added);
	case SDL_EVENT_AUDIO_DEVICE_REMOVED:
		return EULER_SYM(audio_device_removed);
	case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
		return EULER_SYM(audio_device_format_changed);
	case SDL_EVENT_SENSOR_UPDATE: return EULER_SYM(sensor_update);
	case SDL_EVENT_PEN_PROXIMITY_IN: return EULER_SYM(pen_proximity_in);
	case SDL_EVENT_PEN_PROXIMITY_OUT: return EULER_SYM(pen_proximity_out);
	case SDL_EVENT_PEN_DOWN: return EULER_SYM(pen_down);
	case SDL_EVENT_PEN_UP: return EULER_SYM(pen_up);
	case SDL_EVENT_PEN_BUTTON_DOWN: return EULER_SYM(pen_button_down);
	case SDL_EVENT_PEN_BUTTON_UP: return EULER_SYM(pen_button_up);
	case SDL_EVENT_PEN_MOTION: return EULER_SYM(pen_motion);
	case SDL_EVENT_PEN_AXIS: return EULER_SYM(pen_axis);
	case SDL_EVENT_CAMERA_DEVICE_ADDED:
		return EULER_SYM(camera_device_added);
	case SDL_EVENT_CAMERA_DEVICE_REMOVED:
		return EULER_SYM(camera_device_removed);
	case SDL_EVENT_CAMERA_DEVICE_APPROVED:
		return EULER_SYM(camera_device_approved);
	case SDL_EVENT_CAMERA_DEVICE_DENIED:
		return EULER_SYM(camera_device_denied);
	case SDL_EVENT_RENDER_TARGETS_RESET:
		return EULER_SYM(render_targets_reset);
	case SDL_EVENT_RENDER_DEVICE_RESET:
		return EULER_SYM(render_device_reset);
	case SDL_EVENT_RENDER_DEVICE_LOST: return EULER_SYM(render_device_lost);
	default: return 0;
	}
}

static mrb_sym
sdl_scancode_sym(mrb_state *mrb, const SDL_Scancode code)
{
	switch (code) {
	case SDL_SCANCODE_0: return EULER_SYM_LIT("0");
	case SDL_SCANCODE_1: return EULER_SYM_LIT("1");
	case SDL_SCANCODE_2: return EULER_SYM_LIT("2");
	case SDL_SCANCODE_3: return EULER_SYM_LIT("3");
	case SDL_SCANCODE_4: return EULER_SYM_LIT("4");
	case SDL_SCANCODE_5: return EULER_SYM_LIT("5");
	case SDL_SCANCODE_6: return EULER_SYM_LIT("6");
	case SDL_SCANCODE_7: return EULER_SYM_LIT("7");
	case SDL_SCANCODE_8: return EULER_SYM_LIT("8");
	case SDL_SCANCODE_9: return EULER_SYM_LIT("9");
	case SDL_SCANCODE_A: return EULER_SYM(a);
	case SDL_SCANCODE_AC_BACK: return EULER_SYM(ac_back);
	case SDL_SCANCODE_AC_BOOKMARKS: return EULER_SYM(ac_bookmarks);
	case SDL_SCANCODE_AC_CLOSE: return EULER_SYM(ac_close);
	case SDL_SCANCODE_AC_EXIT: return EULER_SYM(ac_exit);
	case SDL_SCANCODE_AC_FORWARD: return EULER_SYM(ac_forward);
	case SDL_SCANCODE_AC_HOME: return EULER_SYM(ac_home);
	case SDL_SCANCODE_AC_NEW: return EULER_SYM(ac_new);
	case SDL_SCANCODE_AC_OPEN: return EULER_SYM(ac_open);
	case SDL_SCANCODE_AC_PRINT: return EULER_SYM(ac_print);
	case SDL_SCANCODE_AC_PROPERTIES: return EULER_SYM(ac_properties);
	case SDL_SCANCODE_AC_REFRESH: return EULER_SYM(ac_refresh);
	case SDL_SCANCODE_AC_SAVE: return EULER_SYM(ac_save);
	case SDL_SCANCODE_AC_SEARCH: return EULER_SYM(ac_search);
	case SDL_SCANCODE_AC_STOP: return EULER_SYM(ac_stop);
	case SDL_SCANCODE_AGAIN: return EULER_SYM(again);
	case SDL_SCANCODE_ALTERASE: return EULER_SYM(alterase);
	case SDL_SCANCODE_APOSTROPHE: return EULER_SYM(apostrophe);
	case SDL_SCANCODE_APPLICATION: return EULER_SYM(application);
	case SDL_SCANCODE_B: return EULER_SYM(b);
	case SDL_SCANCODE_BACKSLASH: return EULER_SYM(backslash);
	case SDL_SCANCODE_BACKSPACE: return EULER_SYM(backspace);
	case SDL_SCANCODE_C: return EULER_SYM(c);
	case SDL_SCANCODE_CANCEL: return EULER_SYM(cancel);
	case SDL_SCANCODE_CAPSLOCK: return EULER_SYM(capslock);
	case SDL_SCANCODE_CHANNEL_DECREMENT:
		return EULER_SYM(channel_decrement);
	case SDL_SCANCODE_CHANNEL_INCREMENT:
		return EULER_SYM(channel_increment);
	case SDL_SCANCODE_CLEAR: return EULER_SYM(clear);
	case SDL_SCANCODE_CLEARAGAIN: return EULER_SYM(clearagain);
	case SDL_SCANCODE_COMMA: return EULER_SYM(comma);
	case SDL_SCANCODE_COPY: return EULER_SYM(copy);
	case SDL_SCANCODE_CRSEL: return EULER_SYM(crsel);
	case SDL_SCANCODE_CURRENCYSUBUNIT: return EULER_SYM(currencysubunit);
	case SDL_SCANCODE_CURRENCYUNIT: return EULER_SYM(currencyunit);
	case SDL_SCANCODE_CUT: return EULER_SYM(cut);
	case SDL_SCANCODE_D: return EULER_SYM(d);
	case SDL_SCANCODE_DECIMALSEPARATOR: return EULER_SYM(decimalseparator);
	case SDL_SCANCODE_DELETE: return EULER_SYM(delete);
	case SDL_SCANCODE_DOWN: return EULER_SYM(down);
	case SDL_SCANCODE_E: return EULER_SYM(e);
	case SDL_SCANCODE_END: return EULER_SYM(end);
	case SDL_SCANCODE_EQUALS: return EULER_SYM(equals);
	case SDL_SCANCODE_ESCAPE: return EULER_SYM(escape);
	case SDL_SCANCODE_EXECUTE: return EULER_SYM(execute);
	case SDL_SCANCODE_EXSEL: return EULER_SYM(exsel);
	case SDL_SCANCODE_F10: return EULER_SYM(f10);
	case SDL_SCANCODE_F11: return EULER_SYM(f11);
	case SDL_SCANCODE_F12: return EULER_SYM(f12);
	case SDL_SCANCODE_F13: return EULER_SYM(f13);
	case SDL_SCANCODE_F14: return EULER_SYM(f14);
	case SDL_SCANCODE_F15: return EULER_SYM(f15);
	case SDL_SCANCODE_F16: return EULER_SYM(f16);
	case SDL_SCANCODE_F17: return EULER_SYM(f17);
	case SDL_SCANCODE_F18: return EULER_SYM(f18);
	case SDL_SCANCODE_F19: return EULER_SYM(f19);
	case SDL_SCANCODE_F1: return EULER_SYM(f1);
	case SDL_SCANCODE_F20: return EULER_SYM(f20);
	case SDL_SCANCODE_F21: return EULER_SYM(f21);
	case SDL_SCANCODE_F22: return EULER_SYM(f22);
	case SDL_SCANCODE_F23: return EULER_SYM(f23);
	case SDL_SCANCODE_F24: return EULER_SYM(f24);
	case SDL_SCANCODE_F2: return EULER_SYM(f2);
	case SDL_SCANCODE_F3: return EULER_SYM(f3);
	case SDL_SCANCODE_F4: return EULER_SYM(f4);
	case SDL_SCANCODE_F5: return EULER_SYM(f5);
	case SDL_SCANCODE_F6: return EULER_SYM(f6);
	case SDL_SCANCODE_F7: return EULER_SYM(f7);
	case SDL_SCANCODE_F8: return EULER_SYM(f8);
	case SDL_SCANCODE_F9: return EULER_SYM(f9);
	case SDL_SCANCODE_F: return EULER_SYM(f);
	case SDL_SCANCODE_FIND: return EULER_SYM(find);
	case SDL_SCANCODE_G: return EULER_SYM(g);
	case SDL_SCANCODE_GRAVE: return EULER_SYM(grave);
	case SDL_SCANCODE_H: return EULER_SYM(h);
	case SDL_SCANCODE_HELP: return EULER_SYM(help);
	case SDL_SCANCODE_HOME: return EULER_SYM(home);
	case SDL_SCANCODE_I: return EULER_SYM(i);
	case SDL_SCANCODE_INSERT: return EULER_SYM(insert);
	case SDL_SCANCODE_J: return EULER_SYM(j);
	case SDL_SCANCODE_K: return EULER_SYM(k);
	case SDL_SCANCODE_KP_000: return EULER_SYM(kp_000);
	case SDL_SCANCODE_KP_00: return EULER_SYM(kp_00);
	case SDL_SCANCODE_KP_0: return EULER_SYM(kp_0);
	case SDL_SCANCODE_KP_1: return EULER_SYM(kp_1);
	case SDL_SCANCODE_KP_2: return EULER_SYM(kp_2);
	case SDL_SCANCODE_KP_3: return EULER_SYM(kp_3);
	case SDL_SCANCODE_KP_4: return EULER_SYM(kp_4);
	case SDL_SCANCODE_KP_5: return EULER_SYM(kp_5);
	case SDL_SCANCODE_KP_6: return EULER_SYM(kp_6);
	case SDL_SCANCODE_KP_7: return EULER_SYM(kp_7);
	case SDL_SCANCODE_KP_8: return EULER_SYM(kp_8);
	case SDL_SCANCODE_KP_9: return EULER_SYM(kp_9);
	case SDL_SCANCODE_KP_A: return EULER_SYM(kp_a);
	case SDL_SCANCODE_KP_AMPERSAND: return EULER_SYM(kp_ampersand);
	case SDL_SCANCODE_KP_AT: return EULER_SYM(kp_at);
	case SDL_SCANCODE_KP_B: return EULER_SYM(kp_b);
	case SDL_SCANCODE_KP_BACKSPACE: return EULER_SYM(kp_backspace);
	case SDL_SCANCODE_KP_BINARY: return EULER_SYM(kp_binary);
	case SDL_SCANCODE_KP_C: return EULER_SYM(kp_c);
	case SDL_SCANCODE_KP_CLEAR: return EULER_SYM(kp_clear);
	case SDL_SCANCODE_KP_CLEARENTRY: return EULER_SYM(kp_clearentry);
	case SDL_SCANCODE_KP_COLON: return EULER_SYM(kp_colon);
	case SDL_SCANCODE_KP_COMMA: return EULER_SYM(kp_comma);
	case SDL_SCANCODE_KP_D: return EULER_SYM(kp_d);
	case SDL_SCANCODE_KP_DBLAMPERSAND: return EULER_SYM(kp_dblampersand);
	case SDL_SCANCODE_KP_DBLVERTICALBAR:
		return EULER_SYM(kp_dblverticalbar);
	case SDL_SCANCODE_KP_DECIMAL: return EULER_SYM(kp_decimal);
	case SDL_SCANCODE_KP_DIVIDE: return EULER_SYM(kp_divide);
	case SDL_SCANCODE_KP_E: return EULER_SYM(kp_e);
	case SDL_SCANCODE_KP_ENTER: return EULER_SYM(kp_enter);
	case SDL_SCANCODE_KP_EQUALS: return EULER_SYM(kp_equals);
	case SDL_SCANCODE_KP_EQUALSAS400: return EULER_SYM(kp_equalsas400);
	case SDL_SCANCODE_INTERNATIONAL3: return EULER_SYM(yen);
	case SDL_SCANCODE_LANG1: return EULER_SYM(hangul_english);
	case SDL_SCANCODE_LANG2: return EULER_SYM(hanja);
	case SDL_SCANCODE_LANG3: return EULER_SYM(katakana);
	case SDL_SCANCODE_LANG4: return EULER_SYM(hiragana);
	case SDL_SCANCODE_LANG5: return EULER_SYM(zenkaku_hankaku);
	case SDL_SCANCODE_KP_EXCLAM: return EULER_SYM(kp_exclam);
	case SDL_SCANCODE_KP_F: return EULER_SYM(kp_f);
	case SDL_SCANCODE_KP_GREATER: return EULER_SYM(kp_greater);
	case SDL_SCANCODE_KP_HASH: return EULER_SYM(kp_hash);
	case SDL_SCANCODE_KP_HEXADECIMAL: return EULER_SYM(kp_hexadecimal);
	case SDL_SCANCODE_KP_LEFTBRACE: return EULER_SYM(kp_leftbrace);
	case SDL_SCANCODE_KP_LEFTPAREN: return EULER_SYM(kp_leftparen);
	case SDL_SCANCODE_KP_LESS: return EULER_SYM(kp_less);
	case SDL_SCANCODE_KP_MEMADD: return EULER_SYM(kp_memadd);
	case SDL_SCANCODE_KP_MEMCLEAR: return EULER_SYM(kp_memclear);
	case SDL_SCANCODE_KP_MEMDIVIDE: return EULER_SYM(kp_memdivide);
	case SDL_SCANCODE_KP_MEMMULTIPLY: return EULER_SYM(kp_memmultiply);
	case SDL_SCANCODE_KP_MEMRECALL: return EULER_SYM(kp_memrecall);
	case SDL_SCANCODE_KP_MEMSTORE: return EULER_SYM(kp_memstore);
	case SDL_SCANCODE_KP_MEMSUBTRACT: return EULER_SYM(kp_memsubtract);
	case SDL_SCANCODE_KP_MINUS: return EULER_SYM(kp_minus);
	case SDL_SCANCODE_KP_MULTIPLY: return EULER_SYM(kp_multiply);
	case SDL_SCANCODE_KP_OCTAL: return EULER_SYM(kp_octal);
	case SDL_SCANCODE_KP_PERCENT: return EULER_SYM(kp_percent);
	case SDL_SCANCODE_KP_PERIOD: return EULER_SYM(kp_period);
	case SDL_SCANCODE_KP_PLUS: return EULER_SYM(kp_plus);
	case SDL_SCANCODE_KP_PLUSMINUS: return EULER_SYM(kp_plusminus);
	case SDL_SCANCODE_KP_POWER: return EULER_SYM(kp_power);
	case SDL_SCANCODE_KP_RIGHTBRACE: return EULER_SYM(kp_rightbrace);
	case SDL_SCANCODE_KP_RIGHTPAREN: return EULER_SYM(kp_rightparen);
	case SDL_SCANCODE_KP_SPACE: return EULER_SYM(kp_space);
	case SDL_SCANCODE_KP_TAB: return EULER_SYM(kp_tab);
	case SDL_SCANCODE_KP_VERTICALBAR: return EULER_SYM(kp_verticalbar);
	case SDL_SCANCODE_KP_XOR: return EULER_SYM(kp_xor);
	case SDL_SCANCODE_L: return EULER_SYM(l);
	case SDL_SCANCODE_LALT: return EULER_SYM(lalt);
	case SDL_SCANCODE_LCTRL: return EULER_SYM(lctrl);
	case SDL_SCANCODE_LEFT: return EULER_SYM(left);
	case SDL_SCANCODE_LEFTBRACKET: return EULER_SYM(leftbracket);
	case SDL_SCANCODE_LGUI: return EULER_SYM(lgui);
	case SDL_SCANCODE_LSHIFT: return EULER_SYM(lshift);
	case SDL_SCANCODE_M: return EULER_SYM(m);
	case SDL_SCANCODE_MEDIA_EJECT: return EULER_SYM(media_eject);
	case SDL_SCANCODE_MEDIA_FAST_FORWARD:
		return EULER_SYM(media_fast_forward);
	case SDL_SCANCODE_MEDIA_NEXT_TRACK: return EULER_SYM(media_next_track);
	case SDL_SCANCODE_MEDIA_PAUSE: return EULER_SYM(media_pause);
	case SDL_SCANCODE_MEDIA_PLAY: return EULER_SYM(media_play);
	case SDL_SCANCODE_MEDIA_PLAY_PAUSE: return EULER_SYM(media_play_pause);
	case SDL_SCANCODE_MEDIA_PREVIOUS_TRACK:
		return EULER_SYM(media_previous_track);
	case SDL_SCANCODE_MEDIA_RECORD: return EULER_SYM(media_record);
	case SDL_SCANCODE_MEDIA_REWIND: return EULER_SYM(media_rewind);
	case SDL_SCANCODE_MEDIA_SELECT: return EULER_SYM(media_select);
	case SDL_SCANCODE_MEDIA_STOP: return EULER_SYM(media_stop);
	case SDL_SCANCODE_MENU: return EULER_SYM(menu);
	case SDL_SCANCODE_MINUS: return EULER_SYM(minus);
	case SDL_SCANCODE_MODE: return EULER_SYM(mode);
	case SDL_SCANCODE_MUTE: return EULER_SYM(mute);
	case SDL_SCANCODE_N: return EULER_SYM(n);
	case SDL_SCANCODE_NONUSBACKSLASH: return EULER_SYM(nonusbackslash);
	case SDL_SCANCODE_NONUSHASH: return EULER_SYM(nonushash);
	case SDL_SCANCODE_NUMLOCKCLEAR: return EULER_SYM(numlockclear);
	case SDL_SCANCODE_O: return EULER_SYM(o);
	case SDL_SCANCODE_OPER: return EULER_SYM(oper);
	case SDL_SCANCODE_OUT: return EULER_SYM(out);
	case SDL_SCANCODE_P: return EULER_SYM(p);
	case SDL_SCANCODE_PAGEDOWN: return EULER_SYM(pagedown);
	case SDL_SCANCODE_PAGEUP: return EULER_SYM(pageup);
	case SDL_SCANCODE_PASTE: return EULER_SYM(paste);
	case SDL_SCANCODE_PAUSE: return EULER_SYM(pause);
	case SDL_SCANCODE_PERIOD: return EULER_SYM(period);
	case SDL_SCANCODE_POWER: return EULER_SYM(power);
	case SDL_SCANCODE_PRINTSCREEN: return EULER_SYM(printscreen);
	case SDL_SCANCODE_PRIOR: return EULER_SYM(prior);
	case SDL_SCANCODE_Q: return EULER_SYM(q);
	case SDL_SCANCODE_R: return EULER_SYM(r);
	case SDL_SCANCODE_RALT: return EULER_SYM(ralt);
	case SDL_SCANCODE_RCTRL: return EULER_SYM(rctrl);
	case SDL_SCANCODE_RETURN2: return EULER_SYM(return2);
	case SDL_SCANCODE_RETURN: return EULER_SYM(return);
	case SDL_SCANCODE_RGUI: return EULER_SYM(rgui);
	case SDL_SCANCODE_RIGHT: return EULER_SYM(right);
	case SDL_SCANCODE_RIGHTBRACKET: return EULER_SYM(rightbracket);
	case SDL_SCANCODE_RSHIFT: return EULER_SYM(rshift);
	case SDL_SCANCODE_S: return EULER_SYM(s);
	case SDL_SCANCODE_SCROLLLOCK: return EULER_SYM(scrolllock);
	case SDL_SCANCODE_SELECT: return EULER_SYM(select);
	case SDL_SCANCODE_SEMICOLON: return EULER_SYM(semicolon);
	case SDL_SCANCODE_SEPARATOR: return EULER_SYM(separator);
	case SDL_SCANCODE_SLASH: return EULER_SYM(slash);
	case SDL_SCANCODE_SLEEP: return EULER_SYM(sleep);
	case SDL_SCANCODE_SPACE: return EULER_SYM(space);
	case SDL_SCANCODE_STOP: return EULER_SYM(stop);
	case SDL_SCANCODE_SYSREQ: return EULER_SYM(sysreq);
	case SDL_SCANCODE_T: return EULER_SYM(t);
	case SDL_SCANCODE_TAB: return EULER_SYM(tab);
	case SDL_SCANCODE_THOUSANDSSEPARATOR:
		return EULER_SYM(thousandsseparator);
	case SDL_SCANCODE_U: return EULER_SYM(u);
	case SDL_SCANCODE_UNDO: return EULER_SYM(undo);
	case SDL_SCANCODE_UP: return EULER_SYM(up);
	case SDL_SCANCODE_V: return EULER_SYM(v);
	case SDL_SCANCODE_VOLUMEDOWN: return EULER_SYM(volumedown);
	case SDL_SCANCODE_VOLUMEUP: return EULER_SYM(volumeup);
	case SDL_SCANCODE_W: return EULER_SYM(w);
	case SDL_SCANCODE_WAKE: return EULER_SYM(wake);
	case SDL_SCANCODE_X: return EULER_SYM(x);
	case SDL_SCANCODE_Y: return EULER_SYM(y);
	case SDL_SCANCODE_Z: return EULER_SYM(z);
	default: return EULER_SYM(unknown);
	}
}

static mrb_sym
sdl_key_sym(mrb_state *mrb, SDL_Keycode code)
{
	switch (code) {
	case SDLK_RETURN: return EULER_SYM(return);
	case SDLK_ESCAPE: return EULER_SYM(escape);
	case SDLK_BACKSPACE: return EULER_SYM(backspace);
	case SDLK_TAB: return EULER_SYM(tab);
	case SDLK_SPACE: return EULER_SYM(space);
	case SDLK_EXCLAIM: return EULER_SYM(exclaim);
	case SDLK_DBLAPOSTROPHE: return EULER_SYM(dblapostrophe);
	case SDLK_HASH: return EULER_SYM(hash);
	case SDLK_DOLLAR: return EULER_SYM(dollar);
	case SDLK_PERCENT: return EULER_SYM(percent);
	case SDLK_AMPERSAND: return EULER_SYM(ampersand);
	case SDLK_APOSTROPHE: return EULER_SYM(apostrophe);
	case SDLK_LEFTPAREN: return EULER_SYM(leftparen);
	case SDLK_RIGHTPAREN: return EULER_SYM(rightparen);
	case SDLK_ASTERISK: return EULER_SYM(asterisk);
	case SDLK_PLUS: return EULER_SYM(plus);
	case SDLK_COMMA: return EULER_SYM(comma);
	case SDLK_MINUS: return EULER_SYM(minus);
	case SDLK_PERIOD: return EULER_SYM(period);
	case SDLK_SLASH: return EULER_SYM(slash);
	case SDLK_0: return EULER_SYM(zero);
	case SDLK_1: return EULER_SYM(one);
	case SDLK_2: return EULER_SYM(two);
	case SDLK_3: return EULER_SYM(three);
	case SDLK_4: return EULER_SYM(four);
	case SDLK_5: return EULER_SYM(five);
	case SDLK_6: return EULER_SYM(six);
	case SDLK_7: return EULER_SYM(seven);
	case SDLK_8: return EULER_SYM(eight);
	case SDLK_9: return EULER_SYM(nine);
	case SDLK_COLON: return EULER_SYM(colon);
	case SDLK_SEMICOLON: return EULER_SYM(semicolon);
	case SDLK_LESS: return EULER_SYM(less);
	case SDLK_EQUALS: return EULER_SYM(equals);
	case SDLK_GREATER: return EULER_SYM(greater);
	case SDLK_QUESTION: return EULER_SYM(question);
	case SDLK_AT: return EULER_SYM(at);
	case SDLK_LEFTBRACKET: return EULER_SYM(leftbracket);
	case SDLK_BACKSLASH: return EULER_SYM(backslash);
	case SDLK_RIGHTBRACKET: return EULER_SYM(rightbracket);
	case SDLK_CARET: return EULER_SYM(caret);
	case SDLK_UNDERSCORE: return EULER_SYM(underscore);
	case SDLK_GRAVE: return EULER_SYM(grave);
	case SDLK_A: return EULER_SYM(a);
	case SDLK_B: return EULER_SYM(b);
	case SDLK_C: return EULER_SYM(c);
	case SDLK_D: return EULER_SYM(d);
	case SDLK_E: return EULER_SYM(e);
	case SDLK_F: return EULER_SYM(f);
	case SDLK_G: return EULER_SYM(g);
	case SDLK_H: return EULER_SYM(h);
	case SDLK_I: return EULER_SYM(i);
	case SDLK_J: return EULER_SYM(j);
	case SDLK_K: return EULER_SYM(k);
	case SDLK_L: return EULER_SYM(l);
	case SDLK_M: return EULER_SYM(m);
	case SDLK_N: return EULER_SYM(n);
	case SDLK_O: return EULER_SYM(o);
	case SDLK_P: return EULER_SYM(p);
	case SDLK_Q: return EULER_SYM(q);
	case SDLK_R: return EULER_SYM(r);
	case SDLK_S: return EULER_SYM(s);
	case SDLK_T: return EULER_SYM(t);
	case SDLK_U: return EULER_SYM(u);
	case SDLK_V: return EULER_SYM(v);
	case SDLK_W: return EULER_SYM(w);
	case SDLK_X: return EULER_SYM(x);
	case SDLK_Y: return EULER_SYM(y);
	case SDLK_Z: return EULER_SYM(z);
	case SDLK_LEFTBRACE: return EULER_SYM(leftbrace);
	case SDLK_PIPE: return EULER_SYM(pipe);
	case SDLK_RIGHTBRACE: return EULER_SYM(rightbrace);
	case SDLK_TILDE: return EULER_SYM(tilde);
	case SDLK_DELETE: return EULER_SYM(delete);
	case SDLK_PLUSMINUS: return EULER_SYM(plusminus);
	case SDLK_CAPSLOCK: return EULER_SYM(capslock);
	case SDLK_F1: return EULER_SYM(f1);
	case SDLK_F2: return EULER_SYM(f2);
	case SDLK_F3: return EULER_SYM(f3);
	case SDLK_F4: return EULER_SYM(f4);
	case SDLK_F5: return EULER_SYM(f5);
	case SDLK_F6: return EULER_SYM(f6);
	case SDLK_F7: return EULER_SYM(f7);
	case SDLK_F8: return EULER_SYM(f8);
	case SDLK_F9: return EULER_SYM(f9);
	case SDLK_F10: return EULER_SYM(f10);
	case SDLK_F11: return EULER_SYM(f11);
	case SDLK_F12: return EULER_SYM(f12);
	case SDLK_PRINTSCREEN: return EULER_SYM(printscreen);
	case SDLK_SCROLLLOCK: return EULER_SYM(scrolllock);
	case SDLK_PAUSE: return EULER_SYM(pause);
	case SDLK_INSERT: return EULER_SYM(insert);
	case SDLK_HOME: return EULER_SYM(home);
	case SDLK_PAGEUP: return EULER_SYM(pageup);
	case SDLK_END: return EULER_SYM(end);
	case SDLK_PAGEDOWN: return EULER_SYM(pagedown);
	case SDLK_RIGHT: return EULER_SYM(right);
	case SDLK_LEFT: return EULER_SYM(left);
	case SDLK_DOWN: return EULER_SYM(down);
	case SDLK_UP: return EULER_SYM(up);
	case SDLK_NUMLOCKCLEAR: return EULER_SYM(numlockclear);
	case SDLK_KP_DIVIDE: return EULER_SYM(kp_divide);
	case SDLK_KP_MULTIPLY: return EULER_SYM(kp_multiply);
	case SDLK_KP_MINUS: return EULER_SYM(kp_minus);
	case SDLK_KP_PLUS: return EULER_SYM(kp_plus);
	case SDLK_KP_ENTER: return EULER_SYM(kp_enter);
	case SDLK_KP_1: return EULER_SYM(kp_1);
	case SDLK_KP_2: return EULER_SYM(kp_2);
	case SDLK_KP_3: return EULER_SYM(kp_3);
	case SDLK_KP_4: return EULER_SYM(kp_4);
	case SDLK_KP_5: return EULER_SYM(kp_5);
	case SDLK_KP_6: return EULER_SYM(kp_6);
	case SDLK_KP_7: return EULER_SYM(kp_7);
	case SDLK_KP_8: return EULER_SYM(kp_8);
	case SDLK_KP_9: return EULER_SYM(kp_9);
	case SDLK_KP_0: return EULER_SYM(kp_0);
	case SDLK_KP_PERIOD: return EULER_SYM(kp_period);
	case SDLK_APPLICATION: return EULER_SYM(application);
	case SDLK_POWER: return EULER_SYM(power);
	case SDLK_KP_EQUALS: return EULER_SYM(kp_equals);
	case SDLK_F13: return EULER_SYM(f13);
	case SDLK_F14: return EULER_SYM(f14);
	case SDLK_F15: return EULER_SYM(f15);
	case SDLK_F16: return EULER_SYM(f16);
	case SDLK_F17: return EULER_SYM(f17);
	case SDLK_F18: return EULER_SYM(f18);
	case SDLK_F19: return EULER_SYM(f19);
	case SDLK_F20: return EULER_SYM(f20);
	case SDLK_F21: return EULER_SYM(f21);
	case SDLK_F22: return EULER_SYM(f22);
	case SDLK_F23: return EULER_SYM(f23);
	case SDLK_F24: return EULER_SYM(f24);
	case SDLK_EXECUTE: return EULER_SYM(execute);
	case SDLK_HELP: return EULER_SYM(help);
	case SDLK_MENU: return EULER_SYM(menu);
	case SDLK_SELECT: return EULER_SYM(select);
	case SDLK_STOP: return EULER_SYM(stop);
	case SDLK_AGAIN: return EULER_SYM(again);
	case SDLK_UNDO: return EULER_SYM(undo);
	case SDLK_CUT: return EULER_SYM(cut);
	case SDLK_COPY: return EULER_SYM(copy);
	case SDLK_PASTE: return EULER_SYM(paste);
	case SDLK_FIND: return EULER_SYM(find);
	case SDLK_MUTE: return EULER_SYM(mute);
	case SDLK_VOLUMEUP: return EULER_SYM(volumeup);
	case SDLK_VOLUMEDOWN: return EULER_SYM(volumedown);
	case SDLK_KP_COMMA: return EULER_SYM(kp_comma);
	case SDLK_KP_EQUALSAS400: return EULER_SYM(kp_equalsas400);
	case SDLK_ALTERASE: return EULER_SYM(alterase);
	case SDLK_SYSREQ: return EULER_SYM(sysreq);
	case SDLK_CANCEL: return EULER_SYM(cancel);
	case SDLK_CLEAR: return EULER_SYM(clear);
	case SDLK_PRIOR: return EULER_SYM(prior);
	case SDLK_RETURN2: return EULER_SYM(return2);
	case SDLK_SEPARATOR: return EULER_SYM(separator);
	case SDLK_OUT: return EULER_SYM(out);
	case SDLK_OPER: return EULER_SYM(oper);
	case SDLK_CLEARAGAIN: return EULER_SYM(clearagain);
	case SDLK_CRSEL: return EULER_SYM(crsel);
	case SDLK_EXSEL: return EULER_SYM(exsel);
	case SDLK_KP_00: return EULER_SYM(kp_00);
	case SDLK_KP_000: return EULER_SYM(kp_000);
	case SDLK_THOUSANDSSEPARATOR: return EULER_SYM(thousandsseparator);
	case SDLK_DECIMALSEPARATOR: return EULER_SYM(decimalseparator);
	case SDLK_CURRENCYUNIT: return EULER_SYM(currencyunit);
	case SDLK_CURRENCYSUBUNIT: return EULER_SYM(currencysubunit);
	case SDLK_KP_LEFTPAREN: return EULER_SYM(kp_leftparen);
	case SDLK_KP_RIGHTPAREN: return EULER_SYM(kp_rightparen);
	case SDLK_KP_LEFTBRACE: return EULER_SYM(kp_leftbrace);
	case SDLK_KP_RIGHTBRACE: return EULER_SYM(kp_rightbrace);
	case SDLK_KP_TAB: return EULER_SYM(kp_tab);
	case SDLK_KP_BACKSPACE: return EULER_SYM(kp_backspace);
	case SDLK_KP_A: return EULER_SYM(kp_a);
	case SDLK_KP_B: return EULER_SYM(kp_b);
	case SDLK_KP_C: return EULER_SYM(kp_c);
	case SDLK_KP_D: return EULER_SYM(kp_d);
	case SDLK_KP_E: return EULER_SYM(kp_e);
	case SDLK_KP_F: return EULER_SYM(kp_f);
	case SDLK_KP_XOR: return EULER_SYM(kp_xor);
	case SDLK_KP_POWER: return EULER_SYM(kp_power);
	case SDLK_KP_PERCENT: return EULER_SYM(kp_percent);
	case SDLK_KP_LESS: return EULER_SYM(kp_less);
	case SDLK_KP_GREATER: return EULER_SYM(kp_greater);
	case SDLK_KP_AMPERSAND: return EULER_SYM(kp_ampersand);
	case SDLK_KP_DBLAMPERSAND: return EULER_SYM(kp_dblampersand);
	case SDLK_KP_VERTICALBAR: return EULER_SYM(kp_verticalbar);
	case SDLK_KP_DBLVERTICALBAR: return EULER_SYM(kp_dblverticalbar);
	case SDLK_KP_COLON: return EULER_SYM(kp_colon);
	case SDLK_KP_HASH: return EULER_SYM(kp_hash);
	case SDLK_KP_SPACE: return EULER_SYM(kp_space);
	case SDLK_KP_AT: return EULER_SYM(kp_at);
	case SDLK_KP_EXCLAM: return EULER_SYM(kp_exclam);
	case SDLK_KP_MEMSTORE: return EULER_SYM(kp_memstore);
	case SDLK_KP_MEMRECALL: return EULER_SYM(kp_memrecall);
	case SDLK_KP_MEMCLEAR: return EULER_SYM(kp_memclear);
	case SDLK_KP_MEMADD: return EULER_SYM(kp_memadd);
	case SDLK_KP_MEMSUBTRACT: return EULER_SYM(kp_memsubtract);
	case SDLK_KP_MEMMULTIPLY: return EULER_SYM(kp_memmultiply);
	case SDLK_KP_MEMDIVIDE: return EULER_SYM(kp_memdivide);
	case SDLK_KP_PLUSMINUS: return EULER_SYM(kp_plusminus);
	case SDLK_KP_CLEAR: return EULER_SYM(kp_clear);
	case SDLK_KP_CLEARENTRY: return EULER_SYM(kp_clearentry);
	case SDLK_KP_BINARY: return EULER_SYM(kp_binary);
	case SDLK_KP_OCTAL: return EULER_SYM(kp_octal);
	case SDLK_KP_DECIMAL: return EULER_SYM(kp_decimal);
	case SDLK_KP_HEXADECIMAL: return EULER_SYM(kp_hexadecimal);
	case SDLK_LCTRL: return EULER_SYM(lctrl);
	case SDLK_LSHIFT: return EULER_SYM(lshift);
	case SDLK_LALT: return EULER_SYM(lalt);
	case SDLK_LGUI: return EULER_SYM(lgui);
	case SDLK_RCTRL: return EULER_SYM(rctrl);
	case SDLK_RSHIFT: return EULER_SYM(rshift);
	case SDLK_RALT: return EULER_SYM(ralt);
	case SDLK_RGUI: return EULER_SYM(rgui);
	case SDLK_MODE: return EULER_SYM(mode);
	case SDLK_SLEEP: return EULER_SYM(sleep);
	case SDLK_WAKE: return EULER_SYM(wake);
	case SDLK_CHANNEL_INCREMENT: return EULER_SYM(channel_increment);
	case SDLK_CHANNEL_DECREMENT: return EULER_SYM(channel_decrement);
	case SDLK_MEDIA_PLAY: return EULER_SYM(media_play);
	case SDLK_MEDIA_PAUSE: return EULER_SYM(media_pause);
	case SDLK_MEDIA_RECORD: return EULER_SYM(media_record);
	case SDLK_MEDIA_FAST_FORWARD: return EULER_SYM(media_fast_forward);
	case SDLK_MEDIA_REWIND: return EULER_SYM(media_rewind);
	case SDLK_MEDIA_NEXT_TRACK: return EULER_SYM(media_next_track);
	case SDLK_MEDIA_PREVIOUS_TRACK: return EULER_SYM(media_previous_track);
	case SDLK_MEDIA_STOP: return EULER_SYM(media_stop);
	case SDLK_MEDIA_EJECT: return EULER_SYM(media_eject);
	case SDLK_MEDIA_PLAY_PAUSE: return EULER_SYM(media_play_pause);
	case SDLK_MEDIA_SELECT: return EULER_SYM(media_select);
	case SDLK_AC_NEW: return EULER_SYM(ac_new);
	case SDLK_AC_OPEN: return EULER_SYM(ac_open);
	case SDLK_AC_CLOSE: return EULER_SYM(ac_close);
	case SDLK_AC_EXIT: return EULER_SYM(ac_exit);
	case SDLK_AC_SAVE: return EULER_SYM(ac_save);
	case SDLK_AC_PRINT: return EULER_SYM(ac_print);
	case SDLK_AC_PROPERTIES: return EULER_SYM(ac_properties);
	case SDLK_AC_SEARCH: return EULER_SYM(ac_search);
	case SDLK_AC_HOME: return EULER_SYM(ac_home);
	case SDLK_AC_BACK: return EULER_SYM(ac_back);
	case SDLK_AC_FORWARD: return EULER_SYM(ac_forward);
	case SDLK_AC_STOP: return EULER_SYM(ac_stop);
	case SDLK_AC_REFRESH: return EULER_SYM(ac_refresh);
	case SDLK_AC_BOOKMARKS: return EULER_SYM(ac_bookmarks);
	case SDLK_SOFTLEFT: return EULER_SYM(softleft);
	case SDLK_SOFTRIGHT: return EULER_SYM(softright);
	case SDLK_CALL: return EULER_SYM(call);
	case SDLK_ENDCALL: return EULER_SYM(endcall);
	case SDLK_LEFT_TAB: return EULER_SYM(left_tab);
	case SDLK_LEVEL5_SHIFT: return EULER_SYM(level5_shift);
	case SDLK_MULTI_KEY_COMPOSE: return EULER_SYM(multi_key_compose);
	case SDLK_LMETA: return EULER_SYM(lmeta);
	case SDLK_RMETA: return EULER_SYM(rmeta);
	case SDLK_LHYPER: return EULER_SYM(lhyper);
	case SDLK_RHYPER: return EULER_SYM(rhyper);
	default: return EULER_SYM(unknown);
	}
}

static mrb_sym
sdl_kmod_sym(mrb_state *mrb, const SDL_Keymod code)
{
	switch (code) {
	case SDL_KMOD_LSHIFT: return EULER_SYM(lshift);
	case SDL_KMOD_RSHIFT: return EULER_SYM(rshift);
	case SDL_KMOD_LEVEL5: return EULER_SYM(level5);
	case SDL_KMOD_LCTRL: return EULER_SYM(lctrl);
	case SDL_KMOD_RCTRL: return EULER_SYM(rctrl);
	case SDL_KMOD_LALT: return EULER_SYM(lalt);
	case SDL_KMOD_RALT: return EULER_SYM(ralt);
	case SDL_KMOD_LGUI: return EULER_SYM(lgui);
	case SDL_KMOD_RGUI: return EULER_SYM(rgui);
	case SDL_KMOD_NUM: return EULER_SYM(num);
	case SDL_KMOD_CAPS: return EULER_SYM(caps);
	case SDL_KMOD_MODE: return EULER_SYM(mode);
	case SDL_KMOD_SCROLL: return EULER_SYM(scroll);
	default: return EULER_SYM(none);
	}
}

static mrb_sym
sdl_jhat_sym(mrb_state *mrb, const uint8_t hat)
{
	switch (hat) {
	case SDL_HAT_LEFTUP: return EULER_SYM(leftup);
	case SDL_HAT_UP: return EULER_SYM(up);
	case SDL_HAT_RIGHTUP: return EULER_SYM(rightup);
	case SDL_HAT_LEFT: return EULER_SYM(left);
	case SDL_HAT_RIGHT: return EULER_SYM(right);
	case SDL_HAT_LEFTDOWN: return EULER_SYM(leftdown);
	case SDL_HAT_DOWN: return EULER_SYM(down);
	case SDL_HAT_RIGHTDOWN: return EULER_SYM(rightdown);
	default: return EULER_SYM(centered);
	}
}

static mrb_sym
sdl_power_sym(mrb_state *mrb, const SDL_PowerState level)
{
	switch (level) {
	case SDL_POWERSTATE_ON_BATTERY: return EULER_SYM(on_battery);
	case SDL_POWERSTATE_NO_BATTERY: return EULER_SYM(no_battery);
	case SDL_POWERSTATE_CHARGING: return EULER_SYM(charging);
	case SDL_POWERSTATE_CHARGED: return EULER_SYM(charged);
	default: return EULER_SYM(unknown);
	}
}

static mrb_sym
sdl_gamepad_axis_sym(mrb_state *mrb, const SDL_GamepadAxis axis)
{
	switch (axis) {
	case SDL_GAMEPAD_AXIS_LEFTX: return EULER_SYM(leftx);
	case SDL_GAMEPAD_AXIS_LEFTY: return EULER_SYM(lefty);
	case SDL_GAMEPAD_AXIS_RIGHTX: return EULER_SYM(rightx);
	case SDL_GAMEPAD_AXIS_RIGHTY: return EULER_SYM(righty);
	case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: return EULER_SYM(left_trigger);
	case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return EULER_SYM(right_trigger);
	default: return EULER_SYM(invalid);
	}
}

static mrb_sym
sdl_gamepad_button_sym(mrb_state *mrb, const SDL_GamepadButton button)
{
	switch (button) {
	case SDL_GAMEPAD_BUTTON_SOUTH: return EULER_SYM(south);
	case SDL_GAMEPAD_BUTTON_EAST: return EULER_SYM(east);
	case SDL_GAMEPAD_BUTTON_WEST: return EULER_SYM(west);
	case SDL_GAMEPAD_BUTTON_NORTH: return EULER_SYM(north);
	case SDL_GAMEPAD_BUTTON_BACK: return EULER_SYM(back);
	case SDL_GAMEPAD_BUTTON_GUIDE: return EULER_SYM(guide);
	case SDL_GAMEPAD_BUTTON_START: return EULER_SYM(start);
	case SDL_GAMEPAD_BUTTON_LEFT_STICK: return EULER_SYM(left_stick);
	case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return EULER_SYM(right_stick);
	case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return EULER_SYM(left_shoulder);
	case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
		return EULER_SYM(right_shoulder);
	case SDL_GAMEPAD_BUTTON_DPAD_UP: return EULER_SYM(dpad_up);
	case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return EULER_SYM(dpad_down);
	case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return EULER_SYM(dpad_left);
	case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return EULER_SYM(dpad_right);
	case SDL_GAMEPAD_BUTTON_MISC1: return EULER_SYM(misc1);
	case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return EULER_SYM(right_paddle1);
	case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return EULER_SYM(left_paddle1);
	case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return EULER_SYM(right_paddle2);
	case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return EULER_SYM(left_paddle2);
	case SDL_GAMEPAD_BUTTON_TOUCHPAD: return EULER_SYM(touchpad);
	case SDL_GAMEPAD_BUTTON_MISC2: return EULER_SYM(misc2);
	case SDL_GAMEPAD_BUTTON_MISC3: return EULER_SYM(misc3);
	case SDL_GAMEPAD_BUTTON_MISC4: return EULER_SYM(misc4);
	case SDL_GAMEPAD_BUTTON_MISC5: return EULER_SYM(misc5);
	case SDL_GAMEPAD_BUTTON_MISC6: return EULER_SYM(misc6);
	default: return EULER_SYM(invalid);
	}
}

static mrb_sym
sdl_pen_input_sym(mrb_state *mrb, const SDL_PenInputFlags flags)
{
	switch (flags) {
	case SDL_PEN_INPUT_DOWN: return EULER_SYM(down);
	case SDL_PEN_INPUT_BUTTON_1: return EULER_SYM(button_1);
	case SDL_PEN_INPUT_BUTTON_2: return EULER_SYM(button_2);
	case SDL_PEN_INPUT_BUTTON_3: return EULER_SYM(button_3);
	case SDL_PEN_INPUT_BUTTON_4: return EULER_SYM(button_4);
	case SDL_PEN_INPUT_BUTTON_5: return EULER_SYM(button_5);
	case SDL_PEN_INPUT_ERASER_TIP: return EULER_SYM(eraser_tip);
	default: return EULER_SYM(invalid);
	}
}

static mrb_sym
sdl_pen_axis_sym(mrb_state *mrb, const SDL_PenAxis axis)
{
	switch (axis) {
	case SDL_PEN_AXIS_PRESSURE: return EULER_SYM(pressure);
	case SDL_PEN_AXIS_XTILT: return EULER_SYM(xtilt);
	case SDL_PEN_AXIS_YTILT: return EULER_SYM(ytilt);
	case SDL_PEN_AXIS_DISTANCE: return EULER_SYM(distance);
	case SDL_PEN_AXIS_ROTATION: return EULER_SYM(rotation);
	case SDL_PEN_AXIS_SLIDER: return EULER_SYM(slider);
	case SDL_PEN_AXIS_TANGENTIAL_PRESSURE:
		return EULER_SYM(tangential_pressure);
	default: return EULER_SYM(invalid);
	}
}

mrb_value
euler::event::sdl_event_to_mrb(
    const euler::util::Reference<euler::util::State> &state,
    const SDL_Event &event)
{
	auto mrb = state->mrb()->mrb();
	mrb_value value = mrb_nil_value();
	auto &mod = state->modules().event;
	auto arena_index = mrb_gc_arena_save(mrb);
	const auto type = sdl_event_sym(mrb, event);
	const auto type_val = mrb_symbol_value(type);

#define SET_INT_IV(ATTR, FIELD)                                                \
	do {                                                                   \
		const auto sym = EULER_IVSYM(ATTR);                            \
		const auto iv = state->mrb()->int_value(                       \
		    static_cast<mrb_int>(event.FIELD));                        \
		state->mrb()->iv_set(value, sym, iv);                          \
	} while (0)

#define SET_BOOL_IV(ATTR, FIELD)                                               \
	do {                                                                   \
		const auto sym = EULER_IVSYM(ATTR);                            \
		const auto iv                                                  \
		    = mrb_bool_value(static_cast<bool>(event.FIELD));          \
		state->mrb()->iv_set(value, sym, iv);                          \
	} while (0)

#define SET_CSTRING_IV(ATTR, FIELD)                                            \
	do {                                                                   \
		const auto sym = EULER_IVSYM(ATTR);                            \
		const auto iv = state->mrb()->str_new_cstr(event.FIELD);       \
		state->mrb()->iv_set(value, sym, iv);                          \
	} while (0)

#define SET_SYM_IV(ATTR, VALUE)                                                \
	do {                                                                   \
		const auto sym = EULER_IVSYM(ATTR);                            \
		const auto val = (VALUE);                                      \
		const auto sym_val = mrb_symbol_value(val);                    \
		state->mrb()->iv_set(value, sym, sym_val);                     \
	} while (0)

#define SET_FLOAT_IV(ATTR, FIELD)                                              \
	do {                                                                   \
		const auto sym = EULER_IVSYM(ATTR);                            \
		const auto iv = state->mrb()->float_value(                     \
		    static_cast<mrb_float>(event.FIELD));                      \
		state->mrb()->iv_set(value, sym, iv);                          \
	} while (0)

#define COMMON_INIT(SYM)                                                       \
	do {                                                                   \
		value = state->mrb()->obj_new(mod.SYM, 0, nullptr);            \
		assert(!mrb_nil_p(value));                                     \
		SET_INT_IV(timestamp, common.timestamp);                       \
		state->mrb()->iv_set(value, EULER_IVSYM(type), type_val);      \
	} while (0)

	switch (static_cast<SDL_EventType>(event.type)) {
	case SDL_EVENT_QUIT: COMMON_INIT(quit); break;
	case SDL_EVENT_DISPLAY_ORIENTATION:
	case SDL_EVENT_DISPLAY_ADDED:
	case SDL_EVENT_DISPLAY_REMOVED:
	case SDL_EVENT_DISPLAY_MOVED:
	case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
	case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED: [[fallthrough]];
	case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
		COMMON_INIT(display);
		SET_INT_IV(display_id, display.displayID);
		if (event.type == SDL_EVENT_DISPLAY_ORIENTATION) {
			SET_INT_IV(orientation, display.data1);
		}
		break;
	case SDL_EVENT_WINDOW_SHOWN:
	case SDL_EVENT_WINDOW_HIDDEN:
	case SDL_EVENT_WINDOW_EXPOSED:
	case SDL_EVENT_WINDOW_MOVED:
	case SDL_EVENT_WINDOW_RESIZED:
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
	case SDL_EVENT_WINDOW_MINIMIZED:
	case SDL_EVENT_WINDOW_MAXIMIZED:
	case SDL_EVENT_WINDOW_RESTORED:
	case SDL_EVENT_WINDOW_MOUSE_ENTER:
	case SDL_EVENT_WINDOW_MOUSE_LEAVE:
	case SDL_EVENT_WINDOW_FOCUS_GAINED:
	case SDL_EVENT_WINDOW_FOCUS_LOST:
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	case SDL_EVENT_WINDOW_HIT_TEST:
	case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
	case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
	case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
	case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
	case SDL_EVENT_WINDOW_OCCLUDED:
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
	case SDL_EVENT_WINDOW_DESTROYED: [[fallthrough]];
	case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
		COMMON_INIT(window);
		SET_INT_IV(window_id, window.windowID);
		switch (event.type) {
		case SDL_EVENT_WINDOW_MOVED:
			SET_INT_IV(x, window.data1);
			SET_INT_IV(y, window.data2);
			break;
		case SDL_EVENT_WINDOW_RESIZED:
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			SET_INT_IV(width, window.data1);
			SET_INT_IV(height, window.data2);
			break;
		case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
			SET_INT_IV(display_id, window.data1);
			break;
		default: break;
		}
		break;
	case SDL_EVENT_KEY_DOWN: [[fallthrough]];
	case SDL_EVENT_KEY_UP:
		COMMON_INIT(keyboard);
		SET_BOOL_IV(down, key.down);
		SET_BOOL_IV(repeat, key.repeat);
		SET_INT_IV(window_id, key.windowID);
		SET_INT_IV(which, key.which);
		SET_INT_IV(raw, key.raw);
		SET_SYM_IV(scancode, sdl_scancode_sym(mrb, event.key.scancode));
		SET_SYM_IV(key, sdl_key_sym(mrb, event.key.key));
		SET_SYM_IV(mod, sdl_kmod_sym(mrb, event.key.mod));
		break;
	case SDL_EVENT_TEXT_INPUT:
		COMMON_INIT(text_input);
		SET_INT_IV(window_id, text.windowID);
		SET_CSTRING_IV(text, text.text);
		break;
	case SDL_EVENT_KEYMAP_CHANGED:
	case SDL_EVENT_KEYBOARD_ADDED: [[fallthrough]];
	case SDL_EVENT_KEYBOARD_REMOVED:
		COMMON_INIT(keyboard_device);
		SET_INT_IV(which, kdevice.which);
		break;
	case SDL_EVENT_TEXT_EDITING:
		COMMON_INIT(text_editing);
		SET_CSTRING_IV(text, edit.text);
		SET_INT_IV(window_id, edit.windowID);
		SET_INT_IV(start, edit.start);
		SET_INT_IV(length, edit.length);
		break;
	case SDL_EVENT_TEXT_EDITING_CANDIDATES: {
		COMMON_INIT(text_editing_candidates);
		SET_INT_IV(window_id, edit_candidates.windowID);
		SET_INT_IV(selected_candidate,
		    edit_candidates.selected_candidate);
		SET_BOOL_IV(horizontal, edit_candidates.horizontal);
		const auto ary = state->mrb()->ary_new_capa(
		    event.edit_candidates.num_candidates);
		for (int i = 0; i < event.edit_candidates.num_candidates; ++i) {
			const auto candidate = state->mrb()->str_new_cstr(
			    event.edit_candidates.candidates[i]);
			state->mrb()->ary_push(ary, candidate);
		}
		state->mrb()->iv_set(value, EULER_SYM(candidates), ary);
		break;
	}
	case SDL_EVENT_MOUSE_MOTION: {
		COMMON_INIT(mouse_motion);
		SET_INT_IV(window_id, motion.windowID);
		SET_INT_IV(which, motion.which);
		const auto state_ary = state->mrb()->ary_new_capa(5);
		const auto buttons = event.motion.state;
		if (buttons & SDL_BUTTON_LMASK) {
			state->mrb()->ary_push(state_ary,
			    mrb_symbol_value(EULER_SYM(left)));
		}
		if (buttons & SDL_BUTTON_MMASK) {
			state->mrb()->ary_push(state_ary,
			    mrb_symbol_value(EULER_SYM(middle)));
		}
		if (buttons & SDL_BUTTON_RMASK) {
			state->mrb()->ary_push(state_ary,
			    mrb_symbol_value(EULER_SYM(right)));
		}
		if (buttons & SDL_BUTTON_X1MASK) {
			state->mrb()->ary_push(state_ary,
			    mrb_symbol_value(EULER_SYM(x1)));
		}
		if (buttons & SDL_BUTTON_X2MASK) {
			state->mrb()->ary_push(state_ary,
			    mrb_symbol_value(EULER_SYM(x2)));
		}
		state->mrb()->iv_set(value, EULER_SYM(state), state_ary);
		SET_FLOAT_IV(x, motion.x);
		SET_FLOAT_IV(y, motion.y);
		SET_FLOAT_IV(xrel, motion.xrel);
		SET_FLOAT_IV(yrel, motion.yrel);
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_MOUSE_BUTTON_UP:
		COMMON_INIT(mouse_button);
		SET_INT_IV(window_id, button.windowID);
		SET_INT_IV(which, button.which);
		SET_INT_IV(button, button.button);
		SET_BOOL_IV(pressed, button.down);
		SET_BOOL_IV(clicks, button.clicks);
		SET_FLOAT_IV(x, button.x);
		SET_FLOAT_IV(y, button.y);
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		COMMON_INIT(mouse_wheel);
		SET_INT_IV(window_id, wheel.windowID);
		SET_INT_IV(which, wheel.which);
		SET_FLOAT_IV(x, wheel.x);
		SET_FLOAT_IV(y, wheel.y);
		SET_SYM_IV(direction,
		    event.wheel.direction == SDL_MOUSEWHEEL_NORMAL
			? EULER_SYM(normal)
			: EULER_SYM(flipped));
		SET_FLOAT_IV(mouse_x, wheel.mouse_x);
		SET_FLOAT_IV(mouse_y, wheel.mouse_y);
		break;
	case SDL_EVENT_MOUSE_ADDED: [[fallthrough]];
	case SDL_EVENT_MOUSE_REMOVED:
		COMMON_INIT(mouse_device);
		SET_INT_IV(which, mdevice.which);
		break;
	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		COMMON_INIT(joystick_axis_motion);
		SET_INT_IV(which, jaxis.which);
		SET_INT_IV(axis, jaxis.axis);
		SET_FLOAT_IV(value, jaxis.value / 32768.0f);
		break;
	case SDL_EVENT_JOYSTICK_BALL_MOTION:
		COMMON_INIT(joystick_ball_motion);
		SET_INT_IV(which, jball.which);
		SET_INT_IV(ball, jball.ball);
		SET_FLOAT_IV(xrel, jball.xrel / 32768.0f);
		SET_FLOAT_IV(yrel, jball.yrel / 32768.0f);
		break;
	case SDL_EVENT_JOYSTICK_HAT_MOTION:
		COMMON_INIT(joystick_hat_motion);
		SET_INT_IV(which, jhat.which);
		SET_INT_IV(hat, jhat.hat);
		SET_SYM_IV(value, sdl_jhat_sym(mrb, event.jhat.value));
		break;
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_JOYSTICK_BUTTON_UP:
		COMMON_INIT(joystick_button);
		SET_INT_IV(which, jbutton.which);
		SET_INT_IV(button, jbutton.button);
		SET_BOOL_IV(down, jbutton.down);
		break;
	case SDL_EVENT_JOYSTICK_ADDED:
	case SDL_EVENT_JOYSTICK_REMOVED: [[fallthrough]];
	case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
		COMMON_INIT(joystick_device);
		SET_INT_IV(which, jdevice.which);
		break;
	case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		COMMON_INIT(joystick_battery_updated);
		SET_INT_IV(which, jbattery.which);
		SET_SYM_IV(state, sdl_power_sym(mrb, event.jbattery.state));
		SET_INT_IV(percent, jbattery.percent);
		break;
	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		COMMON_INIT(gamepad_axis_motion);
		SET_INT_IV(which, gaxis.which);
		SET_SYM_IV(axis,
		    sdl_gamepad_axis_sym(mrb,
			static_cast<SDL_GamepadAxis>(event.gaxis.axis)));
		SET_FLOAT_IV(value, gaxis.value / 32768.0f);
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
		COMMON_INIT(gamepad_button);
		SET_INT_IV(which, gbutton.which);
		SET_SYM_IV(button,
		    sdl_gamepad_button_sym(mrb,
			static_cast<SDL_GamepadButton>(event.gbutton.button)));
		SET_BOOL_IV(down, gbutton.down);
		break;
	case SDL_EVENT_GAMEPAD_ADDED:
	case SDL_EVENT_GAMEPAD_REMOVED:
	case SDL_EVENT_GAMEPAD_REMAPPED:
	case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
		COMMON_INIT(gamepad_device);
		SET_INT_IV(which, gdevice.which);
		break;
	case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
	case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: [[fallthrough]];
	case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
		COMMON_INIT(gamepad_touchpad);
		SET_INT_IV(which, gtouchpad.which);
		SET_INT_IV(touchpad, gtouchpad.touchpad);
		SET_INT_IV(finger, gtouchpad.finger);
		SET_FLOAT_IV(x, gtouchpad.x);
		SET_FLOAT_IV(y, gtouchpad.y);
		SET_FLOAT_IV(pressure, gtouchpad.pressure);
		break;
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
		COMMON_INIT(gamepad_sensor);
		SET_INT_IV(which, gsensor.which);
		SET_INT_IV(sensor, gsensor.sensor);
		auto data_ary = state->mrb()->ary_new_capa(3);
		const auto data = event.gsensor.data;
		for (int i = 0; i < 3; ++i) {
			const auto val = state->mrb()->float_value(data[i]);
			state->mrb()->ary_push(data_ary, val);
		}
		state->mrb()->iv_set(value, EULER_SYM(data), data_ary);
		break;
	}
	case SDL_EVENT_FINGER_DOWN:
	case SDL_EVENT_FINGER_UP:
	case SDL_EVENT_FINGER_MOTION: [[fallthrough]];
	case SDL_EVENT_FINGER_CANCELED:
		COMMON_INIT(touch_finger);
		SET_INT_IV(touch_id, tfinger.touchID);
		SET_INT_IV(finger_id, tfinger.fingerID);
		SET_FLOAT_IV(x, tfinger.x);
		SET_FLOAT_IV(y, tfinger.y);
		SET_FLOAT_IV(dx, tfinger.dx);
		SET_FLOAT_IV(dy, tfinger.dy);
		SET_FLOAT_IV(pressure, tfinger.pressure);
		SET_INT_IV(window_id, tfinger.windowID);
		break;
	case SDL_EVENT_CLIPBOARD_UPDATE: {
		COMMON_INIT(clipboard);
		SET_BOOL_IV(owner, clipboard.owner);
		const auto n = event.clipboard.num_mime_types;
		auto ary = state->mrb()->ary_new_capa(n);
		for (int i = 0; i < n; ++i) {
			const auto mime = state->mrb()->str_new_cstr(
			    event.clipboard.mime_types[i]);
			state->mrb()->ary_push(ary, mime);
		}
		state->mrb()->iv_set(value, EULER_SYM(mime_types), ary);
		break;
	}
	case SDL_EVENT_DROP_FILE:
	case SDL_EVENT_DROP_TEXT:
	case SDL_EVENT_DROP_BEGIN:
	case SDL_EVENT_DROP_COMPLETE: [[fallthrough]];
	case SDL_EVENT_DROP_POSITION:
		COMMON_INIT(drop);
		SET_INT_IV(window_id, drop.windowID);
		SET_FLOAT_IV(x, drop.x);
		SET_FLOAT_IV(y, drop.y);
		SET_CSTRING_IV(source, drop.source);
		SET_CSTRING_IV(data, drop.data);
		break;
	case SDL_EVENT_AUDIO_DEVICE_ADDED:
	case SDL_EVENT_AUDIO_DEVICE_REMOVED: [[fallthrough]];
	case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
		COMMON_INIT(audio_device);
		SET_INT_IV(which, adevice.which);
		SET_BOOL_IV(recording, adevice.recording);
		break;
	case SDL_EVENT_SENSOR_UPDATE: {
		COMMON_INIT(sensor);
		SET_INT_IV(which, sensor.which);
		SET_INT_IV(sensor_timestamp, sensor.sensor_timestamp);
		auto ary = state->mrb()->ary_new_capa(6);
		const auto data = event.sensor.data;
		for (int i = 0; i < 6; ++i) {
			const auto val = state->mrb()->float_value(data[i]);
			state->mrb()->ary_push(ary, val);
		}
		state->mrb()->iv_set(value, EULER_SYM(data), ary);
		break;
	}
	case SDL_EVENT_PEN_PROXIMITY_IN: [[fallthrough]];
	case SDL_EVENT_PEN_PROXIMITY_OUT:
		COMMON_INIT(pen_proximity);
		SET_INT_IV(window_id, pproximity.windowID);
		SET_INT_IV(which, pproximity.which);
		break;
	case SDL_EVENT_PEN_DOWN: [[fallthrough]];
	case SDL_EVENT_PEN_UP:
		COMMON_INIT(pen_touch);
		SET_INT_IV(window_id, ptouch.windowID);
		SET_INT_IV(which, ptouch.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(mrb, event.ptouch.pen_state));
		SET_FLOAT_IV(x, ptouch.x);
		SET_FLOAT_IV(y, ptouch.y);
		SET_BOOL_IV(eraser, ptouch.eraser);
		SET_BOOL_IV(down, ptouch.down);
		break;
	case SDL_EVENT_PEN_BUTTON_DOWN: [[fallthrough]];
	case SDL_EVENT_PEN_BUTTON_UP:
		COMMON_INIT(pen_button);
		SET_INT_IV(window_id, pbutton.windowID);
		SET_INT_IV(which, pbutton.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(mrb, event.pbutton.pen_state));
		SET_INT_IV(button, pbutton.button);
		SET_BOOL_IV(down, pbutton.down);
		break;
	case SDL_EVENT_PEN_MOTION:
		COMMON_INIT(pen_motion);
		SET_INT_IV(window_id, pmotion.windowID);
		SET_INT_IV(which, pmotion.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(mrb, event.pmotion.pen_state));
		SET_FLOAT_IV(x, pmotion.x);
		SET_FLOAT_IV(y, pmotion.y);
		break;
	case SDL_EVENT_PEN_AXIS:
		COMMON_INIT(pen_axis);
		SET_INT_IV(window_id, paxis.windowID);
		SET_INT_IV(which, paxis.which);
		SET_SYM_IV(pen_state,
		    sdl_pen_input_sym(mrb, event.paxis.pen_state));
		SET_FLOAT_IV(x, paxis.x);
		SET_FLOAT_IV(y, paxis.y);
		SET_SYM_IV(axis, sdl_pen_axis_sym(mrb, event.paxis.axis));
		SET_FLOAT_IV(value, paxis.value);
		break;
	case SDL_EVENT_CAMERA_DEVICE_ADDED:
	case SDL_EVENT_CAMERA_DEVICE_REMOVED:
	case SDL_EVENT_CAMERA_DEVICE_APPROVED: [[fallthrough]];
	case SDL_EVENT_CAMERA_DEVICE_DENIED:
		COMMON_INIT(camera_device);
		SET_INT_IV(which, cdevice.which);
		break;
	case SDL_EVENT_RENDER_TARGETS_RESET:
	case SDL_EVENT_RENDER_DEVICE_RESET: [[fallthrough]];
	case SDL_EVENT_RENDER_DEVICE_LOST:
		COMMON_INIT(render);
		SET_INT_IV(window_id, render.windowID);
		break;
	default: {
		mrb_gc_arena_restore(mrb, arena_index);
		const auto error
		    = std::format("Unexpected event type {}", event.type);
		throw std::runtime_error(error);
	}
	}
#undef COMMON_INIT
#undef SET_INT_IV
#undef SET_BOOL_IV
#undef SET_CSTRING_IV
#undef SET_SYM_IV
#undef SET_FLOAT_IV

	return value;
}

// void
// euler::event::init(const util::Reference<util::State> &state)
RClass *
euler::event::init(const util::Reference<util::State> &state, RClass *mod,
    RClass *)
{
	const auto mrb = state->mrb()->mrb();
	auto &event = state->modules().event;
	event.mod = state->mrb()->define_module_under(mod, "Event");

	event.base = state->mrb()->define_class_under(event.mod, "Base",
	    state->object_class());

#define DEFINE_ATTR_READER(TYPE, ATTR)                                         \
	do {                                                                   \
		state->mrb()->define_method_id((event.TYPE), EULER_SYM(ATTR),  \
		    [](mrb_state *mrb, const mrb_value self) {                 \
			    auto state = (::euler::util::State::get(mrb));     \
			    return state->mrb()->iv_get(self,                  \
				EULER_IVSYM(SYM));                             \
		    },                                                         \
		    0);                                                        \
	} while (0)

#define DEFINE_EVENT(TYPE, NAME)                                               \
	do {                                                                   \
		(event.TYPE) = state->mrb()->define_class_under(event.mod,     \
		    (#NAME), event.base);                                      \
		DEFINE_ATTR_READER(TYPE, timestamp);                           \
		DEFINE_ATTR_READER(TYPE, type);                                \
	} while (0)

	DEFINE_EVENT(quit, Quit);

	DEFINE_EVENT(display, Display);
	DEFINE_ATTR_READER(display, display_id);
	DEFINE_ATTR_READER(display, orientation);

	DEFINE_EVENT(window, Window);
	DEFINE_ATTR_READER(window, window_id);
	DEFINE_ATTR_READER(window, x);
	DEFINE_ATTR_READER(window, y);
	DEFINE_ATTR_READER(window, width);
	DEFINE_ATTR_READER(window, height);
	DEFINE_ATTR_READER(window, display_id);

	DEFINE_EVENT(keyboard, Keyboard);
	DEFINE_ATTR_READER(keyboard, down);
	DEFINE_ATTR_READER(keyboard, repeat);
	DEFINE_ATTR_READER(keyboard, window_id);
	DEFINE_ATTR_READER(keyboard, which);
	DEFINE_ATTR_READER(keyboard, raw);
	DEFINE_ATTR_READER(keyboard, scancode);
	DEFINE_ATTR_READER(keyboard, key);
	DEFINE_ATTR_READER(keyboard, mod);

	DEFINE_EVENT(text_input, TextInput);
	DEFINE_ATTR_READER(text_input, window_id);
	DEFINE_ATTR_READER(text_input, text);

	DEFINE_EVENT(keyboard_device, KeyboardDevice);
	DEFINE_ATTR_READER(keyboard_device, which);

	DEFINE_EVENT(text_editing, TextEditing);
	DEFINE_ATTR_READER(text_editing, text);
	DEFINE_ATTR_READER(text_editing, window_id);
	DEFINE_ATTR_READER(text_editing, start);
	DEFINE_ATTR_READER(text_editing, length);

	DEFINE_EVENT(text_editing_candidates, TextEditingCandidates);
	DEFINE_ATTR_READER(text_editing_candidates, window_id);
	DEFINE_ATTR_READER(text_editing_candidates, selected_candidate);
	DEFINE_ATTR_READER(text_editing_candidates, horizontal);
	DEFINE_ATTR_READER(text_editing_candidates, candidates);

	DEFINE_EVENT(mouse_motion, MouseMotion);
	DEFINE_ATTR_READER(mouse_motion, window_id);
	DEFINE_ATTR_READER(mouse_motion, which);
	DEFINE_ATTR_READER(mouse_motion, state);
	DEFINE_ATTR_READER(mouse_motion, x);
	DEFINE_ATTR_READER(mouse_motion, y);
	DEFINE_ATTR_READER(mouse_motion, xrel);
	DEFINE_ATTR_READER(mouse_motion, yrel);

	DEFINE_EVENT(mouse_button, MouseButton);
	DEFINE_ATTR_READER(mouse_button, window_id);
	DEFINE_ATTR_READER(mouse_button, which);
	DEFINE_ATTR_READER(mouse_button, button);
	DEFINE_ATTR_READER(mouse_button, pressed);
	DEFINE_ATTR_READER(mouse_button, clicks);
	DEFINE_ATTR_READER(mouse_button, x);
	DEFINE_ATTR_READER(mouse_button, y);

	DEFINE_EVENT(mouse_wheel, MouseWheel);
	DEFINE_ATTR_READER(mouse_wheel, window_id);
	DEFINE_ATTR_READER(mouse_wheel, which);
	DEFINE_ATTR_READER(mouse_wheel, x);
	DEFINE_ATTR_READER(mouse_wheel, y);
	DEFINE_ATTR_READER(mouse_wheel, direction);
	DEFINE_ATTR_READER(mouse_wheel, mouse_x);
	DEFINE_ATTR_READER(mouse_wheel, mouse_y);

	DEFINE_EVENT(mouse_device, MouseDevice);
	DEFINE_ATTR_READER(mouse_device, which);

	DEFINE_EVENT(joystick_axis_motion, JoystickAxisMotion);
	DEFINE_ATTR_READER(joystick_axis_motion, which);
	DEFINE_ATTR_READER(joystick_axis_motion, axis);
	DEFINE_ATTR_READER(joystick_axis_motion, value);

	DEFINE_EVENT(joystick_ball_motion, JoystickBallMotion);
	DEFINE_ATTR_READER(joystick_ball_motion, which);
	DEFINE_ATTR_READER(joystick_ball_motion, ball);
	DEFINE_ATTR_READER(joystick_ball_motion, xrel);
	DEFINE_ATTR_READER(joystick_ball_motion, yrel);

	DEFINE_EVENT(joystick_hat_motion, JoystickHatMotion);
	DEFINE_ATTR_READER(joystick_hat_motion, which);
	DEFINE_ATTR_READER(joystick_hat_motion, hat);
	DEFINE_ATTR_READER(joystick_hat_motion, value);

	DEFINE_EVENT(joystick_button, JoystickButton);
	DEFINE_ATTR_READER(joystick_button, which);
	DEFINE_ATTR_READER(joystick_button, button);
	DEFINE_ATTR_READER(joystick_button, down);

	DEFINE_EVENT(joystick_device, JoystickDevice);
	DEFINE_ATTR_READER(joystick_device, which);

	DEFINE_EVENT(joystick_battery_updated, JoystickBatteryEvent);
	DEFINE_ATTR_READER(joystick_battery_updated, which);
	DEFINE_ATTR_READER(joystick_battery_updated, state);

	DEFINE_EVENT(gamepad_axis_motion, GamepadAxisMotion);
	DEFINE_ATTR_READER(gamepad_axis_motion, which);
	DEFINE_ATTR_READER(gamepad_axis_motion, axis);
	DEFINE_ATTR_READER(gamepad_axis_motion, value);

	DEFINE_EVENT(gamepad_button, GamepadButton);
	DEFINE_ATTR_READER(gamepad_button, which);
	DEFINE_ATTR_READER(gamepad_button, button);
	DEFINE_ATTR_READER(gamepad_button, down);

	DEFINE_EVENT(gamepad_device, GamepadDevice);
	DEFINE_ATTR_READER(gamepad_device, which);

	DEFINE_EVENT(gamepad_touchpad, GamepadTouchpad);
	DEFINE_ATTR_READER(gamepad_touchpad, which);
	DEFINE_ATTR_READER(gamepad_touchpad, touchpad);
	DEFINE_ATTR_READER(gamepad_touchpad, finger);
	DEFINE_ATTR_READER(gamepad_touchpad, x);
	DEFINE_ATTR_READER(gamepad_touchpad, y);
	DEFINE_ATTR_READER(gamepad_touchpad, pressure);

	DEFINE_EVENT(gamepad_sensor, GamepadSensor);
	DEFINE_ATTR_READER(gamepad_sensor, which);
	DEFINE_ATTR_READER(gamepad_sensor, sensor);
	DEFINE_ATTR_READER(gamepad_sensor, data);

	DEFINE_EVENT(touch_finger, TouchFinger);
	DEFINE_ATTR_READER(touch_finger, touch_id);
	DEFINE_ATTR_READER(touch_finger, finger_id);
	DEFINE_ATTR_READER(touch_finger, x);
	DEFINE_ATTR_READER(touch_finger, y);
	DEFINE_ATTR_READER(touch_finger, dx);
	DEFINE_ATTR_READER(touch_finger, dy);
	DEFINE_ATTR_READER(touch_finger, pressure);
	DEFINE_ATTR_READER(touch_finger, window_id);

	DEFINE_EVENT(clipboard, Clipboard);
	DEFINE_ATTR_READER(clipboard, owner);
	DEFINE_ATTR_READER(clipboard, mime_types);

	DEFINE_EVENT(drop, Drop);
	DEFINE_ATTR_READER(drop, window_id);
	DEFINE_ATTR_READER(drop, x);
	DEFINE_ATTR_READER(drop, y);
	DEFINE_ATTR_READER(drop, source);
	DEFINE_ATTR_READER(drop, data);

	DEFINE_EVENT(audio_device, AudioDevice);
	DEFINE_ATTR_READER(audio_device, which);
	DEFINE_ATTR_READER(audio_device, recording);

	DEFINE_EVENT(sensor, Sensor);
	DEFINE_ATTR_READER(sensor, which);
	DEFINE_ATTR_READER(sensor, sensor_timestamp);
	DEFINE_ATTR_READER(sensor, data);

	DEFINE_EVENT(pen_proximity, PenProximity);
	DEFINE_ATTR_READER(pen_proximity, window_id);
	DEFINE_ATTR_READER(pen_proximity, which);

	DEFINE_EVENT(pen_touch, PenTouch);
	DEFINE_ATTR_READER(pen_touch, window_id);
	DEFINE_ATTR_READER(pen_touch, which);
	DEFINE_ATTR_READER(pen_touch, pen_state);
	DEFINE_ATTR_READER(pen_touch, x);
	DEFINE_ATTR_READER(pen_touch, y);
	DEFINE_ATTR_READER(pen_touch, eraser);
	DEFINE_ATTR_READER(pen_touch, down);

	DEFINE_EVENT(pen_button, PenButton);
	DEFINE_ATTR_READER(pen_button, window_id);
	DEFINE_ATTR_READER(pen_button, which);
	DEFINE_ATTR_READER(pen_button, pen_state);
	DEFINE_ATTR_READER(pen_button, button);
	DEFINE_ATTR_READER(pen_button, down);

	DEFINE_EVENT(pen_motion, PenMotion);
	DEFINE_ATTR_READER(pen_motion, window_id);
	DEFINE_ATTR_READER(pen_motion, which);
	DEFINE_ATTR_READER(pen_motion, pen_state);
	DEFINE_ATTR_READER(pen_motion, x);
	DEFINE_ATTR_READER(pen_motion, y);

	DEFINE_EVENT(pen_axis, PenAxis);
	DEFINE_ATTR_READER(pen_axis, window_id);
	DEFINE_ATTR_READER(pen_axis, which);
	DEFINE_ATTR_READER(pen_axis, pen_state);
	DEFINE_ATTR_READER(pen_axis, x);
	DEFINE_ATTR_READER(pen_axis, y);
	DEFINE_ATTR_READER(pen_axis, axis);
	DEFINE_ATTR_READER(pen_axis, value);

	DEFINE_EVENT(camera_device, CameraDevice);
	DEFINE_ATTR_READER(camera_device, which);

	DEFINE_EVENT(render, Render);
	DEFINE_ATTR_READER(render, window_id);
#undef DEFINE_ATTR_READER
#undef DEFINE_EVENT
	return event.mod;
}
