#pragma once

#include "Framework/Interface/IRuntimeModule.h"
#include "Mesh.h"

namespace Engine {

	class GraphicsManager : public IRuntimeModule
	{
	public:
		GraphicsManager() = default;
		virtual ~GraphicsManager() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();

		void DrawSingleMesh(const Mesh& mesh);
	};

}
