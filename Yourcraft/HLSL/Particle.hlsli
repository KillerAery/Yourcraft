//***********************************************
// GLOBALS                                      *
//***********************************************

#define PT_EMITTER 0
#define PT_FLARE 1
// Array of textures for texturing the particles.
Texture2DArray gTexArray : register(t0);

// Random texture used to generate random numbers in shaders.
Texture1D gRandomTex : register(t1);

SamplerState sam : register(s0);

cbuffer cbPerFrame : register(b0)
{
	float3 gEyePosW;

	// for when the emit position/direction is varying
	float3 gEmitPosW;
	float3 gEmitDirW;
	float4x4 gViewProj;

	float gGameTime;
	float gTimeStep;
};

cbuffer cbFixed : register(b1)
{
	// Net constant acceleration used to accerlate the particles.
	float3 gAccelW;
	//粒子生命周期
	float gLifeTime;
};

//***********************************************
// HELPER FUNCTIONS                             *
//***********************************************
float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);

	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(sam, u, 0).xyz;

	// project onto unit sphere
	return normalize(v);
}

float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);

	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(sam, u, 0).xyz;

	return v;
}

//****************************************************
//  
//****************************************************

struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age		   : AGE;
	uint Type          : TYPE;
};
