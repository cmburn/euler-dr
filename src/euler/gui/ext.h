/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_EXT_H
#define EULER_GUI_EXT_H

#include "euler/util/ext.h"

namespace euler::gui {

// extern const mrb_data_type GUI_BUTTON_TYPE;
// extern const mrb_data_type GUI_ROW_TYPE;
// extern const mrb_data_type GUI_WIDGET_TYPE;

// void init(const util::Reference<util::State> &state);

RClass *init(const util::Reference<util::State> &state, RClass *mod, RClass *);

} /* namespace euler::gui */

#endif /* EULER_GUI_EXT_H */
