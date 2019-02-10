cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType VSMain( VertexInputType input )
{
	PixelInputType output;

	output.position.w = 1.0f;

	output.position = mul( input.position, world );
	output.position = mul( output.position, view );
	output.position = mul( output.position, projection );

	output.color = input.color;

	return output;
}