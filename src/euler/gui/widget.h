/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_WIDGET_H
#define EULER_GUI_WIDGET_H

#include <functional>
#include <string>

#include "euler/gui/row.h"
#include "euler/util/object.h"

namespace euler::gui {
class Window;

/* Equivalent to nk_window */
class Widget final : public util::Object {
	BIND_MRUBY("Euler::Gui::Widget", Widget, gui.widget);
public:
	using ID = uint64_t;
	~Widget() override;

	void release();

	struct Flags {
		bool border : 1 = false;
		bool moveable : 1 = false;
		bool scalable : 1 = false;
		bool closeable : 1 = false;
		bool minimizable : 1 = false;
		bool no_scrollbar : 1 = false;
		bool title : 1 = false;
		bool scroll_auto_hide : 1 = false;
		bool background : 1 = false;
		bool scale_left : 1 = false;
		bool no_input : 1 = false;
	};

	struct Rectangle {
		float x = 0;
		float y = 0;
		float w = 100;
		float h = 100;
	};

	struct Settings {
		Rectangle rect;
		Flags flags;
	};

	static Rectangle read_widget_rect(mrb_state *mrb, mrb_value hash);
	static Flags read_widget_flags(mrb_state *mrb, const mrb_value arr);
	static const Settings DEFAULT_SETTINGS;

	Widget(const util::Reference<Window> &gui, ID id,
	    const char *title = "Widget",
	    const Settings &settings = DEFAULT_SETTINGS);

	void row(Row::Settings settings,
	    const std::function<void(util::Reference<Row> &)> &fn);

	void
	row(const std::function<void(util::Reference<Row> &)> &fn)
	{
		row(Row::DEFAULT_SETTINGS, fn);
	}

	void call(const std::function<void(util::Reference<Widget> &)> &fn);
	/* convenience wrapper to Window->display(this) */
	void display();

	util::Reference<Window> gui() const;
	const std::string &
	title() const
	{
		return _title;
	}
	void
	set_title(const std::string &title)
	{
		_title = title;
	}
	const Rectangle &
	rect() const
	{
		return _rect;
	}
	void
	set_rect(const Rectangle &rect)
	{
		_rect = rect;
	}
	const Flags &
	flags() const
	{
		return _flags;
	}
	void
	set_flags(const Flags &flags)
	{
		_flags = flags;
	}

	[[nodiscard]] ID
	id() const
	{
		return _id;
	}

	Rectangle bounds() const;
	glm::vec2 position() const;
	glm::vec2 size() const;
	float width() const;
	float height() const;
	Rectangle content_region() const;
	bool are_any_active() const;
	glm::uvec2 scroll() const;
	bool has_focus() const;
	bool is_hovered() const;
	void set_bounds(const Rectangle &rect);
	void set_position(const glm::vec2 &pos);
	void set_size(const glm::vec2 &size);
	void focus();
	void set_scroll(const glm::uvec2 &scroll);
	void close();
	void minimize();
	void maximize();
	void show();

private:
	bool in_draw() const;
	nk_context *context() const;

	util::WeakReference<Window> _gui;
	ID _id;
	std::string _title;
	Rectangle _rect;
	Flags _flags;
	std::vector<util::Reference<Row>> _rows;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_WIDGET_H */
