// The most basic Geometry Shader
// take the geometry (vertex position and normal) and a single material and output correct parameters into the
// fat framebuffer

// variable and function used all over the place
#include "SharedGeomVertex.fx"
#include "SharedGeomPixel.fx"

float4x4 matWorld;
// non-shared vertex parameter block
half3     f3Colour0 = half3(0.5, 0.5, 0.5);
half3     f3Colour1 = half3(1.0, 1.0, 1.0);

half3 f3Material = half3(0.5f, 0.5f, 0.f);            // Kd, Ks, MatIndex,

half2 f2DotProductFuncs = half2(0,0);                // Fa, Fb (functions to shape and control the lighting dot product)

// the diffuse texture
shared texture DiffuseMapTexture;
sampler DiffuseMap = sampler_state
{
    Texture = (DiffuseMapTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

// the specular texture
shared texture SpecularMapTexture;
sampler SpecularMap = sampler_state
{
    Texture = (SpecularMapTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};
// the normal texture
shared texture NormalMapTexture;
sampler NormalMap = sampler_state
{
    Texture = (NormalMapTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};


//-----------------------------------
// 2 parts to a geometry shader : vertex and then the pixel shader
// output from the vertex shader is input into the pixel shader
// so I see them as one continous execetion (the fact they occur at different rate doesn't really matter)

struct VS_IN
{
  float3 LocalPosition: POSITION;
  float3 LocalNormal: NORMAL;
     float2 UV: TEXCOORD0;
};

// one oddity is that we have to output position twice from the vertex shader (once normally and once for
// us to output in the fat framebuffer)
struct VS_OUT
{
    float4 RastPosition: POSITION;
    float4 Position: TEXCOORD0;
    float3 Normal: TEXCOORD1;
    float2 UV: TEXCOORD2;
};

// out of VS into PS
struct PS_IN
{
    float4 Position: TEXCOORD0;
    float3 Normal: TEXCOORD1;
    float2 UV: TEXCOORD2;
};
// output from the pixel shader is the fat framebuffer
struct PS_OUT
{
  float4 Position: COLOR0;
 float4 Normal: COLOR1;
 float4 Cs0: COLOR2;
 float4 Cs1: COLOR3;
};

VS_OUT GeomSmoothSolidColourVS( VS_IN inp )
{
    VS_OUT outp;
    
    float4 f4HClipPos;
    float3 f3ViewNorm;
    
    // tranform local space position into HCLIP space
    f4HClipPos = mul(float4(inp.LocalPosition, 1.0), matWorldViewProjection );
    // transform local space normal in view space
    f3ViewNorm = mul( inp.LocalNormal, matTranInvWorldView );
    
    outp.RastPosition = f4HClipPos;
    outp.Position = f4HClipPos;
    outp.Normal = f3ViewNorm;
    //outp.UV = float2(inp.LocalPosition.x, inp.LocalPosition.z);  //What That Shit?

          outp.UV = inp.UV;

    return outp;
}

PS_OUT GeomSmoothSolidColourPS( PS_IN inp )
{
    PS_OUT outp;

    outp.Position.xyz = PackPositionForFatFramebuffer( inp.Position);

    half3 normalTex = tex2D( NormalMap, inp.UV );
    half3 diffuseCol = tex2D( DiffuseMap, inp.UV );
    half3 specularCol = tex2D( SpecularMap, inp.UV );
    
    // normalize once here so each light doesn't have too
    outp.Normal.xyz = PackNormalForFatFramebuffer( normalize( inp.Normal + (normalTex*0.5) ) );
    outp.Cs0.xyz = diffuseCol * f3Material.x;
    outp.Cs1.xyz = specularCol * f3Material.y;

    outp.Position.w = 0;                // unused
    outp.Normal.w = f3Material.z;        // Material index
    outp.Cs0.w = f2DotProductFuncs.x;    // Fa
    outp.Cs1.w = f2DotProductFuncs.y;    // Fb


    return outp;
}

// multi-pass versions should also optimize vertex program but for now leave as is
float4 GeomSmoothSolidColourPSMPass0( PS_IN inp ) : COLOR0
{
    float4 pos;
    pos.xyz = PackPositionForFatFramebuffer( inp.Position );
    pos.w = 0;        // unused
    return pos;
}
float4 GeomSmoothSolidColourPSMPass1( PS_IN inp ) : COLOR0
{
    float4 retv;
    retv.xyz = PackNormalForFatFramebuffer( normalize( inp.Normal ) );
    retv.w = f3Material.z;            // Material index
    return retv;
}

float4 GeomSmoothSolidColourPSMPass2( PS_IN inp ) : COLOR0
{
    float4 retv;
    
    half3 diffuseCol = tex2D( DiffuseMap, inp.UV );

    retv.xyz = diffuseCol;
    retv.w = f2DotProductFuncs.x;        // Fa
    return retv;
}

float4 GeomSmoothSolidColourPSMPass3( PS_IN inp ) : COLOR0
{
    float4 retv;
    
    half3 specularCol = tex2D( SpecularMap, inp.UV );

    retv.xyz = specularCol;
    retv.w = f2DotProductFuncs.y;        // Fb
    return retv;
}

//-------------------------------------------------
// Shadow
struct SHADOW_VS_IN
{
    float3 LocalPosition : POSITION;
};

// one oddity is that we have to output position twice from the vertex shader (once normally and once for
// us to output in the fat framebuffer)
struct SHADOW_VS_OUT
{
    float4 RastPosition : POSITION;
    float1 Depth        : TEXCOORD0;
};

// out of VS into PS
struct SHADOW_PS_IN
{
    float1 Depth    : TEXCOORD0;
};
// output from the pixel shader is the fat framebuffer
struct SHADOW_PS_OUT
{
    float4 Depth : COLOR0;
};

SHADOW_VS_OUT GeomSmoothSolidColourShadowVS( SHADOW_VS_IN inp )
{
    SHADOW_VS_OUT outp;
    
    float4 f4HClipPos;
    float3 f3ViewNorm;
    
    // tranform local space position into HCLIP space
    f4HClipPos = mul(float4(inp.LocalPosition, 1.0), matWorldViewProjection );
    
    outp.RastPosition = f4HClipPos;
    outp.Depth = f4HClipPos.z / f4HClipPos.w;
    
    return outp;
}

SHADOW_PS_OUT GeomSmoothSolidColourShadowPS( SHADOW_PS_IN inp )
{
    SHADOW_PS_OUT outp;
    
    outp.Depth = float4(inp.Depth,inp.Depth,inp.Depth,inp.Depth);

    return outp;
}

technique T0
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourPS();
        ZEnable = true;
        ZFunc = Less;
        ZWriteEnable = true;
        CullMode = CCW;
    }
}

technique T1
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourShadowVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourShadowPS();
        ZEnable = true;
        ZFunc = Less;
        ZWriteEnable = true;
        CullMode = CCW;
    }
}

// multi-pass
technique T2
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourPSMPass0();
        ZEnable = true;
        ZFunc = Less;
        ZWriteEnable = true;
        CullMode = CCW;
    }
}
technique T3
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourPSMPass1();
        ZEnable = true;
        ZFunc = Equal;
        ZWriteEnable = false;
        CullMode = CCW;
    }
}
technique T4
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourPSMPass2();
        ZEnable = true;
        ZFunc = Equal;
        ZWriteEnable = false;
        CullMode = CCW;
    }
}

technique T5
{
    pass P0
    {
        VertexShader = compile vs_2_0 GeomSmoothSolidColourVS();
        PixelShader = compile ps_2_0 GeomSmoothSolidColourPSMPass3();
        ZEnable = true;
        ZFunc = Equal;
        ZWriteEnable = false;
        CullMode = CCW;
    }
}

