/* SPDX-License-Identifier: ISC */

#include "euler/graphics/sprite.h"

#include <VK2D/Renderer.h>
#include <VK2D/Texture.h>

euler::graphics::Sprite::Sprite(const util::Reference<Image> &image)
    : _image(image)

{
}