/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STATE_H
#define EULER_UTIL_STATE_H

#include <thread>

#include "euler/util/mruby_state.h"
#include "euler/util/object.h"

namespace euler::util {
class Logger;
class Storage;
class Image;
class State : public Object {
public:
	~State() override = default;
	State();
	typedef uint64_t tick_t;
	typedef decltype(std::thread::hardware_concurrency()) nthread_t;

	struct Module {
		RClass *mod;
		struct {
			RClass *mod;
		} util;
		struct {
			RClass *mod;
			RClass *column_vector_f32;
			RClass *column_vector_f64;
			RClass *column_vector_i16;
			RClass *column_vector_i32;
			RClass *column_vector_i64;
			RClass *column_vector_u16;
			RClass *column_vector_u32;
			RClass *column_vector_u64;
			RClass *column_vector_c32;
			RClass *column_vector_c64;

			template <typename T>
			RClass *
			column_vector() const
			{
				if constexpr (std::is_same_v<T, float>)
					return column_vector_f32;
				if constexpr (std::is_same_v<T, double>)
					return column_vector_f64;
				if constexpr (std::is_same_v<T, int16_t>)
					return column_vector_i16;
				if constexpr (std::is_same_v<T, int32_t>)
					return column_vector_i32;
				if constexpr (std::is_same_v<T, int64_t>)
					return column_vector_i64;
				if constexpr (std::is_same_v<T, uint16_t>)
					return column_vector_u16;
				if constexpr (std::is_same_v<T, uint32_t>)
					return column_vector_u32;
				if constexpr (std::is_same_v<T, uint64_t>)
					return column_vector_u64;
				if constexpr (std::is_same_v<T,
						  std::complex<float>>)
					return column_vector_c32;
				if constexpr (std::is_same_v<T,
						  std::complex<double>>)
					return column_vector_c64;
				return nullptr;
			}
			
			RClass *cube_f32;
			RClass *cube_f64;
			RClass *cube_i16;
			RClass *cube_i32;
			RClass *cube_i64;
			RClass *cube_u16;
			RClass *cube_u32;
			RClass *cube_u64;
			RClass *cube_c32;
			RClass *cube_c64;
			
			
			template <typename T>
			RClass *
			cube() const
			{
				if constexpr (std::is_same_v<T, float>)
					return cube_f32;
				if constexpr (std::is_same_v<T, double>)
					return cube_f64;
				if constexpr (std::is_same_v<T, int16_t>)
					return cube_i16;
				if constexpr (std::is_same_v<T, int32_t>)
					return cube_i32;
				if constexpr (std::is_same_v<T, int64_t>)
					return cube_i64;
				if constexpr (std::is_same_v<T, uint16_t>)
					return cube_u16;
				if constexpr (std::is_same_v<T, uint32_t>)
					return cube_u32;
				if constexpr (std::is_same_v<T, uint64_t>)
					return cube_u64;
				if constexpr (std::is_same_v<T,
						  std::complex<float>>)
					return cube_c32;
				if constexpr (std::is_same_v<T,
						  std::complex<double>>)
					return cube_c64;
				return nullptr;
			}

			
			RClass *matrix_f32;
			RClass *matrix_f64;
			RClass *matrix_i16;
			RClass *matrix_i32;
			RClass *matrix_i64;
			RClass *matrix_u16;
			RClass *matrix_u32;
			RClass *matrix_u64;
			RClass *matrix_c32;
			RClass *matrix_c64;

			template <typename T>
			RClass *
			matrix() const
			{
				if constexpr (std::is_same_v<T, float>)
					return matrix_f32;
				if constexpr (std::is_same_v<T, double>)
					return matrix_f64;
				if constexpr (std::is_same_v<T, int16_t>)
					return matrix_i16;
				if constexpr (std::is_same_v<T, int32_t>)
					return matrix_i32;
				if constexpr (std::is_same_v<T, int64_t>)
					return matrix_i64;
				if constexpr (std::is_same_v<T, uint16_t>)
					return matrix_u16;
				if constexpr (std::is_same_v<T, uint32_t>)
					return matrix_u32;
				if constexpr (std::is_same_v<T, uint64_t>)
					return matrix_u64;
				if constexpr (std::is_same_v<T,
						  std::complex<float>>)
					return matrix_c32;
				if constexpr (std::is_same_v<T,
						  std::complex<double>>)
					return matrix_c64;
				return nullptr;
			}

			RClass *row_vector_f32;
			RClass *row_vector_f64;
			RClass *row_vector_i16;
			RClass *row_vector_i32;
			RClass *row_vector_i64;
			RClass *row_vector_u16;
			RClass *row_vector_u32;
			RClass *row_vector_u64;
			RClass *row_vector_c32;
			RClass *row_vector_c64;



			RClass *sparse_matrix_f32;
			RClass *sparse_matrix_f64;
			RClass *sparse_matrix_i16;
			RClass *sparse_matrix_i32;
			RClass *sparse_matrix_i64;
			RClass *sparse_matrix_u16;
			RClass *sparse_matrix_u32;
			RClass *sparse_matrix_u64;
			RClass *sparse_matrix_c32;
			RClass *sparse_matrix_c64;
		} math;
	};

	[[nodiscard]] virtual Reference<MRubyState> mrb() const = 0;
	[[nodiscard]] virtual RClass *object_class() const = 0;
	[[nodiscard]] virtual Reference<Logger> log() const = 0;
	[[nodiscard]] virtual Reference<Storage> user_storage() const = 0;
	[[nodiscard]] virtual Reference<Storage> title_storage() const = 0;
	[[nodiscard]] virtual nthread_t available_threads() const = 0;
	[[nodiscard]] virtual tick_t ticks() const = 0;
	[[nodiscard]] virtual tick_t last_tick() const = 0;
	[[nodiscard]] virtual float fps() const = 0;
	[[nodiscard]] virtual tick_t total_ticks() const = 0;
	[[nodiscard]] virtual mrb_value gv_state() const = 0;
	[[nodiscard]] static Reference<State> get(const mrb_state *mrb);
	[[nodiscard]] virtual Reference<Image> load_image(const char *path) = 0;
	virtual void upload_image(const char *label,
	    const Reference<Image> &img) = 0;
	virtual void initialize() = 0;
	virtual void tick() const = 0;
	[[nodiscard]] virtual const Module &modules() const = 0;
	[[nodiscard]] virtual Module &modules() = 0;
};

} /* namespace euler::util */

#endif /* EULER_UTIL_STATE_H */
