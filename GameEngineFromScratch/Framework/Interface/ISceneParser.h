#pragma once

#include <memory>
#include <string>
#include "Framework/Common/SceneNode.h"

namespace Engine {

	class ISceneParser
	{
	public:
		ISceneParser() = default;
		~ISceneParser() = default;

		virtual std::unique_ptr<BaseSceneNode> Parse(const std::string& buf) = 0;
	};

}
