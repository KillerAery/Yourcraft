#include "Particle.hlsli"
#include "RainParticle.hlsli"

// The draw GS just expands points into lines.
[maxvertexcount(2)]
void GS(point VertexOut gin[1],
	inout LineStream<GeoOut> lineStream)
{
	// do not draw emitter particles.
	if (gin[0].Type != PT_EMITTER)
	{
		// Slant line in acceleration direction.
		float3 p0 = gin[0].PosW;
		float3 p1 = gin[0].PosW + 0.07f * gAccelW;

		GeoOut v0;
		v0.PosH = mul(float4(p0, 1.0f), gViewProj);
		v0.Tex = float2(0.0f, 0.0f);
		lineStream.Append(v0);

		GeoOut v1;
		v1.PosH = mul(float4(p1, 1.0f), gViewProj);
		v1.Tex = float2(1.0f, 1.0f);
		lineStream.Append(v1);
	}
}