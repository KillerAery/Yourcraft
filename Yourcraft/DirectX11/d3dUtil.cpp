#include "d3dUtil.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std::experimental;

HRESULT CreateShaderFromFile(const WCHAR * objFileNameInOut, const WCHAR * hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	// Ѱ���Ƿ����Ѿ�����õĶ�����ɫ��
	if (objFileNameInOut && filesystem::exists(objFileNameInOut))
	{
		HR(D3DReadFileToBlob(objFileNameInOut, ppBlobOut));
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// ���� D3DCOMPILE_DEBUG ��־���ڻ�ȡ��ɫ��������Ϣ���ñ�־���������������飬
		// ����Ȼ������ɫ�������Ż�����
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// ��Debug�����½����Ż��Ա������һЩ����������
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
			dwShaderFlags, 0, ppBlobOut, errorBlob.GetAddressOf());
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			return hr;
		}

		// ��ָ��������ļ���������ɫ����������Ϣ���
		if (objFileNameInOut)
		{
			HR(D3DWriteBlobToFile(*ppBlobOut, objFileNameInOut, FALSE));
		}
	}

	return hr;
}

ComPtr<ID3D11ShaderResourceView> CreateDDSTexture2DArrayFromFile(
	ComPtr<ID3D11Device> device,
	ComPtr<ID3D11DeviceContext> deviceContext,
	const std::vector<std::wstring>& filenames,
	UINT maxMipMapSize)
{
	// ����豸���豸�������Ƿ�ǿ�
	if (!device || !deviceContext)
		return nullptr;

	// ******************
	// 1. ��ȡ��������
	//
	UINT size = (UINT)filenames.size();
	UINT mipLevel = maxMipMapSize;
	std::vector<ComPtr<ID3D11Texture2D>> srcTex(size);
	UINT width, height;
	DXGI_FORMAT format;
	for (size_t i = 0; i < size; ++i)
	{
		// ������Щ�������ᱻGPUʹ�ã�����ʹ��D3D11_USAGE_STAGINGö��ֵ
		// ʹ��CPU���Զ�ȡ��Դ
		HR(CreateDDSTextureFromFileEx(device.Get(),
			deviceContext.Get(),
			filenames[i].c_str(),
			maxMipMapSize,
			D3D11_USAGE_STAGING,							// Usage
			0,												// BindFlags
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,	// CpuAccessFlags
			0,												// MiscFlags
			false,
			(ID3D11Resource**)srcTex[i].GetAddressOf(),
			nullptr));

		// ��ȡ�����õ�����Mipmap�ȼ�, ��Ⱥ͸߶�
		D3D11_TEXTURE2D_DESC texDesc;
		srcTex[i]->GetDesc(&texDesc);
		if (i == 0)
		{
			mipLevel = texDesc.MipLevels;
			width = texDesc.Width;
			height = texDesc.Height;
			format = texDesc.Format;
		}
		// ����������������MipMap�ȼ�����Ⱥ͸߶�Ӧ��һ��
		assert(mipLevel == texDesc.MipLevels);
		assert(texDesc.Width == width && texDesc.Height == height);
		// ����Ҫ�������ṩ��ͼƬ���ݸ�ʽӦ����һ�µģ������ڲ�һ�µ��������
		// ʹ��dxtex.exe(DirectX Texture Tool)�����е�ͼƬת��һ�µ����ݸ�ʽ
		assert(texDesc.Format == format);

	}

	// ******************
	// 2.������������
	//
	D3D11_TEXTURE2D_DESC texDesc, texArrayDesc;
	srcTex[0]->GetDesc(&texDesc);
	texArrayDesc.Width = texDesc.Width;
	texArrayDesc.Height = texDesc.Height;
	texArrayDesc.MipLevels = texDesc.MipLevels;
	texArrayDesc.ArraySize = size;
	texArrayDesc.Format = texDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;		// ����ʹ�ö��ز���
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texArray;
	HR(device->CreateTexture2D(&texArrayDesc, nullptr, texArray.GetAddressOf()));

	// ******************
	// 3.�����е���������Դ��ֵ������������
	//

	// ÿ������Ԫ��
	for (UINT i = 0; i < size; ++i)
	{
		// �����е�ÿ��mipmap�ȼ�
		for (UINT j = 0; j < mipLevel; ++j)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			// ����ӳ������i�����У�����j��mipmap�ȼ���2D����
			HR(deviceContext->Map(srcTex[i].Get(),
				j, D3D11_MAP_READ, 0, &mappedTex2D));

			deviceContext->UpdateSubresource(
				texArray.Get(),
				D3D11CalcSubresource(j, i, mipLevel),	// i * mipLevel + j
				nullptr,
				mappedTex2D.pData,
				mappedTex2D.RowPitch,
				mappedTex2D.DepthPitch);
			// ֹͣӳ��
			deviceContext->Unmap(srcTex[i].Get(), j);
		}
	}

	// ******************
	// 4.�������������SRV
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	ComPtr<ID3D11ShaderResourceView> texArraySRV;
	HR(device->CreateShaderResourceView(texArray.Get(), &viewDesc, texArraySRV.GetAddressOf()));


	// �Ѿ�ȷ��������Դ��ComPtr���������ֶ��ͷ�

	return texArraySRV;
}