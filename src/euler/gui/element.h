/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_ELEMENT_H
#define EULER_GUI_ELEMENT_H

#include "euler/graphics/window.h"
#include "euler/util/object.h"

namespace euler::gui {
class Row;

class Element : public util::Object {
public:
	// enum class Alignment {
	// 	Left = 1,
	// 	HCenter = 2,
	// 	Right = 3,
	// 	Top = 1 << 2,
	// 	VCenter = 2 << 2,
	// 	Bottom = 3 << 2,
	// };

	explicit Element(const util::Reference<Row> &);
	~Element() override;

	util::Reference<Row> row() const;
	util::Reference<util::State> state() const;
	vulkan::detail::gui_context *context() const;

private:
	util::Reference<Row> _row;
};

template <typename T>
using element_fn = std::function<bool(const util::Reference<T> &)>;

} /* namespace euler::gui */

#endif /* EULER_GUI_ELEMENT_H */
