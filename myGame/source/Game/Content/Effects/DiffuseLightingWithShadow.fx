//#include "include\\Common.fxh"

/************* Resources *************/
static const float4 ColorWhite = { 1, 1, 1, 1 };
static const float3 ColorBlack = { 0, 0, 0 };
static const float DepthBias = 0.005;

cbuffer CBufferPerFrame
{
	float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float3 LightPosition = { 0.0f, 0.0f, 0.0f };
	float LightRadius = 10.0f;
	float3 CameraPosition;
	float2 ShadowMapSize = { 1024.0f, 1024.0f };

	float3 pos;
	float range;
	float3 dir;
	float cone;
	float3 att;
	float4 ambient;
	float4 diffuse;
}

cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
	float4x4 World : WORLD;
	float4 SpecularColor : SPECULAR = { 1.0f, 1.0f, 1.0f, 1.0f };
	float SpecularPower : SPECULARPOWER = 25.0f;

	float4x4 ProjectiveTextureMatrix;
}

Texture2D ColorTexture;
Texture2D ShadowMap;

SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = ColorWhite;
};

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

RasterizerState BackFaceCulling
{
	CullMode = BACK;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
    float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float2 TextureCoordinate : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float Attenuation : TEXCOORD2;
	float4 ShadowTextureCoordinate : TEXCOORD3;
};

/************* Utility Functions *************/

float3 get_vector_color_contribution(float4 light, float3 color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

float3 get_scalar_color_contribution(float4 light, float color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

/************* Vertex Shader *************/

VS_OUTPUT VS(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
	OUT.WorldPosition = mul(IN.ObjectPosition, World).xyz;
	OUT.TextureCoordinate = IN.TextureCoordinate;
	OUT.Normal = normalize(mul(float4(IN.Normal, 0), World).xyz);

	float3 lightDirection = LightPosition - OUT.WorldPosition;
	OUT.Attenuation = saturate(1.0f - (length(lightDirection) / LightRadius));

	OUT.ShadowTextureCoordinate = mul(IN.ObjectPosition, ProjectiveTextureMatrix);

	return OUT;
}

/************* Pixel Shader *************/

float4 shadow_pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 OUT = (float4)0;

	float3 lightDirection = LightPosition - IN.WorldPosition;
	lightDirection = normalize(lightDirection);

	float3 viewDirection = normalize(CameraPosition - IN.WorldPosition);

	float3 normal = normalize(IN.Normal);
	float n_dot_l = dot(normal, lightDirection);
	float3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	float4 lightCoefficients = lit(n_dot_l, n_dot_h, SpecularPower);

	float3 ambient = get_vector_color_contribution(AmbientColor, color.rgb);
	float3 diffuse = get_vector_color_contribution(LightColor, lightCoefficients.y * color.rgb) * IN.Attenuation;
	float3 specular = get_scalar_color_contribution(SpecularColor, min(lightCoefficients.z, color.w)) * IN.Attenuation;

	if (IN.ShadowTextureCoordinate.w >= 0.0f)
	{
		IN.ShadowTextureCoordinate.xyz /= IN.ShadowTextureCoordinate.w;
		float pixelDepth = IN.ShadowTextureCoordinate.z;
		float sampledDepth = ShadowMap.Sample(ShadowMapSampler, IN.ShadowTextureCoordinate.xy).x + DepthBias;

		// Shadow applied in a boolean fashion -- either in shadow or not
		float3 shadow = (pixelDepth > sampledDepth ? ColorBlack : ColorWhite.rgb);
		diffuse *= shadow;
		specular *= shadow;
	}

	OUT.rgb = ambient + diffuse + specular;
	OUT.a = 1.0f;

	return OUT;
}

//float4 PS(VS_OUTPUT input) : SV_TARGET
//{
//	input.Normal = normalize(input.Normal);
//
//	float4 diffuseTexture = ColorTexture.Sample(ColorSampler, input.TextureCoordinate);
//
//	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
//
//	//Create the vector between light position and pixels position
//	float3 lightToPixelVec = pos - input.worldPos;
//
//	//Find the distance between the light pos and pixel pos
//	float d = length(lightToPixelVec);
//
//	//Add the ambient light
//	float3 finalAmbient = diffuseTexture * ambient;
//
//	//If pixel is too far, return pixel color with ambient light
//	if (d > range)
//	return float4(finalAmbient, diffuseTexture.a);
//
//	//Turn lightToPixelVec into a unit length vector describing
//	//the pixels direction from the lights position
//	lightToPixelVec /= d;
//
//	//Calculate how much light the pixel gets by the angle
//	//in which the light strikes the pixels surface
//	float howMuchLight = dot(lightToPixelVec, input.Normal);
//
//	//If light is striking the front side of the pixel
//	if (howMuchLight > 0.0f)
//	{
//		//Add light to the finalColor of the pixel
//		finalColor += diffuseTexture * diffuse;
//
//		//Calculate Light's Distance Falloff factor
//		finalColor /= (att[0] + (att[1] * d)) + (att[2] * (d*d));
//
//		//Calculate falloff from center to edge of pointlight cone
//		finalColor *= pow(max(dot(-lightToPixelVec, dir), 0.0f), cone);
//	}
//
//	//make sure the values are between 1 and 0, and add the ambient
//	finalColor = saturate(finalColor + finalAmbient);
//
//	//Return Final Color
//	return float4(finalColor, diffuse.a);
//}



/************* Techniques *************/

technique11 shadow_mapping
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, shadow_pixel_shader()));

		SetRasterizerState(BackFaceCulling);
	}
}