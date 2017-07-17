#include "stdafx.h"
#include "DXApp.h"

DXApp::DXApp()
{

}

DXApp::~DXApp()
{

}

void DXApp::viewportInit()
{
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));


	RECT winSize;
	GetWindowRect(hWnd, &winSize);

	unsigned int width = winSize.right - winSize.left;
	unsigned int height = winSize.bottom - winSize.top;

	viewport.Height = height;
	viewport.Width = width;
	viewport.MaxDepth = 1;
	viewport.MinDepth = 0;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
}

void DXApp::pipelineCreation()
{
	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	D3D11_RASTERIZER_DESC raster;
	ZeroMemory(&raster, sizeof(D3D11_RASTERIZER_DESC));

	//Health Bar
	raster.AntialiasedLineEnable = true;
	raster.CullMode = D3D11_CULL_BACK;
	raster.DepthBias = 0;
	raster.DepthBiasClamp = 0.0f;
	raster.DepthClipEnable = true;
	raster.FillMode = D3D11_FILL_SOLID;
	raster.FrontCounterClockwise = false;
	raster.MultisampleEnable = false;
	raster.ScissorEnable = false;
	raster.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&raster, healthbarRasterSolid.GetAddressOf());

	//Health Outline
	raster.AntialiasedLineEnable = true;
	raster.CullMode = D3D11_CULL_BACK;
	raster.DepthBias = 0;
	raster.DepthBiasClamp = 0.0f;
	raster.DepthClipEnable = true;
	raster.FillMode = D3D11_FILL_WIREFRAME;
	raster.FrontCounterClockwise = false;
	raster.MultisampleEnable = false;
	raster.ScissorEnable = false;
	raster.SlopeScaledDepthBias = 0.0f;

	HRESULT res = S_OK;

	res = device->CreateRenderTargetView(resource, NULL, healthbarRenderTarget.GetAddressOf());
	res = device->CreateInputLayout(vertexDesc, 2, VertexShader, sizeof(VertexShader), healthbarInputLayout.GetAddressOf());
	res = device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, healthbarPShader.GetAddressOf());
	res = device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, healthbarVShader.GetAddressOf());
	res = device->CreateRasterizerState(&raster, healthbarRasterWireframe.GetAddressOf());
}



void DXApp::pipelineInit()
{
	unsigned int strides = sizeof(SIMPLE_VERTEX);
	unsigned int offset = 0;


	deviceContext->VSSetShader(healthbarVShader.Get(), NULL, 0);
	deviceContext->PSSetShader(healthbarPShader.Get(), NULL, 0);
	deviceContext->IASetInputLayout(healthbarInputLayout.Get());
}


