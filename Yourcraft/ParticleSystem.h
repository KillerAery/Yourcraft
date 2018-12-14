#pragma once
#include <Camera.h>
#include "Component.h"
#include "Effects.h"

class ParticleSystem :
	public Component
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ParticleSystem();
	~ParticleSystem();

	void Init(ID3D11Device * device, ParticleEffect * effect,
		ComPtr<ID3D11ShaderResourceView> tex2DArray, UINT maxParticles);

	void SetEyePos(const Vector3& eyePosW);
	void SetEmitPos(const Vector3& emitPosW);
	void SetEmitDir(const Vector3& emitDirW);

	void Reset();
	void Update(float dt, float gameTime);
	void Draw(ComPtr<ID3D11DeviceContext> dc, const Camera& cam);
	// Time elapsed since the system was reset.
	//float GetAge()const;
protected:
	void BuildVB(ComPtr<ID3D11Device> device);
protected:
	UINT mMaxParticles;
	bool mFirstRun;

	float mGameTime;
	float mTimeStep;
	float mAge;

	Vector3 mEyePosW;
	Vector3 mEmitPosW;
	Vector3 mEmitDirW;

	ParticleEffect* mEffect;

	ComPtr<ID3D11Buffer> mInitVB;
	ComPtr<ID3D11Buffer> mDrawVB;
	ComPtr<ID3D11Buffer> mStreamOutVB;

	ComPtr<ID3D11ShaderResourceView> mTexArraySRV;
};