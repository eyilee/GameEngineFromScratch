#pragma once

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine {

	typedef struct _Color {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	} Color;

	typedef struct _Image {
		uint32_t Width;
		uint32_t Height;
		Color* data;
		uint32_t bitcount;
		uint32_t pitch;
		size_t  data_size;
	} Image;

}
