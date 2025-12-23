/* SPDX-License-Identifier: ISC */

#include "euler/gui/element.h"
#include "VK2D/Gui.h"
#include "widget.h"
#include "window.h"

#include "euler/gui/row.h"

euler::gui::Element::~Element() = default;

euler::util::Reference<euler::gui::Row>
euler::gui::Element::row() const
{
	return _row;
}

euler::util::Reference<euler::util::State>
euler::gui::Element::state() const
{
	return row()->widget()->gui()->state();
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
nk_context *
euler::gui::Element::context() const
{
	return vk2dGuiContext();
}

euler::gui::Element::Element(const util::Reference<Row> &row)
    : _row(row)
{
}
