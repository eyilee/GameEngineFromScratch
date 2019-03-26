#pragma once

#include "IRuntimeModule.h"

namespace Engine {

	class IApplication : public IRuntimeModule
	{
	public:
		IApplication() = default;
		virtual ~IApplication() = default;

		virtual int Initialize() = 0;

		virtual void Finalize() = 0;

		virtual void Tick() = 0;

		virtual bool IsQuit() = 0;
	};

}