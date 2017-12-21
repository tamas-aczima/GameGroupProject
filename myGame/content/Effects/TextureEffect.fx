/************* Resources *************/

#define FLIP_TEXTURE_Y 0

cbuffer CBufferPerFrame
{
	float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
}

cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLVIEWPROJECTION;
}

Texture2D ColorTexture <
	string ResourceName = "Content\\Textures\\earth.jpg";
	string UIName = "Color Texture";
	string ResourceType = "2D";
>;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

/************* Data Structures *************/

struct VS_INPUT
{
	float4 ObjectPosition : POSITION;
	float2 TextureCoordinate: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinate: TEXCOORD;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
};

/*** Utility Functions ***/

float2 get_corrected_texture_coordinate(float2 textureCoordinate)
{
#if FLIP_TEXTURE_Y
	return float2(textureCoordinate.x, 1.0 - textureCoordinate.y);
#else
	return textureCoordinate;
#endif
}

float3 get_vector_color_contribution(float4 light, float3 color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
	OUT.TextureCoordinate = get_corrected_texture_coordinate(IN.TextureCoordinate);

	return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 OUT = (float4)0;

	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	float3 ambient = get_vector_color_contribution(AmbientColor, color.rgb);


	OUT.rgb = ambient;
	OUT.a = AmbientColor.a;

	return OUT;
}

/************* Techniques *************/

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

		SetRasterizerState(DisableCulling);
	}
};