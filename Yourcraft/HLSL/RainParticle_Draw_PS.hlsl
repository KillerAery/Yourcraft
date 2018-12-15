#include "Particle.hlsli"
#include "RainParticle.hlsli"

float4 PS(GeoOut pin) : SV_TARGET
{
	return gTexArray.Sample(sam, float3(pin.Tex,0));
}
