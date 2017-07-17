#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <iostream>
#include "DDSTextureLoader.h"

#include "VertexShader.csh"
#include "PixelShader.csh"

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")

class DXApp
{
public:
	struct SIMPLE_VERTEX
	{
		XMFLOAT4 point;
		XMFLOAT4 color;

		SIMPLE_VERTEX() {};
		SIMPLE_VERTEX(XMFLOAT4 newPoint, XMFLOAT4 newColor)
		{
			point = newPoint;
			color = newColor;
		}
	};


	Microsoft::WRL::ComPtr<ID3D11InputLayout> healthbarInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> healthbarVShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> healthbarPShader;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> healthbarRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Buffer> healthbarBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> healthbarSRV;

	Microsoft::WRL::ComPtr<ID3D11Buffer> healthbarOutlineBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> healthbarInlineBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> healthbarBackgroundBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> hudBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	IDXGISwapChain *swapchain;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	ID3D11Resource *resource;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> healthbarRasterWireframe, healthbarRasterSolid;

	D3D11_VIEWPORT viewport;
	DXGI_SWAP_CHAIN_DESC sDesc;
	D3D11_SUBRESOURCE_DATA data;

	//window variables
	HWND hWnd;
	unsigned int bufferWidth = 800;
	unsigned int bufferHeight = 600;


	//Bar Variables
	SIMPLE_VERTEX border[5];
	SIMPLE_VERTEX health[6];

	//color variables
	XMFLOAT4 green = { 0.2f, 0.8f, 0.2f, 1.0f };
	XMFLOAT4 yellow = { 0.95f, 0.95f, 0.05f, 1.0f };
	XMFLOAT4 bRed = { 1.0f, 0, 0, 1.0f };
	XMFLOAT4 dRed = { .33f, 0, 0, 1.0f };

	//Variables 
	float maxHealth = 100;
	float currHealth = maxHealth;
	float ratio;

	//HUD variables
	SIMPLE_VERTEX hud[5];

	void viewportInit();
	void pipelineCreation();
	void pipelineInit();
	void bufferSetUp();
	void swapchainSetUp();
	void clearViewport();
	XMFLOAT4 colorLerp(XMFLOAT4 colorOne, XMFLOAT4 colorTwo, float ratio);

	void hudInit();

	void Init();
	void Update();
	void Render();

	DXApp();
	~DXApp();
};

