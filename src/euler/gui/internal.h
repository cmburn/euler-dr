/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_INTERNAL_H
#define EULER_GUI_INTERNAL_H

/* DO NOT INCLUDE VK2D/nuklear.h DIRECTLY!
 * Include this header instead. */
#include <VK2D/Gui.h>

#include "euler/graphics/image.h"
#include "euler/gui/common.h"
#include "euler/util/color.h"
#include "euler/util/state.h"

namespace euler::gui {

/* ReSharper disable once CppRedundantElaboratedTypeSpecifier */
struct nk_image image_to_nuklear(const util::Reference<util::State> &,
    const util::Reference<graphics::Image> &);

nk_color to_nuklear(const util::Reference<util::State> &state,
    util::Color color);
nk_symbol_type to_nuklear(const util::Reference<util::State> &state,
    Symbol symbol);
nk_flags to_nuklear_text(const util::Reference<util::State> &state,
    Alignment align);
nk_flags to_nuklear_widget(const util::Reference<util::State> &state,
    Alignment align);
nk_flags to_nuklear_header(const util::Reference<util::State> &state,
    Alignment align);
struct nk_image to_nuklear(const util::Reference<util::State> &state,
    const util::Reference<graphics::Image> &);

util::Color from_nuklear(const util::Reference<util::State> &state,
    nk_color color);
Symbol from_nuklear(const util::Reference<util::State> &state,
    nk_symbol_type symbol);
Alignment from_nuklear_text(const util::Reference<util::State> &state,
    nk_flags align);
Alignment from_nuklear_widget(const util::Reference<util::State> &state,
    nk_flags align);
Alignment from_nuklear_header(const util::Reference<util::State> &state,
    nk_flags align);

Symbol to_symbol(mrb_state *mrb, mrb_sym sym);
mrb_value from_symbol(mrb_state *mrb, Symbol symbol);

} /* namespace euler::gui */

#endif /* EULER_GUI_INTERNAL_H */
