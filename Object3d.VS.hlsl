/*****************************************************
float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}
*****************************************************/

/***************************************************************
ShaderとはGPU上で動くプログラムでレンダリングパイプラインの中核をなす機能
VertexShaderは頂点に対して加工を行うShader
入力された頂点座標を同次クリップ空間に変換して出力しラスタライザに引き渡す
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