#pragma once
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	DX11(HWND aHWND);
	~DX11() = default;

	void Initialize(bool aDebugMode);
	void BeginFrame(float aClearColor[4]);
	void EndFrame();
	void Resize(int aNewWidth, int aNewHeight);

	static void BindRenderTarget();
	static void BindRenderTargetTexture();

#define HRASSERTLOG(aHr, aDescription) DX11::HRAssert(aHr, aDescription, true, __FILE__, __LINE__)
#define HRASSERT(aHr, aDescription) DX11::HRAssert(aHr, aDescription, false, __FILE__, __LINE__)
	static void HRAssert(HRESULT aHr, const std::string& aDescription, bool aPrint = false, const char* aFile = nullptr, int aLine = 0);

	static ComPtr<ID3D11Device> ourDevice;
	static ComPtr<ID3D11DeviceContext> ourContext;
	static ComPtr<IDXGISwapChain> ourSwapChain;
	static ComPtr<ID3D11RenderTargetView> ourBackBuffer;
	static ComPtr<ID3D11DepthStencilView> ourDepthBuffer;

	// Render Target View bound to a texture, used by ImGui to render Scene Tab.
	static ComPtr<ID3D11ShaderResourceView> ourTextureSRV;
	static ComPtr<ID3D11RenderTargetView> ourTextureBuffer;
	static ComPtr<ID3D11Texture2D> ourTexture;

private:
	void CreateRenderTargetView();
	// Used by ImGui to render Scene Tab.
	void CreateSceneTextureResources();
	void CreateDepth();
	void CreateDepthTexture();
	void SetViewPort();
	void SetPrimitiveTopology();
	void SetAndCreateVertexShader(std::string aShaderFileName);
	void SetAndCreatePixelShader(std::string aShaderFileName);

	HWND myHWND;

	int myWidth;
	int myHeight;
};

