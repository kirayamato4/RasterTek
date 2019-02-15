cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer CameraBuffer
{
	float4 cameraPosition;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewDirection : TEXCOORD1;
};


PixelInputType	VSMain( VertexInputType input )
{
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;

	output.position = mul( input.position, world );
	output.position = mul( output.position, view );
	output.position = mul( output.position, projection );

	output.tex = input.tex;

	output.normal = mul( input.normal, ( float3x3 )world );
	output.normal = normalize( output.normal );

	worldPosition = mul( input.position, world );
	output.viewDirection.xyz = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection.w = 1.0f;
	output.viewDirection = normalize( output.viewDirection );

	return output;
}