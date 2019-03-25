#pragma once

#include <memory>
#include <string>
#include "Framework/Common/SceneNode.h"

namespace Engine {
	class ISceneParser
	{
	public:
		virtual std::unique_ptr<BaseSceneNode> Parse(const std::string& buf) = 0;
	};
}

