#pragma once
#include "Component.h"
#include "LightHelper.h"
#include <wrl/client.h>
#include <WICTextureLoader.h>


class Render :
	public Component
{
public:
	Render();
	virtual ~Render() = 0;
	virtual void Update() = 0;
protected:
	Material material;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texA;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texD;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	UINT vertexCount;
	UINT indexCount;
	DXGI_FORMAT indexFormat;
};

