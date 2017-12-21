//#include "include\\Common.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
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
}

Texture2D ColorTexture;
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
    float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float2 TextureCoordinate : TEXCOORD0;
	float4 worldPos : Position;
};

/************* Vertex Shader *************/

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;

	output.Position = mul(inPos, WorldViewProjection);
	output.worldPos = mul(inPos, World);

	output.Normal = mul(normal, World);

	output.TextureCoordinate = inTexCoord;

	return output;
}

//VS_OUTPUT vertex_shader(VS_INPUT IN)
//{
//	VS_OUTPUT OUT = (VS_OUTPUT)0;
//	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
//	OUT.TextureCoordinate = get_corrected_texture_coordinate(IN.TextureCoordinate);
//	OUT.Normal = normalize(mul(float4(IN.Normal, 0), World).xyz);
//	OUT.LightDirection = normalize(-LightDirection);
//	OUT.LightDirection2 = normalize(-LightDirection2);
//	return OUT;
//}

/************* Pixel Shader *************/

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);

	float4 diffuseTexture = ColorTexture.Sample(ColorSampler, input.TextureCoordinate);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	//Create the vector between light position and pixels position
	float3 lightToPixelVec = pos - input.worldPos;

	//Find the distance between the light pos and pixel pos
	float d = length(lightToPixelVec);

	//Add the ambient light
	float3 finalAmbient = diffuseTexture * ambient;

	//If pixel is too far, return pixel color with ambient light
	if (d > range)
	return float4(finalAmbient, diffuseTexture.a);

	//Turn lightToPixelVec into a unit length vector describing
	//the pixels direction from the lights position
	lightToPixelVec /= d;

	//Calculate how much light the pixel gets by the angle
	//in which the light strikes the pixels surface
	float howMuchLight = dot(lightToPixelVec, input.Normal);

	//If light is striking the front side of the pixel
	if (howMuchLight > 0.0f)
	{
		//Add light to the finalColor of the pixel
		finalColor += diffuseTexture * diffuse;

		//Calculate Light's Distance Falloff factor
		finalColor /= (att[0] + (att[1] * d)) + (att[2] * (d*d));

		//Calculate falloff from center to edge of pointlight cone
		finalColor *= pow(max(dot(-lightToPixelVec, dir), 0.0f), cone);
	}

	//make sure the values are between 1 and 0, and add the ambient
	finalColor = saturate(finalColor + finalAmbient);

	//Return Final Color
	return float4(finalColor, diffuse.a);
}

//float4 pixel_shader(VS_OUTPUT IN) : SV_Target
//{
	/*float4 OUT = (float4)0;
	float3 normal = normalize(IN.Normal);
	float3 lightDirection = normalize(IN.LightDirection);
	float n_dot_l = dot(lightDirection, normal);

	float3 lightDirection2 = normalize(IN.LightDirection2);
	float n_dot_2 = dot(lightDirection2, normal);

	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	float3 ambient = AmbientColor.rgb * AmbientColor.a * color.rgb;
	float3 diffuse = (float3)0;
	float3 diffuse2 = (float3)0;
	if (n_dot_l > 0)
	{
		diffuse = LightColor.rgb * LightColor.a * n_dot_l * color.rgb;
	}
	if (n_dot_2 > 0)
	{
		diffuse2 = LightColor2.rgb * LightColor2.a * n_dot_2 * color.rgb;
	}
	OUT.rgb = ambient + diffuse + diffuse2;
	OUT.a = color.a;
	return OUT;*/



	//IN.Normal = normalize(IN.Normal);
	//
	//float4 diffuse = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	//
	//float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	//
	////Create the vector between light position and pixels position
	//float3 lightToPixelVec = IN.Position - IN.worldPos;
	//
	////Find the distance between the light pos and pixel pos
	//float d = length(lightToPixelVec);
	//
	////Add the ambient light
	//float3 finalAmbient = diffuse * AmbientColor;
	//
	////If pixel is too far, return pixel color with ambient light
	///*if (d > range)
	//return float4(finalAmbient, diffuse.a);*/
	//
	////Turn lightToPixelVec into a unit length vector describing
	////the pixels direction from the lights position
	//lightToPixelVec /= d;
	//
	////Calculate how much light the pixel gets by the angle
	////in which the light strikes the pixels surface
	//float howMuchLight = dot(lightToPixelVec, IN.Normal);
	//
	////If light is striking the front side of the pixel
	//if (howMuchLight > 0.0f)
	//{
	//	//Add light to the finalColor of the pixel
	//	finalColor += diffuse * diffuse;
	//
	//	//Calculate Light's Distance Falloff factor
	//	//finalColor /= (a + (att[1] * d)) + (att[2] * (d*d));
	//
	//	//Calculate falloff from center to edge of pointlight cone
	//	//finalColor *= pow(max(dot(-lightToPixelVec, dir), 0.0f), cone);
	//}
	//
	////make sure the values are between 1 and 0, and add the ambient
	//finalColor = saturate(finalColor + finalAmbient);
	//
	////Return Final Color
	//return float4(finalColor, diffuse.a);

//}

/************* Techniques *************/

technique11 main11
{
    pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}