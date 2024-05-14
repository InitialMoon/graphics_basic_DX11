#include "Light.hlsli"

[maxvertexcount(6)]
void GS(triangle VertexOut input[3], inout TriangleStream<VertexOut> output)
{
    float mirror = 400.0f;// ���Ӿ����ַ�ɭ�ֵ�λ�ã�x����

    int i;
    // ԭʼ״̬
    for (i = 0; i < 3; i++)
    {
        output.Append(input[i]);
    }
    output.RestartStrip();

    // ���ɾ����ַ�
    for (i = 2; i >= 0; i--)
    {
        VertexOut vOut;

        // ����λ��ƽ��
        vOut.PosW = input[i].PosW;
        vOut.PosW.x = mirror - input[i].PosW.x;

        // �ο�Բ������Ʒ�ʽ����Ҫ����βü�����ϵ�н��ӽ����½��м��㣬���������⣬���������ַ��޷�����
        matrix viewProj = mul(g_View, g_Proj);
        vOut.PosH = mul(float4(vOut.PosW, 1.0f), viewProj);

        // ����������yozƽ��Գ�
        vOut.NormalW = input[i].NormalW;
        vOut.NormalW.x = -vOut.NormalW.x;

        // ��ɫ����
        vOut.Color = float4(0.3f, 0.9f, 0.9f, 0.8f);

        output.Append(vOut);
    }
    output.RestartStrip();
}