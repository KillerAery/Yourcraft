
struct VertexOut
{
	float3 PosW  : POSITION;
	uint   Type  : TYPE;
};

struct GeoOut
{
	float4 PosH  : SV_Position;
	float2 Tex   : TEXCOORD;
};

