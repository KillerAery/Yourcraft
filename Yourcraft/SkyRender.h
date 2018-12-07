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

	// ��Ҫ�ṩ��������պ���ͼ ���� �Ѿ������õ���պ�����.dds�ļ�
	void Init(ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		const std::wstring& cubemapFilename,
		float skySphereRadius,
		bool generateMips = false);

	// ��Ҫ�ṩ��պе�������������ͼ
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

