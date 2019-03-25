#pragma once

#include <windows.h>
#include "Framework/Common/BaseApplication.h"

namespace Engine {

	class WindowsApplication : public BaseApplication
	{
	private:
		HWND m_hWnd;

	public:
		WindowsApplication() = default;
		WindowsApplication(GfxConfiguration& config) : BaseApplication(config) {};
		~WindowsApplication() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();

		inline HWND GetMainWindow() const { return m_hWnd; };

	private:
		// the WindowProc function prototype
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

}
