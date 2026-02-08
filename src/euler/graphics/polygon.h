/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_POLYGON_H
#define EULER_RENDERER_POLYGON_H

// #include <glm/glm.hpp>
#include <vector>

#include "euler/math/vector.h"
#include "euler/util/object.h"

typedef struct VK2DPolygon_t *VK2DPolygon;

namespace euler::graphics {
class Polygon : public util::Object {
public:
	struct VertexColor {
		util::Reference<math::VectorImpl<float, 3>> position;
		util::Reference<math::VectorImpl<float, 4>> color;
	};
	Polygon(const std::vector<VertexColor> &vertices);
	Polygon(const std::vector<util::Reference<math::VectorImpl<float, 2>>>
		    &vertices,
	    bool filled = false);
	~Polygon();

private:
	VK2DPolygon _polygon = nullptr;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_POLYGON_H */
