#pragma once

#include <memory>
#include <string>
#include "Framework/Common/Scene.h"
#include "Framework/Common/SceneNode.h"

namespace Engine {

	class ISceneParser
	{
	public:
		ISceneParser() = default;
		virtual ~ISceneParser() = default;

		virtual std::unique_ptr<Scene> Parse(const std::string& buf) = 0;
	};

}
