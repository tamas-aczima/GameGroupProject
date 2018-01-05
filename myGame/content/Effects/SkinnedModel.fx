#define MaxBones 100

/************* Constants *************/

#define FLIP_TEXTURE_Y 0

/************* Data Structures *************/

struct DIRECTIONAL_LIGHT
{
	float3 Direction;
	float4 Color;
};

struct POINT_LIGHT
{
	float3 Position : POSITION;
	float LightRadius;
	float4 Color : COLOR;
};

struct SPOT_LIGHT
{
	float3 Position;
	float4 Direction;
	float OuterAngle;
	float InnerAngle;
	float LightRadius;
	float4 Color;
};

struct LIGHT_CONTRIBUTION_DATA
{
	float4 Color;
	float3 Normal;
	float3 ViewDirection;
	float4 LightColor;
	float4 LightDirection;
	float4 SpecularColor;
	float SpecularPower;
};


/************* Utility Functions *************/

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

float3 get_scalar_color_contribution(float4 light, float color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

float4 get_light_data(float3 lightPosition, float3 worldPosition, float lightRadius)
{
	float4 lightData;
	float3 lightDirection = lightPosition - worldPosition;

	lightData.xyz = normalize(lightDirection);
	lightData.w = saturate(1.0f - length(lightDirection) / lightRadius); // Attenuation

	return lightData;
}

float3 get_light_contribution(LIGHT_CONTRIBUTION_DATA IN)
{
	float3 lightDirection = IN.LightDirection.xyz;
	float n_dot_l = dot(IN.Normal, lightDirection);
	float3 halfVector = normalize(lightDirection + IN.ViewDirection);
	float n_dot_h = dot(IN.Normal, halfVector);

	float4 lightCoefficients = lit(n_dot_l, n_dot_h, IN.SpecularPower);
	float3 diffuse = get_vector_color_contribution(IN.LightColor, lightCoefficients.y * IN.Color.rgb) * IN.LightDirection.w;
	float3 specular = get_scalar_color_contribution(IN.SpecularColor, min(lightCoefficients.z, IN.Color.w)) * IN.LightDirection.w;

	return (diffuse + specular);
}

float get_fog_amount(float3 eyePosition, float3 worldPosition, float fogStart, float fogRange)
{
	return saturate((length(eyePosition - worldPosition) - fogStart) / (fogRange));
}

float get_fog_amount(float3 viewDirection, float fogStart, float fogRange)
{
	return saturate((length(viewDirection) - fogStart) / (fogRange));
}

/************* Resources *************/
cbuffer CBufferPerFrame
{
	float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float3 LightPosition = { 0.0f, 0.0f, 0.0f };
	float LightRadius = 10.0f;
	float3 CameraPosition;

	//spotlight
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
}

cbuffer CBufferSkinning
{
	float4x4 BoneTransforms[MaxBones];
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
	uint4 BoneIndices : BONEINDICES;
	float4 BoneWeights : WEIGHTS;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float2 TextureCoordinate : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float Attenuation : TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4x4 skinTransform = (float4x4)0;
	skinTransform += BoneTransforms[IN.BoneIndices.x] * IN.BoneWeights.x;
	skinTransform += BoneTransforms[IN.BoneIndices.y] * IN.BoneWeights.y;
	skinTransform += BoneTransforms[IN.BoneIndices.z] * IN.BoneWeights.z;
	skinTransform += BoneTransforms[IN.BoneIndices.w] * IN.BoneWeights.w;

	float4 position = mul(IN.ObjectPosition, skinTransform);
	OUT.Position = mul(position, WorldViewProjection);
	OUT.WorldPosition = mul(position, World).xyz;

	float4 normal = mul(float4(IN.Normal, 0), skinTransform);
	OUT.Normal = normalize(mul(normal, World).xyz);

	OUT.TextureCoordinate = IN.TextureCoordinate;

	float3 lightDirection = LightPosition - OUT.WorldPosition;
	OUT.Attenuation = saturate(1.0f - (length(lightDirection) / LightRadius));

	return OUT;
}

/************* Pixel Shaders *************/

float4 pixel_shader(VS_OUTPUT input) : SV_Target
{
	/*float4 OUT = (float4)0;

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

	OUT.rgb = ambient + diffuse + specular;
	OUT.a = 1.0f;

	return OUT;*/

	input.Normal = normalize(input.Normal);

	float4 diffuseTexture = ColorTexture.Sample(ColorSampler, input.TextureCoordinate);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	//Create the vector between light position and pixels position
	float3 lightToPixelVec = pos - input.WorldPosition;

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

/************* Techniques *************/

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
	}
}