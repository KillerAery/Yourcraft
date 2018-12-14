#include "Particle.hlsli"
#include "RainParticle.hlsli"

VertexOut VS(Particle vin)
{
	VertexOut vout;

	float t = vin.Age;

	// constant acceleration equation
	vout.PosW = 0.5f*t*t*gAccelW + t * vin.InitialVelW + vin.InitialPosW;

	vout.Type = vin.Type;

	return vout;
}
