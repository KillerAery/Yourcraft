#pragma once
#include "d3dUtil.h"

struct Particle
{
	Particle() : position(DirectX::XMFLOAT3(0,0,0)), velocity(DirectX::XMFLOAT3(0, 0, 0)), size(DirectX::XMFLOAT2(1,1)), age(0), type(0) {
	}

	Particle(const Particle&) = default;
	Particle& operator=(const Particle&) = default;

	Particle(Particle&&) = default;
	Particle& operator=(Particle&&) = default;

	Particle(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& velocity, DirectX::XMFLOAT2 const& size, float age, u_int type)
		: position(position), velocity(velocity), size(size), age(age), type(type)
	{ }

	Particle(DirectX::FXMVECTOR position, DirectX::FXMVECTOR velocity, DirectX::FXMVECTOR size, float age, u_int type)
		: age(age), type(type)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->velocity, velocity);
		XMStoreFloat2(&this->size, size);
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT2 size;
	float age;
	u_int type;

	static const int InputElementCount = 5;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

