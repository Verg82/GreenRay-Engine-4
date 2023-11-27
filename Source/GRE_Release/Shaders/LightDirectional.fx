// a light shader that performs basic directional lighting
// no materials yet but has a fixed specular exponent
#include "SharedLightPixel.fx"

// sky and ground colour of the hemisphere light
half3 f3LightColour    = float3(0.5,0.5,0.5);       //colour of the light
half3 f3LightDirection = float3(-0.707,0.707,0); //direction of the light

float4x4 matShadowWarp;                // matrix that that a position from view space to shadow space
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

// output from the pixel shader is back buffer
struct PS_OUT
{
    half4 Colour: COLOR0;
};

PS_OUT LightDirectional(float2 uv: TEXCOORD0)
{
  PS_OUT outp;

  //Lookup Framebuffer At The Pixel
  float4 packPos = tex2D(PositionBuffer, uv);
  float4 packNormal = tex2D(NormalBuffer, uv);
  float4 packCs0 = tex2D(Cs0Buffer, uv);
  float4 packCs1 = tex2D(Cs1Buffer, uv);
    
  //Unpack Parameters From Framebuffer    
  half MatIndx;
   
     //Unpack position and normal into view space
  float3 posInView = UnPackPositionFromFatFramebuffer(packPos.xyz);
  half3 N = UnPackNormalFromFatFramebuffer(packNormal.xyz);
  MatIndx = packNormal.w;







  half shadow;    //Multiple colour by shadow (0 when in shadow)
  float4 dist;
  float4 posInShadow = mul(float4(posInView,1),matShadowWarp);
  float distInShadow = posInShadow.z/posInShadow.w;

  //Get the 4 jittered distances as a vector
  dist.x = tex2Dproj(ShadowMap, posInShadow + f4ShadowJitter0); // do projection in texture load
  dist.y = tex2Dproj(ShadowMap, posInShadow + f4ShadowJitter1); // do projection in texture load
  dist.z = tex2Dproj(ShadowMap, posInShadow + f4ShadowJitter2); // do projection in texture load
  dist.w = tex2Dproj(ShadowMap, posInShadow + f4ShadowJitter3); // do projection in texture load
    
  //< is a vector op under HLSL preduces a boolean result vector
  dist.xyzw = (distInShadow < dist.xyzw);

  //Dist.xyzw each component contains 1 not in shadow, acculamate and scale shadow value
  shadow = dot(dist,dist)*0.25f;
    
  //Camera vector
  half3 I = normalize(posInView.xyz);

  //Recieve colour from the illumate function
  half3 Col;
    
  Illuminate(N, I, f3LightDirection, packCs0, packCs1, f3LightColour, MatIndx, Col);
    
  half3 finalColour = Col*shadow;

  outp.Colour = half4(finalColour,1);
  return outp;    
}

// techniques that compile and use the approiate pixel shader
technique T0
{
    pass P0
    {
        PixelShader = compile ps_2_0 LightDirectional();
        AlphaBlendEnable = True;
        AlphaTestEnable = True;
        AlphaRef = 0;
        SrcBlend = One;
        DestBlend = One;
        ZEnable = false;
    }
}