#include "Particle.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	//float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	output.color = gMaterial.color * textureColor * input.color;

	if (output.color.a == 0.0) {
		discard;
	}

	output.color = gMaterial.color * textureColor * input.color;

	return output;
}