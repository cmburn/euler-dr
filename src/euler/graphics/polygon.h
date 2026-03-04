/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_POLYGON_H
#define EULER_RENDERER_POLYGON_H

#include <vector>

#include "euler/util/math.h"
#include "euler/util/object.h"

typedef struct VK2DPolygon_t *VK2DPolygon;

namespace euler::graphics {
class Polygon : public util::Object {
public:
	struct VertexColor {
		util::Vec3 position;
		util::Vec4 color;
	};
	Polygon(const std::vector<VertexColor> &vertices);
	Polygon(const std::vector<util::Vec2> &vertices, bool filled = false);
	~Polygon();

private:
	VK2DPolygon _polygon = nullptr;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_POLYGON_H */
