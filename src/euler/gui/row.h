/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_ROW_H
#define EULER_GUI_ROW_H

#include <functional>

#include "euler/gui/button.h"
#include "euler/gui/checkbox.h"
#include "euler/gui/color_picker.h"
#include "euler/gui/dropdown.h"
#include "euler/gui/element.h"
#include "euler/gui/knob.h"
#include "euler/gui/progress_bar.h"
#include "euler/gui/radio_button.h"
#include "euler/gui/selectable.h"
#include "euler/gui/slider.h"
#include "euler/util/object.h"

namespace euler::gui {
class Widget;

class Row final : public util::Object {
	BIND_MRUBY("Euler::Gui::Row", Row, gui.row);
public:


	enum class Layout : uint8_t {
		Static,
		Dynamic,
		Custom,
	};

	struct Settings {
		float height = 24.0f;
		int columns = 1;
		Layout layout = Layout::Dynamic;
	};

	static const Settings DEFAULT_SETTINGS;

	explicit Row(const util::Reference<Widget> &window,
	    const Settings &settings = DEFAULT_SETTINGS);
	void call(const std::function<void(util::Reference<Row> &)> &fn);

	~Row() override;

	void button(const Button::Settings &settings,
	    const std::function<void(util::Reference<Button> &)> &on_click);
	void
	button(const std::function<void(util::Reference<Button> &)> &on_click)
	{
		button(Button::DEFAULT_SETTINGS, on_click);
	}
	util::Reference<Button> button(const Button::Settings &settings = {});
	util::Reference<Widget> widget() const;

private:
	void end_row();
	util::Reference<Widget> _widget;
	float _height = 24.0f;
	int _item_width = 0;
	int _cols = 1;
	int _col_index = 0;
	Layout _layout = Layout::Dynamic;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_ROW_H */
