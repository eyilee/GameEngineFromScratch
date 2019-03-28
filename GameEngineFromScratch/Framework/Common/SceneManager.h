#pragma once

#include "Framework/Math/math.h"
#include "Framework/Common/Scene.h"
#include "Framework/Interface/IRuntimeModule.h"

namespace Engine {
	class SceneManager : public IRuntimeModule
	{
	public:
		virtual ~SceneManager();

		virtual int Initialize();
		virtual void Finalize();

		virtual void Tick();

		void LoadScene(const char* scene_file_name);

		const Scene& GetSceneForRendering();

	protected:
		void LoadOgexScene(const char* ogex_scene_file_name);

	protected:
		std::unique_ptr<Scene>  m_pScene;
	};

	extern SceneManager* g_pSceneManager;
}

