#pragma once

#include <vector>
#include <string>
#include "Render.h"
#include "Effects.h"
#include "Camera.h"


class SkyRender :
	public Render
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	SkyRender();
	~SkyRender();

	// 需要提供完整的天空盒贴图 或者 已经创建好的天空盒纹理.dds文件
	void Init(ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		const std::wstring& cubemapFilename,
		float skySphereRadius,
		bool generateMips = false);

	// 需要提供天空盒的六张正方形贴图
	void Init(ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		const std::vector<std::wstring>& cubemapFilenames,
		float skySphereRadius,
		bool generateMips = false);

	void BindGameObject(GameObject * gameObject);
	void UnbindGameObject();

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect& skyEffect, const Camera& camera);
private:
	void InitResource(ComPtr<ID3D11Device> device, float skySphereRadius);
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect& skyEffect, const Camera& camera);
protected:
	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
	UINT mIndexCount;
	ComPtr<ID3D11ShaderResourceView> mTextureCubeSRV;
};

