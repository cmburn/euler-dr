/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/surface.h"

#include <VK2D/Constants.h>
#include <VK2D/Gui.h>
#include <VK2D/Renderer.h>
#include <VK2D/Structs.h>
#include <VK2D/Texture.h>

#include "euler/vulkan/renderer.h"

euler::vulkan::Surface::Surface() = default;
euler::vulkan::Surface::~Surface() = default;

const euler::util::Reference<euler::vulkan::Renderer> &
euler::vulkan::Surface::renderer() const
{
	return _renderer;
}

euler::util::Reference<euler::vulkan::Renderer> &
euler::vulkan::Surface::renderer()
{
	return _renderer;
}
bool
euler::vulkan::Surface::draw(int &exit_code,
    const std::function<bool(int &)> &fn)
{
	vk2dRendererStartFrame(util::COLOR_BLACK.to_float_array().data());
	try {
		render_debug_overlay();
		const auto result = fn(exit_code);
		vk2dRendererEndFrame();
		return result;
	} catch (const std::exception &e) {
		log()->error("Unhandled exception in frame: {}", e.what());
	} catch (...) {
		log()->error("Unknown exception in frame");
	}
	vk2dRendererEndFrame();
	return false;
}

/* this is haphazardly stolen from debug.c in VK2D, sorry Paolo */

void
euler::vulkan::Surface::render_debug_overlay()
{
	if (_debug_font == nullptr)
		_debug_font = vk2dTextureLoad("assets/font.png");
	vk2dRendererLockCameras(VK2D_DEFAULT_CAMERA);
	char title[100];
	VK2DRendererConfig conf = vk2dRendererGetConfig();
	float inUse, total;
	vk2dRendererGetVRAMUsage(&inUse, &total);
	sprintf(title,
	    "Euler [%0.2fms] [%0.2ffps] %ix MSAA\nVRAM: %0.2fMiB/%0.2fGiB",
	    vk2dRendererGetAverageFrameTime(),
	    1000 / vk2dRendererGetAverageFrameTime(), conf.msaa, inUse,
	    total / 1024);
	vk2dRendererSetColourMod(VK2D_BLACK);
	int w, h;
	SDL_GetWindowSize(window(), &w, &h);
	vk2dDrawRectangle(0, 0, static_cast<float>(w), 17 * 2 * 2);
	vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
	int x = 0, y = 0;
	float ox = x;
	for (int i = 0; i < (int)strlen(title); i++) {
		if (title[i] != '\n') {
			vk2dRendererDrawTexture(_debug_font, x, y, 2, 2, 0, 0,
			    0, (title[i] * 8) % 128, floorf(title[i] / 16) * 16,
			    8, 16);
			x += 8 * 2;
		} else {
			x = ox;
			y += 16 * 2;
		}
	}

	vk2dRendererUnlockCameras();
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
euler::vulkan::Surface::start_gui_input()
{
	vk2dGuiStartInput();
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
euler::vulkan::Surface::end_gui_input()
{
	vk2dGuiEndInput();
	nk_clear(vk2dGuiContext());
}

void
euler::vulkan::Surface::set_renderer(const util::Reference<Renderer> &renderer)
{
	_renderer = renderer;
}