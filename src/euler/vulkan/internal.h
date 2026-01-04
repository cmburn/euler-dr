/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_INTERNAL_H
#define EULER_VULKAN_INTERNAL_H

#ifdef EULER_INTERNAL_BUILD
#include <VK2D/VK2D.h>
#include <VK2D/nuklear_defs.h>
#endif

namespace euler::vulkan::detail {
#ifdef EULER_INTERNAL_BUILD
typedef VK2DTexture texture_pointer;
typedef VK2DLogger logger;
typedef nk_context gui_context;
typedef VK2DShader shader_pointer;
#else
typedef void *texture_pointer;
typedef void logger;
typedef void gui_context;
typedef void *shader_pointer;
#endif

} /* namespace euler::vulkan::detail */

#endif /* EULER_VULKAN_INTERNAL_H */
