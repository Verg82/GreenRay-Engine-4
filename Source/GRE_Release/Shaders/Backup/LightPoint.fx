// a light shader that performs basic point lighting
// no materials yet but has a fixed specular exponent
#include "SharedGeomVertex.fx"
#include "SharedLightPixel.fx"

// sky and ground colour of the hemisphere light
half3 f3LightColour		= float3(0.1,0.1,0.1);	// colour of the light
float3 f3LightPosition		= float3(0,10,10);	// point of the light
half fRcpRadius			= 1.0f;					// the 1.f / the lights radius (distance of total fall off)
half fAttenuationFunction	= 7.f/1024.f;		// function to use for distance attenuation

int LightID = 0;								// used to set and test the stencil buff

// output from the pixel shader is back buffer
struct PS_OUT
{
	half4 Colour : COLOR0;
};

half4 ShowVolume0( float4 uv : TEXCOORD0 ) : COLOR0
{
	return half4(0.5f,0,0,1);
}

half4 ShowVolume1( float4 uv : TEXCOORD0 ) : COLOR0
{
	return half4(0,0.5f,0,1);
}

PS_OUT LightPoint( float4 uv : TEXCOORD0 )
{
	PS_OUT outp;

	// lookup framebuffer at this pixel (we use have to use projection as this are 3D objects)
	float4 packPos = tex2Dproj( PositionBuffer, uv );
	half4 packNormal = tex2Dproj( NormalBuffer, uv );
	half4 packCs0 = tex2Dproj( Cs0Buffer, uv );
	half4 packCs1 = tex2Dproj( Cs1Buffer, uv );
	
	// unpack parameters from framebuffer	
	// unpack position and normal into view space
	float3 posInView = UnPackPositionFromFatFramebuffer( packPos.xyz );
	half3 N = UnPackNormalFromFatFramebuffer( packNormal.xyz );
	half MatIndx = packNormal.w;
	
	// form light vector to each pixel
	float3 L = f3LightPosition - posInView;
	half lDirMag = dot( L, L );
	half invlDirMag = sqrt( lDirMag );
	L /= invlDirMag;

	// attenuation
	half atten = LookupAttenuation( invlDirMag, fRcpRadius, fAttenuationFunction );

	// camera vector = normalize( CamPosInViewSpace(0,0,0) - posInViewSpace )
	half3 I = normalize( -posInView.xyz );

	// recieve colour from the illumate function
	half3 Col;
	
	Illuminate( N, I, L, packCs0, packCs1, f3LightColour, MatIndx, Col );
	
	half3 finalColour =  Col * atten;

	outp.Colour = half4( finalColour, 1 );

	return outp;
}

// output from the vertex shader is position and uv to lookup screen space pixel
struct VS_OUT
{
	float4 RastPosition : POSITION;
	float4 ScreenXY		: TEXCOORD0;
};

VS_OUT LightPointConvexVS( float3 LocalPosition : POSITION )
{
	VS_OUT outp;
	float4 f4HClipPos;
	
	// tranform local space position into HCLIP space
	f4HClipPos = mul(float4(LocalPosition, 1.0), matWorldViewProjection );
	
	outp.RastPosition = f4HClipPos;
	outp.ScreenXY = f4HClipPos;

	outp.ScreenXY.xy = float2(  outp.ScreenXY.x * 0.5, -outp.ScreenXY.y * 0.5 );

	outp.ScreenXY.xy += (0.5 * outp.ScreenXY.w);
	outp.ScreenXY.x += 0.5f * fRcpFrameWidth * outp.ScreenXY.w;
	outp.ScreenXY.y += 0.5f * fRcpFrameHeight * outp.ScreenXY.w;

	return outp;
}

// techniques that compile and use the approiate pixel shader

// states for convex objects with an infinite far plane and no z-testing
// (guarentees each pixel is hit once and once only)
technique T0
{
	pass P0
	{
		VertexShader = compile vs_1_1 LightPointConvexVS();
		PixelShader = compile ps_2_0 LightPoint();
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		AlphaRef = 0;
		SrcBlend = One;
		DestBlend = One;

		// render back faces only, with an infinate far plane and no z-testing
		// guarentees a convex object only touchs each pixel once and once only		
		CullMode = CW;
		ZEnable = False;
	}
}

// states for convex objects with z-testing but that don't touch the front clip plane
// (guarentees each pixel is hit once and once only)
technique T1
{
	pass P0
	{
		VertexShader = compile vs_1_1 LightPointConvexVS();
		PixelShader = compile ps_2_0 LightPoint();
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		AlphaRef = 0;
		SrcBlend = One;
		DestBlend = One;
		
		// render front faces only, with z-testing. If the z-test fails the pixel
		// is not visible with regard lighting and can be rejected.
		CullMode = CCW;
		ZEnable = True;
		ZFunc = Less;
		ZWriteEnable = False;
	}
}


//---------------------
// Debug versions of the above technique
//--------------------
technique Debug0
{
	pass P0
	{
		VertexShader = compile vs_1_1 LightPointConvexVS();
		PixelShader = compile ps_2_0 ShowVolume0();
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		AlphaRef = 0;
		SrcBlend = One;
		DestBlend = One;

		// render back faces only, with an infinate far plane and no z-testing
		// guarentees a convex object only touchs each pixel once and once only		
		CullMode = CW;
		ZEnable = False;
	}
}

technique Debug1
{
	pass P0
	{
		VertexShader = compile vs_1_1 LightPointConvexVS();
		PixelShader = compile ps_2_0 ShowVolume1();
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		AlphaRef = 0;
		SrcBlend = One;
		DestBlend = One;
		
		// render front faces only, with z-testing. If the z-test fails the pixel
		// is not visible with regard lighting and can be rejected.
		CullMode = CCW;
		ZEnable = True;
		ZFunc = Less;
		ZWriteEnable = False;
	}
}
