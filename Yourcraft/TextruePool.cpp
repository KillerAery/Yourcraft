#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include "d3dUtil.h"
#include "TextruePool.h"

TextruePool::TextruePool()
{
}


TextruePool::~TextruePool()
{
}

void TextruePool::LoadTextrue(const std::wstring& str, ComPtr<ID3D11Device> device)
{		
	auto itr = mTextrues.emplace(str, ComPtr<ID3D11ShaderResourceView>());
	auto textruePtr = itr.first->second;

	if (str.substr(str.size() - 3, 3) == L"dds")
	{
		HR(DirectX::CreateDDSTextureFromFile(device.Get(), str.c_str(), nullptr,
			textruePtr.GetAddressOf()));
	}
	else if (!str.empty())
	{
		HR(DirectX::CreateWICTextureFromFile(device.Get(), str.c_str(), nullptr,
			textruePtr.GetAddressOf()));
	}
}
