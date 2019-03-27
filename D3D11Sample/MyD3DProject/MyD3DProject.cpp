// MyD3DProject.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "MyD3DProject.h"

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
HWND hWnd;										// 主視窗
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateGraphicsResources();
void CreateRenderTargetView();
void CreateDepthStencilView();
void SetViewport();

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置程式碼。
	CreateGraphicsResources();

	// 將全域字串初始化
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MYD3DPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 執行應用程式初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYD3DPROJECT));

	MSG msg;

	// 主訊息迴圈:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYD3DPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYD3DPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解:
//
//        在這個函式中，我們將執行個體控制代碼儲存在全域變數中，
//        並建立及顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途: 處理主視窗的訊息。
//
//  WM_COMMAND  - 處理應用程式功能表
//  WM_PAINT    - 繪製主視窗
//  WM_DESTROY  - 張貼結束訊息然後傳回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 剖析功能表選取項目:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此新增任何使用 hdc 的繪圖程式碼...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

ID3D11Device* g_pD3D11Device;
D3D_FEATURE_LEVEL g_FeatureLevel;
ID3D11DeviceContext* g_pD3D11DeviceContext;
UINT g_4XMSAAQuality;
IDXGISwapChain* g_pDXGISwapChain;
ID3D11RenderTargetView* g_pD3D11RenderTargetView;
ID3D11DepthStencilView* g_pD3D11DepthStencilView;

HRESULT CreateGraphicsResources() {

	HRESULT hr = S_OK;

	const D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		FeatureLevels,
		_countof(FeatureLevels),
		D3D11_SDK_VERSION,
		&g_pD3D11Device,
		&g_FeatureLevel,
		&g_pD3D11DeviceContext
	);

	hr = g_pD3D11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &g_4XMSAAQuality);

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = 960;
	scd.BufferDesc.Height = 540;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (g_4XMSAAQuality) {
		scd.SampleDesc.Count = 4;
		scd.SampleDesc.Quality = g_4XMSAAQuality - 1;
	}
	else {
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
	}

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	IDXGIDevice* DXGIDevice = nullptr;
	hr = g_pD3D11Device->QueryInterface(IID_PPV_ARGS(&DXGIDevice));

	IDXGIAdapter* DXGIAdapter = nullptr;
	hr = DXGIDevice->GetParent(IID_PPV_ARGS(&DXGIAdapter));

	IDXGIFactory* DXGIFactory = nullptr;
	hr = DXGIAdapter->GetParent(IID_PPV_ARGS(&DXGIFactory));

	hr = DXGIFactory->CreateSwapChain(g_pD3D11Device, &scd, &g_pDXGISwapChain);

	SafeRelease(&DXGIDevice);
	SafeRelease(&DXGIAdapter);
	SafeRelease(&DXGIFactory);

	if (SUCCEEDED(hr)) {
		CreateRenderTargetView();
		CreateDepthStencilView();
		g_pD3D11DeviceContext->OMGetRenderTargets(1, &g_pD3D11RenderTargetView, &g_pD3D11DepthStencilView);

		SetViewport();
	}

	return hr;
}

void CreateRenderTargetView() {

	HRESULT hr = S_OK;

	ID3D11Texture2D* backBuffer;

	hr = g_pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	hr = g_pD3D11Device->CreateRenderTargetView(backBuffer, nullptr, &g_pD3D11RenderTargetView);

	SafeRelease(&backBuffer);
}

void CreateDepthStencilView() {

	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC td;

	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.Width = 960;
	td.Height = 540;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (g_4XMSAAQuality) {
		td.SampleDesc.Count = 4;
		td.SampleDesc.Quality = g_4XMSAAQuality - 1;
	}
	else {
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
	}

	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;

	hr = g_pD3D11Device->CreateTexture2D(&td, nullptr, &depthStencilBuffer);
	hr = g_pD3D11Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &g_pD3D11DepthStencilView);
}

void SetViewport() {
	D3D11_VIEWPORT v;
	v.TopLeftX = 0.0f;
	v.TopLeftY = 0.0f;
	v.Width = static_cast<float>(960);
	v.Height = static_cast<float>(540);
	v.MinDepth = 0.0f;
	v.MaxDepth = 1.0f;
	g_pD3D11DeviceContext->RSSetViewports(1, &v);
}