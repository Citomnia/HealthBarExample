//cbuffer MatrixBuffer : register(b0)
//{
//	matrix world;
//	matrix view;
//	matrix ortho;
//};

struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	sendToRasterizer.projectedCoordinate.w = 1;

	sendToRasterizer.projectedCoordinate.xy = fromVertexBuffer.coordinate.xy;
	sendToRasterizer.projectedCoordinate.z = 0;

	sendToRasterizer.colorOut = fromVertexBuffer.color;

	return sendToRasterizer;
}