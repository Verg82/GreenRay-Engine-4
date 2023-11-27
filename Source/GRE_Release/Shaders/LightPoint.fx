// a light shader that performs basic point lighting
// no materials yet but has a fixed specular exponent
#include "SharedGeomVertex.fx"
#include "SharedLightPixel.fx"




static const int NUM_SHADOWS = 1;     // Scene lights 







// sky and ground colour of the hemisphere light
half3 f3LightColour       = float3(0.1,0.1,0.1); // colour of the light
float3 f3LightPosition    = float3(0,10,10);     // point of the light
half fRcpRadius           = 1.0f;                // the 1.f / the lights radius (distance of total fall off)
half fAttenuationFunction = 7.f/1024.f;           // function to use for distance attenuation
int LightID                                      = 0;                   // used to set and test the stencil buff


//Shadoww
float4x4 matShadowWarp[NUM_SHADOWS];                // matrix that a position from view space to shadow space

float4x4 matShadowWarp0;
float4x4 matShadowWarp1;

texture ShadowMapTexture;
sampler ShadowMap = sampler_state 
{
    Texture = (ShadowMapTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};


//Output from the pixel shader is back buffer
struct PS_OUT
{
   half4 Color: COLOR0;
};




half CalculateShadow(float4x4 MatWarp, float3 PosInView)
{
    //half shadow;    //Multiple colour by shadow (0 when in shadow)
    float4 dist;
    float4 PosInShadow = mul(float4(PosInView,1),MatWarp);
    float DistInShadow = PosInShadow.z/PosInShadow.w;

        //Get the 4 jittered distances as a vector
        //do projection in texture load
    dist.x = tex2Dproj(ShadowMap, PosInShadow + f4ShadowJitter0); 
    dist.y = tex2Dproj(ShadowMap, PosInShadow + f4ShadowJitter1);   
    dist.z = tex2Dproj(ShadowMap, PosInShadow + f4ShadowJitter2);
    dist.w = tex2Dproj(ShadowMap, PosInShadow + f4ShadowJitter3); 

 //< is a vector op under HLSL preduces a boolean result vector
  dist.xyzw = (DistInShadow < dist.xyzw);

 //Dist.xyzw each component contains 1 not in shadow, acculamate and scale shadow value
  return dot(dist,dist)*0.25f;
}


PS_OUT LightPoint(float4 uv: TEXCOORD0)
{
    PS_OUT Out;

    // lookup framebuffer at this pixel (we use have to use projection as this are 3D objects)
    float4 packPos = tex2Dproj(PositionBuffer, uv);
    half4 packNormal = tex2Dproj(NormalBuffer, uv);
    half4 packCs0 = tex2Dproj(Cs0Buffer, uv);
    half4 packCs1 = tex2Dproj(Cs1Buffer, uv);
    
    // unpack parameters from framebuffer    
    // unpack position and normal into view space
    float3 PosInView = UnPackPositionFromFatFramebuffer(packPos.xyz);
    half3 N = UnPackNormalFromFatFramebuffer(packNormal.xyz);
    half MatIndx = packNormal.w;
    
    // form light vector to each pixel
    float3 L = f3LightPosition-PosInView;
    half lDirMag = dot(L, L);
    half invlDirMag = sqrt(lDirMag);
    L /= invlDirMag;

    // attenuation
    half atten = LookupAttenuation(invlDirMag, fRcpRadius, fAttenuationFunction);

    // camera vector = normalize(CamPosInViewSpace(0,0,0) - posInViewSpace)
    half3 I = normalize(-PosInView.xyz);

    // recieve colour from the illumate function
    half3 Col;
    
    Illuminate(N, I, L, packCs0, packCs1, f3LightColour, MatIndx, Col);
    

      float Ambient = 0.1f; //Ambient Amount Of Lighting

    //Light And Shadow Processing
    half Shadow = 0.0f;

half Shadow0 = 0.0f;
 //   for (int I = 0; I < NUM_SHADOWS; I++)
  //   {
       //Calculate Shadow
   //    Shadow += CalculateShadow(matShadowWarp[I], PosInView);

    //}


  Shadow = CalculateShadow(matShadowWarp0, PosInView)+1.0f;
 // Shadow0 = CalculateShadow(matShadowWarp1, PosInView);

    //half3 FinalColor = (Ambient+atten)*Col;
    half3 FinalColor = (Ambient+Shadow*atten)*Col;

    Out.Color = half4(FinalColor,1);

    return Out;
}

// output from the vertex shader is position and uv to lookup screen space pixel
struct VS_OUT
{
  float4 RastPosition: POSITION;
  float4 ScreenXY: TEXCOORD0;
};

VS_OUT LightPointConvexVS(float3 LocalPosition: POSITION)
{
    VS_OUT outp;
    float4 f4HClipPos;
    
    // tranform local space position into HCLIP space
    f4HClipPos = mul(float4(LocalPosition, 1.0), matWorldViewProjection);
    
    outp.RastPosition = f4HClipPos;
    outp.ScreenXY = f4HClipPos;

    outp.ScreenXY.xy = float2(outp.ScreenXY.x * 0.5, -outp.ScreenXY.y * 0.5);

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