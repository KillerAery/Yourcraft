
struct VertexOut
{
	float3 PosW  : POSITION;
	uint   Type  : TYPE;
};

struct GeoOut
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

