#include "Light.hlsli"

// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
    // 标准化法向量
    pIn.NormalW = normalize(pIn.NormalW);

    // 顶点指向眼睛的向量
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);

    // 初始化 
    float4 ambient, diffuse, spec;
    float4 A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 反射检测
    DirectionalLight dirLight = g_DirLight;
    [flatten]
    if (g_IsReflection) {
        dirLight.Direction = mul(dirLight.Direction, (float3x3) (g_Reflection));
    }
    ComputeDirectionalLight(g_Material, g_DirLight, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    // 处理多点光源灯写了多行
    // 光源1的反射
    PointLight pointLight1 = g_PointLight0;
    [flatten]
    if (g_IsReflection)
    {
        pointLight1.Position = (float3) mul(float4(pointLight1.Position, 1.0f), g_Reflection);
    }
    ComputePointLight(g_Material, pointLight1, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    // 光源2的反射
    PointLight pointLight2 = g_PointLight1;
    [flatten]
    if (g_IsReflection)
    {
        pointLight2.Position = (float3) mul(float4(pointLight2.Position, 1.0f), g_Reflection);
    }
    ComputePointLight(g_Material, pointLight2, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    // 光源3的反射
    PointLight pointLight3 = g_PointLight2;
    [flatten]
    if (g_IsReflection)
    {
        pointLight3.Position = (float3) mul(float4(pointLight3.Position, 1.0f), g_Reflection);
    }
    ComputePointLight(g_Material, pointLight3, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    // 探照灯光源的反射
    SpotLight spotLight = g_SpotLight;
    [flatten]
    if (g_IsReflection) {
        spotLight.Position = (float3) mul(float4(spotLight.Position, 1.0f), g_Reflection);
        spotLight.Direction = mul(spotLight.Direction, (float3x3) g_Reflection);
    }
    ComputeSpotLight(g_Material, g_SpotLight, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = pIn.Color * (ambient + diffuse) + spec;
	
    litColor.a = g_Material.Diffuse.a * pIn.Color.a;
	
    //float4 texColor = g_Tex.Sample(g_SamLinear, pIn.Tex);
    //float4 litColor = texColor * (ambient + diffuse) + spec;
    //litColor.a = texColor.a * g_Material.Diffuse.a;

    return litColor;
}