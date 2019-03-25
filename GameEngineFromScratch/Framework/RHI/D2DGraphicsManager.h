#pragma once

#include <d2d1.h>
#include "Framework/Common/GraphicsManager.h"

namespace Engine {

	class D2DGraphicsManager : public GraphicsManager
	{
	protected:
		ID2D1Factory *m_pFactory = nullptr;
		ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;

	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

	protected:
		HRESULT CreateGraphicsResources();
	};

}