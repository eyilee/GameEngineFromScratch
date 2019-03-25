#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif

namespace Engine {

	template<typename T, size_t SizeOfArray>
	constexpr size_t countof(T(&)[SizeOfArray]) { return SizeOfArray; }

	template<typename T, size_t RowSize, size_t ColSize>
	constexpr size_t countof(T(&)[RowSize][ColSize]) { return RowSize * ColSize; }

	template<typename T>
	constexpr float normalize(T value) {
		return value < 0
			? -static_cast<float>(value) / (std::numeric_limits<T>::min)()
			: static_cast<float>(value) / (std::numeric_limits<T>::max)()
			;
	}

	template <template<typename> class TT, typename T, int ... Indexes>
	class swizzle {
		T v[sizeof...(Indexes)];

	public:
		TT<T>& operator=(const TT<T>& rhs)
		{
			int indexes[] = { Indexes... };
			for (int i = 0; i < sizeof...(Indexes); i++) {
				v[indexes[i]] = rhs[i];
			}
			return *(TT<T>*)this;
		}

		operator TT<T>() const
		{
			return TT<T>(v[Indexes]...);
		}
	};

	template <typename T>
	struct Vector2Type
	{
		union {
			T data[2];
			struct { T x, y; };
			struct { T u, v; };
			swizzle<Engine::Vector2Type, T, 0, 1> xy;
			swizzle<Engine::Vector2Type, T, 1, 0> yx;
		};

		Vector2Type<T>() {};
		Vector2Type<T>(const T& _v) : x(_v), y(_v) {};
		Vector2Type<T>(const T& _x, const T& _y) : x(_x), y(_y) {};

		operator T*() { return data; };
		operator const T*() const { return static_cast<const T*>(data); };
	};

	typedef Vector2Type<float> Vector2f;

	template <typename T>
	struct Vector3Type
	{
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			swizzle<Engine::Vector2Type, T, 0, 1> xy;
			swizzle<Engine::Vector2Type, T, 0, 2> xz;
			swizzle<Engine::Vector2Type, T, 1, 0> yx;
			swizzle<Engine::Vector2Type, T, 1, 2> yz;
			swizzle<Engine::Vector2Type, T, 2, 0> zx;
			swizzle<Engine::Vector2Type, T, 2, 1> zy;
			swizzle<Engine::Vector3Type, T, 0, 1, 2> xyz;
			swizzle<Engine::Vector3Type, T, 0, 2, 1> xzy;
			swizzle<Engine::Vector3Type, T, 1, 0, 2> yxz;
			swizzle<Engine::Vector3Type, T, 1, 2, 0> yzx;
			swizzle<Engine::Vector3Type, T, 2, 0, 1> zxy;
			swizzle<Engine::Vector3Type, T, 2, 1, 0> zyx;
		};

		Vector3Type<T>() {};
		Vector3Type<T>(const T& _v) : x(_v), y(_v), z(_v) {};
		Vector3Type<T>(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {};

		operator T*() { return data; };
		operator const T*() const { return static_cast<const T*>(data); };
	};

	typedef Vector3Type<float> Vector3f;

	template <typename T>
	struct Vector4Type
	{
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			swizzle<Engine::Vector3Type, T, 0, 1, 2> xyz;
			swizzle<Engine::Vector3Type, T, 0, 2, 1> xzy;
			swizzle<Engine::Vector3Type, T, 1, 0, 2> yxz;
			swizzle<Engine::Vector3Type, T, 1, 2, 0> yzx;
			swizzle<Engine::Vector3Type, T, 2, 0, 1> zxy;
			swizzle<Engine::Vector3Type, T, 2, 1, 0> zyx;
			swizzle<Engine::Vector4Type, T, 0, 1, 2, 3> xyzw;
			swizzle<Engine::Vector4Type, T, 0, 1, 2, 3> rgba;
			swizzle<Engine::Vector4Type, T, 2, 1, 0, 3> bgra;
		};

		Vector4Type<T>() {};
		Vector4Type<T>(const T& _v) : x(_v), y(_v), z(_v), w(_v) {};
		Vector4Type<T>(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) {};
		Vector4Type<T>(const Vector3Type<T>& v3) : x(v3.x), y(v3.y), z(v3.z), w(1.0f) {};
		Vector4Type<T>(const Vector3Type<T>& v3, const T& _w) : x(v3.x), y(v3.y), z(v3.z), w(_w) {};

		operator T * () { return data; };
		operator const T * () const { return static_cast<const T*>(data); };

