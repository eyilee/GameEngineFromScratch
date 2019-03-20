#pragma once

#include "IRuntimeModule.h"

namespace Engine {

	class IApplication : public IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;

		// One cycle of the main loop
		virtual void Tick() = 0;
		virtual bool IsQuit() = 0;
	};

}