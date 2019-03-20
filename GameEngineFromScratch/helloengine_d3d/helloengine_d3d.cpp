﻿// helloengine_d3d.cpp : 定義應用程式的進入點。
//
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <tchar.h>
#include <stdint.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

const uint32_t SCREEN_WIDTH = 960;
const uint32_t SCREEN_HEIGHT = 480;

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

// global declarations
IDXGISwapChain          *g_pSwapchain = nullptr;              // the pointer to the swap chain interface
ID3D11Device            *g_pDev = nullptr;              // the pointer to our Direct3D device interface
ID3D11DeviceContext     *g_pDevcon = nullptr;              // the pointer to our Direct3D device context

ID3D11RenderTargetView  *g_pRTView = nullptr;

ID3D11InputLayout       *g_pLayout = nullptr;              // the pointer to the input layout
ID3D11VertexShader      *g_pVS = nullptr;              // the pointer to the vertex shader
ID3D11PixelShader       *g_pPS = nullptr;              // the pointer to the pixel shader

ID3D11Buffer *g_pVBuffer = nullptr; // Vertex Buffer

// vertex buffer structure
struct VERTEX {
	XMFLOAT3    Position;
	XMFLOAT4    Color;
};

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = nullptr;
	}
}

void CreateRenderTarget() {
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer;

	// Get a pointer to the back buffer
	g_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	// Create a render-target view
	g_pDev->CreateRenderTargetView(pBackBuffer, NULL,
		&g_pRTView);
	pBackBuffer->Release();

	// Bind the view
	g_pDevcon->OMSetRenderTargets(1, &g_pRTView, NULL);
}

void SetViewPort() {
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	g_pDevcon->RSSetViewports(1, &viewport);
}

// this is the function that loads and prepares the shaders
void InitPipeline() {
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	D3DReadFileToBlob(L"SampleVertexShader.vso", &VS);
	D3DReadFileToBlob(L"SamplePixelShader.pso", &PS);

	// encapsulate both shaders into shader objects
	g_pDev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVS);
	g_pDev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPS);

	// set the shader objects
	g_pDevcon->VSSetShader(g_pVS, 0, 0);
	g_pDevcon->PSSetShader(g_pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	g_pDev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pLayout);
	g_pDevcon->IASetInputLayout(g_pLayout);

	VS->Release();
	PS->Release();
}

// this is the function that creates the shape to render
void InitGraphics() {
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(0.45f, -0.5, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	g_pDev->CreateBuffer(&bd, NULL, &g_pVBuffer);       // create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	g_pDevcon->Map(g_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(VERTEX) * 3);                       // copy the data
	g_pDevcon->Unmap(g_pVBuffer, NULL);                                      // unmap the buffer
}

// this function prepare graphic resources for use
HRESULT CreateGraphicsResources(HWND hWnd)
{
	HRESULT hr = S_OK;
	if (g_pSwapchain == nullptr)
	{
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1;                                    // one back buffer
		scd.BufferDesc.Width = SCREEN_WIDTH;
		scd.BufferDesc.Height = SCREEN_HEIGHT;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
		scd.OutputWindow = hWnd;                                // the window to be used
		scd.SampleDesc.Count = 4;                               // how many multisamples
		scd.Windowed = TRUE;                                    // windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

		const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_1,
													D3D_FEATURE_LEVEL_11_0,
													D3D_FEATURE_LEVEL_10_1,
													D3D_FEATURE_LEVEL_10_0,
													D3D_FEATURE_LEVEL_9_3,
													D3D_FEATURE_LEVEL_9_2,
													D3D_FEATURE_LEVEL_9_1 };
		D3D_FEATURE_LEVEL FeatureLevelSupported;

		// create a device, device context and swap chain using the information in the scd struct
		hr = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			FeatureLevels,
			_countof(FeatureLevels),
			D3D11_SDK_VERSION,
			&scd,
			&g_pSwapchain,
			&g_pDev,
			&FeatureLevelSupported,
			&g_pDevcon);

		if (hr == E_INVALIDARG) {
			hr = D3D11CreateDeviceAndSwapChain(NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				0,
				&FeatureLevelSupported,
				1,
				D3D11_SDK_VERSION,
				&scd,
				&g_pSwapchain,
				&g_pDev,
				NULL,
				&g_pDevcon);
		}

		if (hr == S_OK) {
			CreateRenderTarget();
			SetViewPort();
			InitPipeline();
			InitGraphics();
		}
	}
	return hr;
}

void DiscardGraphicsResources()
{
	SafeRelease(&g_pLayout);
	SafeRelease(&g_pVS);
	SafeRelease(&g_pPS);
	SafeRelease(&g_pVBuffer);
	SafeRelease(&g_pSwapchain);
	SafeRelease(&g_pRTView);
	SafeRelease(&g_pDev);
	SafeRelease(&g_pDevcon);
}

// this is the function used to render a single frame
void RenderFrame()
{
	// clear the back buffer to a deep blue
	const FLOAT clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_pDevcon->ClearRenderTargetView(g_pRTView, clearColor);

	// do 3D rendering on the back buffer here
	{
		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		g_pDevcon->IASetVertexBuffers(0, 1, &g_pVBuffer, &stride, &offset);

		// select which primtive type we are using
		g_pDevcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw the vertex buffer to the back buffer
		g_pDevcon->Draw(3, 0);
	}

	// swap the back buffer and the front buffer
	g_pSwapchain->Present(0, 0);
}


// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置程式碼。

	// 將全域字串初始化
	MyRegisterClass(hInstance);

	// 執行應用程式初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 主訊息迴圈:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"Hello Engine!";
	wcex.lpszClassName = L"Engine";
	wcex.hIconSm = nullptr;

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

	HWND hWnd = CreateWindowW(L"Engine", L"Hello Engine!", WS_OVERLAPPEDWINDOW,
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
	LRESULT result = 0;
	bool wasHandled = false;

	switch (message)
	{
	case WM_CREATE:
		wasHandled = true;
		break;

	case WM_PAINT:
		result = CreateGraphicsResources(hWnd);
		RenderFrame();
		wasHandled = true;
		break;

	case WM_SIZE:
		if (g_pSwapchain != nullptr)
		{
			DiscardGraphicsResources();
			//g_pSwapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}
		wasHandled = true;
		break;

	case WM_DESTROY:
		DiscardGraphicsResources();
		PostQuitMessage(0);
		wasHandled = true;
		break;

	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, nullptr, false);
		wasHandled = true;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	// Handle any messages the switch statement didn't
	if (!wasHandled) { result = DefWindowProc(hWnd, message, wParam, lParam); }
	return result;
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