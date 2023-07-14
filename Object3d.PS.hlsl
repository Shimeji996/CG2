/*********************************************
float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
*********************************************/

/*********************************************
Materialは色など三角形の表面の材質を決定するもの
*********************************************/
struct Material {
	float32_t4 color;
};

/*********************************************
ConstantBufferはShaderにCPUから値を渡すもの
registerはShader上でのResource配置情報
*********************************************/
ConstantBuffer<Material>gMaterial : register(b0);

/***********************************************************************************
PixelShaderは実際に画面に打つPixelの色を決めるShader
VertexShaderの出力を補間した値などを使いながらPixelの色を決めOutputMerger(出力結合)に引き渡す
PIXはピクセルのこと
***********************************************************************************/

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main() {
	PixelShaderOutput output;
	output.color = gMaterial.color;
	return output;
}