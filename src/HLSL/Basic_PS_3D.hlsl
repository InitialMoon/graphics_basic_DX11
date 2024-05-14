// ����
#include "Basic.hlsli"

// ������ɫ��(3D)
float4 PS_3D(VertexPosHWNormalTex pIn) : SV_Target
{
    // ��׼��������
    pIn.NormalW = normalize(pIn.NormalW);

    // ����ָ���۾�������
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);

    
    // ��ʼ��Ϊ0 
    float4 ambient, diffuse, spec;
    float4 A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // ����
    DirectionalLight dirLight = g_DirLight;
    [flatten]
    if (g_IsReflection)
    {
        dirLight.Direction = mul(dirLight.Direction, (float3x3) (g_Reflection));
    }
    ComputeDirectionalLight(g_Material, g_DirLight, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

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
    
    PointLight pointLight2 = g_PointLight1;
    [flatten]
    if (g_IsReflection) {
        pointLight2.Position = (float3) mul(float4(pointLight2.Position, 1.0f), g_Reflection);
    }
    ComputePointLight(g_Material, pointLight2, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    PointLight pointLight3 = g_PointLight2;
    [flatten]
    if (g_IsReflection) {
        pointLight3.Position = (float3) mul(float4(pointLight3.Position, 1.0f), g_Reflection);
    }
    ComputePointLight(g_Material, pointLight3, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    SpotLight spotLight = g_SpotLight;
    [flatten]
    if (g_IsReflection) {
        spotLight.Position = (float3) mul(float4(spotLight.Position, 1.0f), g_Reflection);
        spotLight.Direction = mul(spotLight.Direction, (float3x3) g_Reflection);
    }
    ComputeSpotLight(g_Material, spotLight, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 texColor = g_Tex.Sample(g_SamLinear, pIn.Tex);
    float4 litColor = texColor * (ambient + diffuse) + spec;
    litColor.a = texColor.a * g_Material.Diffuse.a;
	
    return litColor;
}