void DXApp::hudInit()
{
	hud[0] = SIMPLE_VERTEX(XMFLOAT4(-1.0f, -0.5f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	hud[1] = SIMPLE_VERTEX(XMFLOAT4(-0.15f, -0.5f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	hud[2] = SIMPLE_VERTEX(XMFLOAT4(-0.15f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	hud[3] = SIMPLE_VERTEX(XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	hud[4] = SIMPLE_VERTEX(XMFLOAT4(-1.0f, -0.25f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	CD3D11_BUFFER_DESC hudVertexBufferDesc;
	ZeroMemory(&hudVertexBufferDesc, sizeof(hudVertexBufferDesc));
	hudVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	hudVertexBufferDesc.ByteWidth = sizeof(hud);
	hudVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hudVertexBufferDesc.CPUAccessFlags = 0;
	hudVertexBufferDesc.MiscFlags = 0;
	hudVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA hudVertexBufferData = { 0 };
	hudVertexBufferData.pSysMem = hud;
	hudVertexBufferData.SysMemPitch = 0;
	hudVertexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&hudVertexBufferDesc, &hudVertexBufferData, &hudBuffer);
}

void DXApp::bufferSetUp()
{
	RECT winSize;
	GetWindowRect(hWnd, &winSize);

	unsigned int width = winSize.right - winSize.left;
	unsigned int height = winSize.bottom - winSize.top;

	border[0] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	border[1] = SIMPLE_VERTEX(XMFLOAT4(-0.48f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	border[2] = SIMPLE_VERTEX(XMFLOAT4(-0.48f, -0.97f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	border[3] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.97f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	border[4] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));


	health[0] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));
	health[1] = SIMPLE_VERTEX(XMFLOAT4(-0.48f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));
	health[2] = SIMPLE_VERTEX(XMFLOAT4(-0.48f, -0.97f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));
	health[3] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.90f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));
	health[4] = SIMPLE_VERTEX(XMFLOAT4(-0.48f, -0.97f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));
	health[5] = SIMPLE_VERTEX(XMFLOAT4(-0.98f, -0.97f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f));


	CD3D11_BUFFER_DESC outlineVertexBufferDesc;
	ZeroMemory(&outlineVertexBufferDesc, sizeof(outlineVertexBufferDesc));
	outlineVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	outlineVertexBufferDesc.ByteWidth = sizeof(border);
	outlineVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	outlineVertexBufferDesc.CPUAccessFlags = 0;
	outlineVertexBufferDesc.MiscFlags = 0;
	outlineVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA outlineVertexBufferData = { 0 };
	outlineVertexBufferData.pSysMem = border;
	outlineVertexBufferData.SysMemPitch = 0;
	outlineVertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC inlineVertexBufferDesc;
	ZeroMemory(&inlineVertexBufferDesc, sizeof(inlineVertexBufferDesc));
	inlineVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	inlineVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	inlineVertexBufferDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 6;
	inlineVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	inlineVertexBufferDesc.MiscFlags = 0;
	inlineVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA inlineVertexBufferData = { 0 };
	inlineVertexBufferData.pSysMem = health;
	inlineVertexBufferData.SysMemPitch = 0;
	inlineVertexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&outlineVertexBufferDesc, &outlineVertexBufferData, &healthbarOutlineBuffer);
	device->CreateBuffer(&inlineVertexBufferDesc, &inlineVertexBufferData, &healthbarInlineBuffer);
}

void DXApp::swapchainSetUp()
{
	ZeroMemory(&sDesc, sizeof(sDesc));
	RECT winSize;
	GetWindowRect(hWnd, &winSize);

	unsigned int width = winSize.right - winSize.left;
	unsigned int height = winSize.bottom - winSize.top;

	sDesc.BufferCount = 1;
	sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sDesc.BufferDesc.Height = height;
	sDesc.BufferDesc.Width = width;
	sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sDesc.OutputWindow = hWnd;
	sDesc.Windowed = true;
	sDesc.SampleDesc.Count = 1;
	sDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	unsigned int flag = 0;

	D3D_FEATURE_LEVEL featureArray[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT featureArraySize = ARRAYSIZE(featureArray);
	HRESULT Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, featureArray, featureArraySize, D3D11_SDK_VERSION, &sDesc, &swapchain, &device, NULL, &deviceContext);

	swapchain->GetBuffer(0, __uuidof(resource), reinterpret_cast<void**>(&resource));

}

void DXApp::clearViewport()
{

	const float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	deviceContext->ClearRenderTargetView(healthbarRenderTarget.Get(), color);
	deviceContext->RSSetViewports(1, &viewport);
}

XMFLOAT4 DXApp::colorLerp(XMFLOAT4 colorOne, XMFLOAT4 colorTwo, float ratio)
{
	
	XMFLOAT4 newColor;

	newColor.x = colorOne.x + ratio * (colorTwo.x - colorOne.x);
	newColor.y = colorOne.y + ratio * (colorTwo.y - colorOne.y);
	newColor.z = colorOne.z + ratio * (colorTwo.z - colorOne.z);
	newColor.w = colorOne.w + ratio * (colorTwo.w - colorTwo.w);

	return newColor;
}


void DXApp::Init()
{
	swapchainSetUp();
	bufferSetUp();
	hudInit();

	viewportInit();

	pipelineCreation();
}

void DXApp::Render()
{
	unsigned int strides = sizeof(SIMPLE_VERTEX);
	unsigned int offset = 0;

	deviceContext->OMSetRenderTargets(1, healthbarRenderTarget.GetAddressOf(), NULL);
	clearViewport();

	pipelineInit();

	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	D3D11_MAPPED_SUBRESOURCE mapSub;
	deviceContext->Map(healthbarInlineBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
	memcpy(mapSub.pData, &health, sizeof(health));
	deviceContext->Unmap(healthbarInlineBuffer.Get(), NULL);


	deviceContext->RSSetState(healthbarRasterSolid.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, healthbarInlineBuffer.GetAddressOf(), &strides, &offset);
	deviceContext->Draw(6, 0);



	deviceContext->RSSetState(healthbarRasterWireframe.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	deviceContext->IASetVertexBuffers(0, 1, healthbarOutlineBuffer.GetAddressOf(), &strides, &offset);
	deviceContext->Draw(5, 0);

	deviceContext->IASetVertexBuffers(0, 1, hudBuffer.GetAddressOf(), &strides, &offset);
	deviceContext->Draw(5, 0);

	swapchain->Present(1, 0);
}

void DXApp::Update()
{
	//Get Input for Debug
	if (GetAsyncKeyState(VK_UP) & 0x1)
	{
		++currHealth;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x1)
	{
		--currHealth;
	}

	//Error Checking
	if (currHealth > 100)
	{
		currHealth = 100;
	}

	if (currHealth < 0)
	{
		currHealth = 0;
	}

	//Ratio of Health Variables
	ratio = (float)(currHealth / maxHealth);

	//If health changes, affect the health bar
	if (ratio <= 1)
	{
	
		health[1].point.x = border[0].point.x + ratio * (border[1].point.x - border[0].point.x);
		health[2].point.x = border[0].point.x + ratio * (border[1].point.x - border[0].point.x);
		health[4].point.x = border[0].point.x + ratio * (border[1].point.x - border[0].point.x);

		if (ratio > .66f)
		{
			health[0].color = colorLerp(yellow, green, ratio);
			health[1].color = colorLerp(yellow, green, ratio);
			health[2].color = colorLerp(yellow, green, ratio);
			health[3].color = colorLerp(yellow, green, ratio);
			health[4].color = colorLerp(yellow, green, ratio);
			health[5].color = colorLerp(yellow, green, ratio);
		}

		if (ratio > .34f && ratio < 0.66f)
		{
			health[0].color = colorLerp(bRed, yellow, ratio);
			health[1].color = colorLerp(bRed, yellow, ratio);
			health[2].color = colorLerp(bRed, yellow, ratio);
			health[3].color = colorLerp(bRed, yellow, ratio);
			health[4].color = colorLerp(bRed, yellow, ratio);
			health[5].color = colorLerp(bRed, yellow, ratio);
		}

		if (ratio < .35f)
		{
			health[0].color = colorLerp(dRed, bRed, ratio);
			health[1].color = colorLerp(dRed, bRed, ratio);
			health[2].color = colorLerp(dRed, bRed, ratio);
			health[3].color = colorLerp(dRed, bRed, ratio);
			health[4].color = colorLerp(dRed, bRed, ratio);
			health[5].color = colorLerp(dRed, bRed, ratio);
		}
	}
}