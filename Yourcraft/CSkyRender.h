#pragma once

#include <vector>
#include <string>
#include "Render.h"
#include "Effects.h"
#include "Camera.h"


class CSkyRender :
	public Render
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	CSkyRender();
	~CSkyRender();
protected:
	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
	UINT mIndexCount;
	ComPtr<ID3D11ShaderResourceView> mTextureCubeSRV;
};

