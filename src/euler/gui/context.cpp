/* SPDX-License-Identifier: ISC */

#include "euler/gui/context.h"
// #include "../../../cmake-build-debug/_deps/nuklear-src/nuklear.h"

#include "euler/gui/internal.h"
#include "euler/gui/nuklear.h"
#include "euler/gui/widget.h"
#include "euler/util/ext.h"
#include "euler/util/logger.h"
#include "euler/util/window.h"

using euler::gui::Context;

static inline bool
in_rect(const struct nk_rect &rect, const int16_t x, const int16_t y)
{
	return y < rect.h && y >= rect.y && x < rect.w && x >= rect.x;
}

mrb_value
Context::display(const util::Reference<Widget> &widget)
{
	if (widget == nullptr) return mrb_nil_value();
	auto mrb = state()->mrb()->mrb();
	if (state()->phase() != util::State::Phase::Draw) {
		// mrb_raise(mrb, mrb->eStandardError_class,
		//     "Widgets can only be displayed during State#draw");
		state()->mrb()->raise(
		    state()->mrb()->mrb()->eStandardError_class,
		    "Widgets can only be displayed during State#draw");
	}
	const auto id = widget->id();
	if (!_widget_callbacks.contains(widget->id())) {
		state()->mrb()->raise(
		    state()->mrb()->mrb()->eStandardError_class,
		    "Widget was not created by this Window, or has already "
		    "been released.");
	}
	const auto fn = _widget_callbacks.at(id);
	if (std::holds_alternative<NativeCallback>(fn)) {
		widget->call(std::get<NativeCallback>(fn));
		return mrb_nil_value();
	}
	auto out = mrb_nil_value();
	widget->call([&](util::Reference<Widget> &w) -> void {
		const auto block = std::get<mrb_value>(fn);
		auto self_value
		    = w.wrap(mrb, state()->modules().gui.widget, &Widget::TYPE);
		state()->mrb()->funcall_argv(block, EULER_SYM(call), 1,
		    &self_value);
	});
	return out;
}

void
Context::release_widget(const Widget::ID id)
{
	const auto fn = _widget_callbacks.find(id);
	if (fn == _widget_callbacks.end()) return;
	if (std::holds_alternative<mrb_value>(fn->second))
		state()->mrb()->gc_unregister(std::get<mrb_value>(fn->second));
	_widget_callbacks.erase(id);
	_available_widget_ids.push_back(id);
}

const nk_context *
Context::context() const
{
	return &_context;
}

nk_context *
Context::context()
{
	return &_context;
}

euler::util::Reference<euler::util::State>
Context::state() const
{
	return _state.strengthen();
}

euler::gui::Widget::ID
Context::next_widget_id()
{
	if (_available_widget_ids.empty()) return ++_widget_counter;
	const auto id = _available_widget_ids.front();
	_available_widget_ids.pop_front();
	return id;
}