		Vector4Type& operator=(const T* f)
		{
			memcpy(data, f, sizeof(T) * 4);
			return *this;
		};
	};

	typedef Vector4Type<float> Vector4f;
	typedef Vector4Type<float> Quaternion;
	typedef Vector4Type<uint8_t> R8G8B8A8Unorm;
	typedef Vector4Type<uint8_t> Vector4i;

	// Matrix

	template <typename T, int ROWS, int COLS>
	struct Matrix
	{
		union {
			T data[ROWS][COLS];
		};

		T* operator[](int row_index) {
			return data[row_index];
		}

		const T* operator[](int row_index) const {
			return data[row_index];
		}

		operator T*() { return &data[0][0]; };
		operator const T*() const { return static_cast<const T*>(&data[0][0]); };

		Matrix& operator=(const T* _data)
		{
			memcpy(data, _data, ROWS * COLS * sizeof(T));
			return *this;
		}
	};

	typedef Matrix<float, 4, 4> Matrix4X4f;

	template <typename T, int ROWS, int COLS>
	std::ostream& operator<<(std::ostream& out, Matrix<T, ROWS, COLS> matrix)
	{
		out << std::endl;
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				out << matrix.data[i][j] << ((j == COLS - 1) ? '\n' : ',');
			}
		}
		out << std::endl;

		return out;
	}

	void MulByElement(const float a[], const float b[], float result[], const size_t count)
	{
		for (size_t index = 0; index < count; index++) {
			result[index] = a[index] * b[index];
		}
	}

	void DotProduct(const float a[], const float b[], float* result, const size_t count) {
		*result = 0;
		float* r = new float[count];
		MulByElement(a, b, r, count);
		for (size_t index = 0; index < count; index++) {
			*result += r[index];
		}
		delete[] r;
	}

	template <typename T>
	void Normalize(T& result)
	{
		float *v = result;
		size_t count = countof(result.data);

		float length;
		DotProduct(v, v, &length, count);
		length = 1.0f / sqrt(length);

		for (size_t index = 0; index < count; index++) {
			v[index] *= length;
		}
	}

	inline void BuildIdentityMatrix(Matrix4X4f& matrix)
	{
		Matrix4X4f identity = { {{
			{ 1.0f, 0.0f, 0.0f, 0.0f},
			{ 0.0f, 1.0f, 0.0f, 0.0f},
			{ 0.0f, 0.0f, 1.0f, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f}
		}} };

		matrix = identity;

		return;
	}

	inline void MatrixTranslation(Matrix4X4f& matrix, const float x, const float y, const float z)
	{
		Matrix4X4f translation = { {{
			{ 1.0f, 0.0f, 0.0f, 0.0f},
			{ 0.0f, 1.0f, 0.0f, 0.0f},
			{ 0.0f, 0.0f, 1.0f, 0.0f},
			{    x,    y,    z, 1.0f}
		}} };

		matrix = translation;

		return;
	}

	inline void MatrixRotationX(Matrix4X4f& matrix, const float angle)
	{
		float c = cosf(angle), s = sinf(angle);

		Matrix4X4f rotation = { {{
			{  1.0f, 0.0f, 0.0f, 0.0f },
			{  0.0f,    c,    s, 0.0f },
			{  0.0f,   -s,    c, 0.0f },
			{  0.0f, 0.0f, 0.0f, 1.0f },
		}} };

		matrix = rotation;

		return;
	}

	inline void MatrixScale(Matrix4X4f& matrix, const float x, const float y, const float z)
	{
		Matrix4X4f scale = { {{
			{    x, 0.0f, 0.0f, 0.0f},
			{ 0.0f,    y, 0.0f, 0.0f},
			{ 0.0f, 0.0f,    z, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f},
		}} };

		matrix = scale;

		return;
	}

	inline void MatrixRotationY(Matrix4X4f& matrix, const float angle)
	{
		float c = cosf(angle), s = sinf(angle);

		Matrix4X4f rotation = { {{
			{    c, 0.0f,   -s, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{    s, 0.0f,    c, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		}} };

		matrix = rotation;

		return;
	}


	inline void MatrixRotationZ(Matrix4X4f& matrix, const float angle)
	{
		float c = cosf(angle), s = sinf(angle);

		Matrix4X4f rotation = { {{
			{    c,    s, 0.0f, 0.0f },
			{   -s,    c, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		}} };

		matrix = rotation;

		return;
	}

	inline void MatrixRotationAxis(Matrix4X4f& matrix, const Vector3f& axis, const float angle)
	{
		float c = cosf(angle), s = sinf(angle), one_minus_c = 1.0f - c;

		Matrix4X4f rotation = { {{
			{   c + axis.x * axis.x * one_minus_c,  axis.x * axis.y * one_minus_c + axis.z * s, axis.x * axis.z * one_minus_c - axis.y * s, 0.0f    },
			{   axis.x * axis.y * one_minus_c - axis.z * s, c + axis.y * axis.y * one_minus_c,  axis.y * axis.z * one_minus_c + axis.x * s, 0.0f    },
			{   axis.x * axis.z * one_minus_c + axis.y * s, axis.y * axis.z * one_minus_c - axis.x * s, c + axis.z * axis.z * one_minus_c, 0.0f },
			{   0.0f,  0.0f,  0.0f,  1.0f   }
		}} };

		matrix = rotation;
	}

	inline void MatrixRotationQuaternion(Matrix4X4f& matrix, Quaternion q)
	{
		Matrix4X4f rotation = { {{
			{   1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z,  2.0f * q.x * q.y + 2.0f * q.w * q.z,   2.0f * q.x * q.z - 2.0f * q.w * q.y,    0.0f    },
			{   2.0f * q.x * q.y - 2.0f * q.w * q.z,    1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z, 2.0f * q.y * q.z + 2.0f * q.w * q.x,    0.0f    },
			{   2.0f * q.x * q.z + 2.0f * q.w * q.y,    2.0f * q.y * q.z - 2.0f * q.y * q.z - 2.0f * q.w * q.x, 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y, 0.0f    },
			{   0.0f,   0.0f,   0.0f,   1.0f    }
		}} };

		matrix = rotation;
	}

}