/*****************************************************
float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}
*****************************************************/

/***************************************************************
Shader�Ƃ�GPU��œ����v���O�����Ń����_�����O�p�C�v���C���̒��j���Ȃ��@�\
VertexShader�͒��_�ɑ΂��ĉ��H���s��Shader
���͂��ꂽ���_���W�𓯎��N���b�v��Ԃɕϊ����ďo�͂����X�^���C�U�Ɉ����n��
***************************************************************/
struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
};

struct VertexShaderInput {
	float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = input.position;
	return output;
}