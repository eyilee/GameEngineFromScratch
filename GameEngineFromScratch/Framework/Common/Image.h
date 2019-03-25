#pragma once

#include "Framework/Math/math.h"

using namespace Engine;

namespace Engine {

	typedef struct _Image {
		uint32_t Width;
		uint32_t Height;
		R8G8B8A8Unorm* data;
		uint32_t bitcount;
		uint32_t pitch;
		size_t  data_size;
	} Image;

	std::ostream& operator<<(std::ostream& out, const Image& image)
	{
		out << "Image" << std::endl;
		out << "-----" << std::endl;
		out << "Width: 0x" << image.Width << std::endl;
		out << "Height: 0x" << image.Height << std::endl;
		out << "Bit Count: 0x" << image.bitcount << std::endl;
		out << "Pitch: 0x" << image.pitch << std::endl;
		out << "Data Size: 0x" << image.data_size << std::endl;

		return out;
	}
}
