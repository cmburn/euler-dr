/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <string>

#include "euler/util/ext.h"
#include "euler/util/math.h"
#include "euler/util/object.h"

namespace euler::math {
using size_type = util::size_type;
static constexpr auto Dynamic = util::Dynamic;

class Matrix : public util::Object {
	BIND_MRUBY("Euler::Math::Matrix", Matrix, math.matrix);

public:
	enum class Type {
		Float,
		Double,
		Int16,
		Int32,
		Int64,
		UInt16,
		UInt32,
		UInt64,
	};
	[[nodiscard]] virtual Type type() const = 0;
	[[nodiscard]] virtual bool is_fixed() const = 0;
	[[nodiscard]] virtual bool is_row_vector() const = 0;
	[[nodiscard]] virtual bool is_vector() const = 0;
	[[nodiscard]] virtual size_type row_count() const = 0;
	[[nodiscard]] virtual size_type column_count() const = 0;
	[[nodiscard]] virtual util::Reference<Matrix> size() const = 0;
	[[nodiscard]] virtual std::string to_string() const = 0;
};

template <size_type Rows, size_type Columns> class SizedMatrix : public Matrix {
public:
	static_assert(Rows != Dynamic && Columns != Dynamic);

	[[nodiscard]] size_type
	row_count() const override
	{
		return Rows;
	}

	size_type
	column_count() const override
	{
		return Columns;
	}

	bool
	is_fixed() const override
	{
		return true;
	}
};

template <> class SizedMatrix<Dynamic, Dynamic> : public Matrix {
public:
	size_type
	row_count() const override
	{
		return _row_count;
	}

	size_type
	column_count() const override
	{
		return _column_count;
	}

	bool
	is_fixed() const override
	{
		return false;
	}

	bool
	is_row_vector() const override
	{
		return false;
	}

	bool
	is_vector() const override
	{
		return false;
	}

private:
	size_type _row_count = 0;
	size_type _column_count = 0;
};

template <> class SizedMatrix<Dynamic, 1> : public Matrix {
public:
	size_type
	row_count() const override
	{
		return _row_count;
	}

	size_type
	column_count() const override
	{
		return 1;
	}

private:
	size_type _row_count = 0;
};

template <> class SizedMatrix<1, Dynamic> : public Matrix {
public:
	size_type
	row_count() const override
	{
		return 1;
	}

	size_type
	column_count() const override
	{
		return _column_count;
	}

private:
	size_type _column_count = 0;
};

template <typename T = float, size_type Rows = Dynamic,
    size_type Columns = Dynamic>
class MatrixImpl : public SizedMatrix<Rows, Columns> {
	static constexpr bool is_fixed = Rows != Dynamic && Columns != Dynamic;

public:
	typedef T value_type;
	static constexpr size_type row_count = Rows;
	static constexpr size_type column_count = Columns;
	typedef Eigen::Matrix<value_type, row_count, column_count> matrix_type;

private:
	matrix_type _matrix;
};

template <typename T = float, size_type Rows = Dynamic>
using Vector = MatrixImpl<T, Rows, 1>;

template <typename T = float, size_type Columns = Dynamic>
using RowVector = MatrixImpl<T, 1, Columns>;

} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
