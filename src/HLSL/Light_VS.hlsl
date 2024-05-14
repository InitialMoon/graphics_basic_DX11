#include "Light.hlsli"

// 顶点着色器
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);

    float3 normalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);

    // 将镜子中的法相进行翻转
    [flatten]
    if (g_IsReflection) {
        posW = mul(posW, g_Reflection);
        normalW = mul(normalW, (float3x3) g_Reflection);
    }

    vOut.PosH = mul(posW, viewProj);
    vOut.PosW = posW.xyz;
    vOut.NormalW = normalW;
    vOut.Color = vIn.Color; // 这里alpha通道的值默认为1.0
    return vOut;
}