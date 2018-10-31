#ifndef D3DUTIL_H
#define D3DUTIL_H

#include <d3d11_1.h>			// �Ѱ���Windows.h
#include <DirectXCollision.h>	// �Ѱ���DirectXMath.h
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <DDSTextureLoader.h>	
#include <WICTextureLoader.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <filesystem>
#include <SimpleMath.h>
#include "dxerr.h"

// ��ֲ�����Ĵ����飬����Ŀ������ʹ��Unicode�ַ���
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
{                                                          \
	HRESULT hr = (x);                                      \
	if(FAILED(hr))                                         \
	{                                                      \
		DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
	}                                                      \
}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 
#endif

//
// ��ɫ��������غ���
//

HRESULT CreateShaderFromFile(const WCHAR * objFileNameInOut, const WCHAR * hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** ppBlobOut);


//
// ����������غ���
//

// ���ݸ�����DDS�����ļ����ϣ�����2D��������
// Ҫ����������Ŀ�Ⱥ͸߶ȶ�һ��
// ��maxMipMapSizeΪ0��ʹ��Ĭ��mipmap�ȼ�
// ����mipmap�ȼ������ᳬ��maxMipMapSize
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateDDSTexture2DArrayFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	const std::vector<std::wstring>& filenames,
	UINT maxMipMapSize = 0);



//
// ������������غ���
//

// ���ݸ�����һ�Ű����������������������������������
// Ҫ�������߱�Ϊ4:3���Ұ�������ʽ����:
// .  +Y .  .
// -X +Z +X -Z 
// .  -Y .  .
// �ú���Ĭ�ϲ�����mipmap(���ȼ���Ϊ1)������Ҫ������generateMipsΪtrue
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateWICTextureCubeFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	std::wstring cubemapFileName,
	bool generateMips = false);

// ���ݰ�D3D11_TEXTURECUBE_FACE����˳�����������������������������
// Ҫ��������ͬ����С��������
// �ú���Ĭ�ϲ�����mipmap(���ȼ���Ϊ1)������Ҫ������generateMipsΪtrue
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateWICTextureCubeFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	std::vector<std::wstring> cubemapFileNames,
	bool generateMips = false);

#endif
