#include "Light.hlsli"

// ������ɫ��
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);

    float3 normalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);

    // �������еķ�����з�ת
    [flatten]
    if (g_IsReflection) {
        posW = mul(posW, g_Reflection);
        normalW = mul(normalW, (float3x3) g_Reflection);
    }

    vOut.PosH = mul(posW, viewProj);
    vOut.PosW = posW.xyz;
    vOut.NormalW = normalW;
    vOut.Color = vIn.Color; // ����alphaͨ����ֵĬ��Ϊ1.0
    return vOut;
}