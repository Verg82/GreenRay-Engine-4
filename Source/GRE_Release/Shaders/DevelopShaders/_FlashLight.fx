
//Flash Light
float4x4 WorldViewProjection;    // World*View*Projection matrix
float4x4 World;         //World
float4x4 TexTransform;  //Texture transformation matrix
float3 EyePos;          //World position of the camera
//float Ambient_ = 0.0f;  //Amount of ambient light
float LightFalloff;     //Range of the light
texture LightTex;
texture  Texture;                // Texture For Scene Rendering


//Flash Light Texture Sampler
//Must be clamped so the light texture shows up only once
sampler LightTexSampler = sampler_state
{
    Texture = <LightTex>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

sampler ColorTexSampler = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = Wrap;
    AddressV  = Wrap;
};



//Flash Light Structures
struct MainA2V{float4 Position: POSITION;float2 TexCoord0: TEXCOORD0;};
struct MainV2P{float4 Position: POSITION;float2 TexCoord0: TEXCOORD0;
                                 float4 TexCoord1: TEXCOORD1;float3 LightVec: TEXCOORD2;};





//Flash Light VS
void MainVS(in MainA2V IN, out MainV2P OUT)
{
    //Copy color texture coordinates through
    OUT.TexCoord0 = IN.TexCoord0;
    
    //First two are the projected texture coordinates
    OUT.TexCoord1 = mul(IN.Position, TexTransform);
    
    //Position to light vector
    OUT.LightVec = mul(IN.Position.xyz, (float3x3)World) - EyePos;
    
    //Transform model-space vertex position to screen space:
    OUT.Position = mul(IN.Position, WorldViewProjection);
}


//Flash Light PS
float4 MainPS(in MainV2P IN): COLOR 
{
    //Read in values for light texture and color texture
    float4 Color    = tex2D(ColorTexSampler, IN.TexCoord0);
    float4 Light    = tex2Dproj(LightTexSampler, IN.TexCoord1);
    
    //Squared distance from pixel to light
    float LenSq = dot( IN.LightVec, IN.LightVec );
        
    //Find the attenuation
    float Attn = min(( LightFalloff * LightFalloff ) / LenSq, 1.0f);
    
    //Compute lighting amount
    float4 I = (Light * Attn); //+ Ambient_;
    
    //Color of texture * lighting amount
    return Color * I;
}

technique FlashLight
{

  //Flash Light
  pass P0
  {
    VertexShader = compile vs_1_1 MainVS();
    PixelShader = compile ps_2_0 MainPS();

  }
}