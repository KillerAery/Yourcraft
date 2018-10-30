#ifndef EFFECTS_H
#define EFFECTS_H
#include <memory>
#include "LightHelper.h"
#include "RenderStates.h"


class IEffect
{
public:
	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	IEffect() = default;

	// ��֧�ָ��ƹ���
	IEffect(const IEffect&) = delete;
	IEffect& operator=(const IEffect&) = delete;

	// ����ת��
	IEffect(IEffect&& moveFrom) = default;
	IEffect& operator=(IEffect&& moveFrom) = default;

	virtual ~IEffect() = default;

	// ���²��󶨳���������
	virtual void Apply(ComPtr<ID3D11DeviceContext> deviceContext) = 0;
};


class BasicFX : public IEffect
{
public:

	enum RenderType { RenderObject, RenderInstance };

	BasicFX();
	virtual ~BasicFX() override;

	BasicFX(BasicFX&& moveFrom);
	BasicFX& operator=(BasicFX&& moveFrom);

	// ��ȡ����
	static BasicFX& Get();

	

	// ��ʼ��Basic.fx������Դ����ʼ����Ⱦ״̬
	bool InitAll(ComPtr<ID3D11Device> device);


	// 
	// ��Ⱦģʽ�ı��
	//

	// Ĭ��״̬������
	void SetRenderDefault(ComPtr<ID3D11DeviceContext> deviceContext, RenderType type);
	
	//
	// ��������
	//

	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W);
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V);
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P);
	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P);

	//
	// ���ա����ʺ������������
	//

	// �������͵ƹ�����������Ŀ
	static const int maxLights = 5;

	void SetDirLight(size_t pos, const DirectionalLight& dirLight);
	void SetPointLight(size_t pos, const PointLight& pointLight);
	void SetSpotLight(size_t pos, const SpotLight& spotLight);

	void SetMaterial(const Material& material);


	void SetTextureUsed(bool isUsed);

	void SetTextureAmbient(ComPtr<ID3D11ShaderResourceView> texture);
	void SetTextureDiffuse(ComPtr<ID3D11ShaderResourceView> texture);
	

	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);
	

	// Ӧ�ó�����������������Դ�ı��
	void Apply(ComPtr<ID3D11DeviceContext> deviceContext);
	
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};










#endif