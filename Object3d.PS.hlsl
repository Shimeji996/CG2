/*********************************************
float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
*********************************************/

/*********************************************
Material�͐F�ȂǎO�p�`�̕\�ʂ̍ގ������肷�����
*********************************************/
struct Material {
	float32_t4 color;
};

/*********************************************
ConstantBuffer��Shader��CPU����l��n������
register��Shader��ł�Resource�z�u���
*********************************************/
ConstantBuffer<Material>gMaterial : register(b0);

/***********************************************************************************
PixelShader�͎��ۂɉ�ʂɑł�Pixel�̐F�����߂�Shader
VertexShader�̏o�͂��Ԃ����l�Ȃǂ��g���Ȃ���Pixel�̐F������OutputMerger(�o�͌���)�Ɉ����n��
PIX�̓s�N�Z���̂���
***********************************************************************************/

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main() {
	PixelShaderOutput output;
	output.color = gMaterial.color;
	return output;
}