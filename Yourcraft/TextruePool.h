#pragma once

#include <string>
#include <map>
#include <d3d11.h>
#include <wrl/client.h>


class TextruePool
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	TextruePool();
	~TextruePool();

	std::map<std::wstring, ComPtr<ID3D11ShaderResourceView>> mTextrues;

	void LoadTextrue(const std::wstring & str,ComPtr<ID3D11Device> device);
	void UnloadTextrue(const std::wstring & str, ComPtr<ID3D11Device> device);
};

