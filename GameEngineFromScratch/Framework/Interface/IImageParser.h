#pragma once

#include "Framework/Common/Buffer.h"
#include "Framework/Common/Image.h"

namespace Engine {

	class IImageParser
	{
	public:
		virtual Image Parse(const Buffer& buf) = 0;
	};

}