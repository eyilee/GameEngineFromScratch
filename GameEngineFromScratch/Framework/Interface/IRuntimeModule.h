#pragma once

namespace Engine {

	class IRuntimeModule
	{
	public:
		IRuntimeModule() = default;
		~IRuntimeModule() = default;

		virtual int Initialize() = 0;

		virtual void Finalize() = 0;

		virtual void Tick() = 0;
	};

}
