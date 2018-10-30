#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
    row_major matrix gWorld;
    row_major matrix gView;
    row_major matrix gProj;
    row_major matrix gWorldInvTranspose;
}

cbuffer PSConstantBuffer : register(b1)
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    Material gMaterial;
    float3 gEyePosW;
}



struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;     // �������е�λ��
    float3 NormalW : NORMAL; // �������������еķ���
    float4 Color : COLOR;
};


