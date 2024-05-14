#include "Light.hlsli"

[maxvertexcount(6)]
void GS(triangle VertexOut input[3], inout TriangleStream<VertexOut> output)
{
    float mirror = 400.0f;// 镜子距离字符森林的位置，x方向

    int i;
    // 原始状态
    for (i = 0; i < 3; i++)
    {
        output.Append(input[i]);
    }
    output.RestartStrip();

    // 生成镜像字符
    for (i = 2; i >= 0; i--)
    {
        VertexOut vOut;

        // 镜面位置平移
        vOut.PosW = input[i].PosW;
        vOut.PosW.x = mirror - input[i].PosW.x;

        // 参考圆柱体绘制方式，需要在齐次裁剪坐标系中将视角重新进行计算，否则会出问题，两个镜像字符无法分离
        matrix viewProj = mul(g_View, g_Proj);
        vOut.PosH = mul(float4(vOut.PosW, 1.0f), viewProj);

        // 法向量关于yoz平面对称
        vOut.NormalW = input[i].NormalW;
        vOut.NormalW.x = -vOut.NormalW.x;

        // 颜色更改
        vOut.Color = float4(0.3f, 0.9f, 0.9f, 0.8f);

        output.Append(vOut);
    }
    output.RestartStrip();
}