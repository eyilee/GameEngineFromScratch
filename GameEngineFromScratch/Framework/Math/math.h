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
	};

	typedef Vector4Type<float> Vector4f;
	typedef Vector4Type<uint8_t> R8G8B8A8Unorm;
	typedef Vector4Type<uint8_t> Vector4i;

}