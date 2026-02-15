/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_POLYGON_H
#define EULER_RENDERER_POLYGON_H

#include <vector>

#include <armadillo>

#include "euler/util/object.h"

typedef struct VK2DPolygon_t *VK2DPolygon;

namespace euler::graphics {
class Polygon : public util::Object {
public:
	struct VertexColor {
		arma::Col<float>::fixed<3> position;
		arma::Col<float>::fixed<4> color;
	};
	Polygon(const std::vector<VertexColor> &vertices);
	Polygon(const std::vector<arma::Col<float>::fixed<2>> &vertices,
	    bool filled = false);
	~Polygon();

private:
	VK2DPolygon _polygon = nullptr;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_POLYGON_H */
