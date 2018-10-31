#pragma once
#include "Component.h"
#include "Effects.h"

class Render :
	public Component
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Render();
	virtual ~Render() = 0;
	virtual void Update() = 0;
protected:
	UINT vertexCount;
	UINT indexCount;
	DXGI_FORMAT indexFormat;
};