void
Context::render()
{
	using Renderer = graphics::Renderer;
	const nk_command *cmd = nullptr;
	nk_foreach (cmd, &_context) {
		switch (cmd->type) {
		case NK_COMMAND_NOP: break;
		case NK_COMMAND_SCISSOR: {
			const auto casted
			    = reinterpret_cast<const nk_command_scissor *>(cmd);
			const Renderer::ScissorCommand scissor = {
				.position = { casted->x, casted->y },
				.size = { casted->w, casted->h },
			};
			_renderer->scissor(scissor);
		} break;
		case NK_COMMAND_LINE: {
			const auto casted
			    = reinterpret_cast<const nk_command_line *>(cmd);
			const Renderer::LineCommand line = {
				.points = {
					Renderer::Point {
						casted->begin.x,
						casted->begin.y,
					},
					Renderer::Point {
						casted->end.x,
						casted->end.y,
					},
				},
				.line_thickness = casted->line_thickness,
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->line(line);
		} break;
		case NK_COMMAND_CURVE: {
			const auto casted
			    = reinterpret_cast<const nk_command_curve *>(cmd);
			const Renderer::CurveCommand curve = {
				.line_thickness = casted->line_thickness,
				.points = {
					Renderer::Point {
						casted->begin.x,
						casted->begin.y,
					},
					Renderer::Point {
						casted->ctrl[0].x,
						casted->ctrl[0].y,
					},
					Renderer::Point {
						casted->ctrl[1].x,
						casted->ctrl[1].y,
					},
					Renderer::Point {
						casted->end.x,
						casted->end.y,
					},
				},
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->curve(curve);
		} break;
		case NK_COMMAND_RECT: {
			const auto casted
			    = reinterpret_cast<const nk_command_rect *>(cmd);
			const Renderer::RectCommand rect = {
				.rounding = casted->rounding,
				.line_thickness = casted->line_thickness,
				.position = { casted->x, casted->y },
				.size = { casted->w, casted->h },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->rect(rect);
		} break;
		case NK_COMMAND_RECT_FILLED: {
			const auto casted
			    = reinterpret_cast<const nk_command_rect_filled *>(
				cmd);
			const Renderer::RectFilledCommand rect_filled = {
				.rounding = casted->rounding,
				.position = { casted->x, casted->y },
				.size = { casted->w, casted->h },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->rect_filled(rect_filled);
		} break;
		case NK_COMMAND_RECT_MULTI_COLOR: {
			const auto casted = reinterpret_cast<
			    const nk_command_rect_multi_color *>(cmd);
			const Renderer::RectMultiColorCommand rmc = {
				.position = { casted->x, casted->y },
				.size = { casted->w, casted->h },
				.left = util::Color::from_nk(casted->left),
				.top = util::Color::from_nk(casted->top),
				.bottom = util::Color::from_nk(casted->bottom),
				.right = util::Color::from_nk(casted->right),
			};
			_renderer->rect_multi_color(rmc);
		} break;
		case NK_COMMAND_CIRCLE: {
			const auto casted
			    = reinterpret_cast<const nk_command_circle *>(cmd);
			const Renderer::CircleCommand circle = {
				.center = { casted->x, casted->y },
				.line_thickness = casted->line_thickness,
				.size = { casted->w, casted->h },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->circle(circle);
		} break;
		case NK_COMMAND_CIRCLE_FILLED: {
			const auto casted = reinterpret_cast<
			    const nk_command_circle_filled *>(cmd);
			const Renderer::CircleFilledCommand circle_filled = {
				.center = { casted->x, casted->y },
				.size = { casted->w, casted->h },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->circle_filled(circle_filled);
		} break;
		case NK_COMMAND_ARC: {
			const auto casted
			    = reinterpret_cast<const nk_command_arc *>(cmd);
			const Renderer::ArcCommand arc = {
				.center = { casted->cx, casted->cy },
				.radius = casted->r,
				.line_thickness = casted->line_thickness,
				.a = { casted->a[0], casted->a[1] },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->arc(arc);
		} break;
		case NK_COMMAND_ARC_FILLED: {
			const auto casted
			    = reinterpret_cast<const nk_command_arc_filled *>(
				cmd);
			const Renderer::ArcFilledCommand arc_filled = {
				.center = { casted->cx, casted->cy },
				.radius = casted->r,
				.a = { casted->a[0], casted->a[1] },
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->arc_filled(arc_filled);
		} break;
		case NK_COMMAND_TRIANGLE: {
			const auto casted
			    = reinterpret_cast<const nk_command_triangle *>(
				cmd);
			const Renderer::TriangleCommand triangle = {
				.line_thickness = casted->line_thickness,
				.points = {
					Renderer::Point {
						casted->a.x,
						casted->a.y,
					},
					Renderer::Point {
						casted->b.x,
						casted->b.y,
					},
					Renderer::Point {
						casted->c.x,
						casted->c.y,
					},
				},
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->triangle(triangle);
		} break;
		case NK_COMMAND_TRIANGLE_FILLED: {
			const auto casted = reinterpret_cast<
			    const nk_command_triangle_filled *>(cmd);
			const Renderer::TriangleFilledCommand triangle_filled = {
				.points = {
					Renderer::Point {
						casted->a.x,
						casted->a.y,
					},
					Renderer::Point {
						casted->b.x,
						casted->b.y,
					},
					Renderer::Point {
						casted->c.x,
						casted->c.y,
					},
				},
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->triangle_filled(triangle_filled);
		} break;
		case NK_COMMAND_POLYGON: {
			const auto casted
			    = reinterpret_cast<const nk_command_polygon *>(cmd);
			std::vector<Renderer::Point> points;
			for (int i = 0; i < casted->point_count; ++i) {
				points.emplace_back(casted->points[i].x,
				    casted->points[i].y);
			}
			const Renderer::PolygonCommand polygon = {
				.line_thickness = casted->line_thickness,
				.points = std::move(points),
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->polygon(polygon);
		} break;
		case NK_COMMAND_POLYGON_FILLED: {
			const auto casted = reinterpret_cast<
			    const nk_command_polygon_filled *>(cmd);
			std::vector<Renderer::Point> points;
			for (int i = 0; i < casted->point_count; ++i) {
				points.emplace_back(casted->points[i].x,
				    casted->points[i].y);
			}
			const Renderer::PolygonFilledCommand polygon_filled = {
				.points = std::move(points),
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->polygon_filled(polygon_filled);
		} break;
		case NK_COMMAND_POLYLINE: {
			const auto casted
			    = reinterpret_cast<const nk_command_polyline *>(
				cmd);
			std::vector<Renderer::Point> points;
			for (int i = 0; i < casted->point_count; ++i) {
				points.emplace_back(casted->points[i].x,
				    casted->points[i].y);
			}
			const Renderer::MultilineCommand polyline = {
				.line_thickness = casted->line_thickness,
				.points = std::move(points),
				.color = util::Color::from_nk(casted->color),
			};
			_renderer->polyline(polyline);
		} break;
		case NK_COMMAND_TEXT: {
			/* TODO */
		} break;
		case NK_COMMAND_IMAGE: {
			const auto casted
			    = reinterpret_cast<const nk_command_image *>(cmd);
			const Renderer::ImageCommand image = {
				.position = { casted->x, casted->y },
				.size = { casted->w, casted->h },
				.image = nullptr,
				.color = util::Color::from_nk(casted->col),
			};
			_renderer->image(image);
		} break;
		default:
			state()->log()->warn("Unexpected nk_command value {}",
			    static_cast<int>(cmd->type));
			break;
		}
	}
	nk_clear(&_context);
}

euler::util::Reference<euler::gui::Widget>
Context::widget(const char *title, const Widget::Settings &settings,
    const std::function<void(util::Reference<Widget> &)> &fn)
{
	Widget::ID id = next_widget_id();
	_widget_callbacks.emplace(id, fn);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}

euler::util::Reference<euler::gui::Widget>
Context::widget(const char *title, const Widget::Settings &settings,
    mrb_value block)
{
	Widget::ID id = next_widget_id();
	state()->mrb()->gc_register(block);
	_widget_callbacks.emplace(id, block);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}
