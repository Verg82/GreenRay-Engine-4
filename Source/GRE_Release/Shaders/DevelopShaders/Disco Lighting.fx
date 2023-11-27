//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// DiscoLighting
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Walls
//--------------------------------------------------------------//
string DiscoLighting_Walls_Room : ModelData = "C:/Program Files (x86)/ATI Research Inc/RenderMonkey 1.6/Examples/Media/Models/Cube.3ds";

float4x4 view_proj_matrix : ViewProjection;
float4 lightPos
<
   string UIName = "lightPos";
   string UIWidget = "Direction";
   bool UIVisible = " true";
   float4 UIMin = ( -100.00, -100.00, -100.00, -100.00 );
   float4 UIMax = ( 100.00, 100.00, 100.00, 100.00 );
   bool Normalize = " false";
> = ( 0.00, 28.00, 0.00, 1.00 );
float time_0_X : Time0_X;
float rotateX
<
   string UIName = "rotateX";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.48;
float rotateY
<
   string UIName = "rotateY";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.42;
float rotateZ
<
   string UIName = "rotateZ";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.45;
float4 view_position : ViewPosition;
struct VS_OUTPUT 
{
   float4 Pos:      POSITION;
   float2 texCoord: TEXCOORD0;
   float3 lightVec: TEXCOORD1;
   float3 viewVec:  TEXCOORD2;
   float3 dir:      TEXCOORD3;
};

VS_OUTPUT DiscoLighting_Walls_Vertex_Shader_main(float4 Pos: POSITION, float3 normal: NORMAL)
{
   VS_OUTPUT Out;

   // The model we're using doesn't contain any texture coordinates,
   // however,being a simple cube we can easily generate them ourselves.
   Out.texCoord.x = 0.5 * lerp(Pos.x, Pos.y, abs(normal.x));
   Out.texCoord.y = 0.5 * lerp(Pos.z, Pos.y, abs(normal.z));

   // No texture coordinates means no tangent space, so we'll
   // have to generate it ourselves too.
   float3 tangent  = -float3(abs(normal.y) + abs(normal.z), abs(normal.x), 0);
   float3 binormal = -float3(0, abs(normal.z), abs(normal.x) + abs(normal.y));

   // Scale and flip
   Pos.xyz *= -200;

   Out.Pos = mul(view_proj_matrix, Pos);

   // Transform light vector into tangent space
   float3 lightVec = lightPos - Pos;
   Out.lightVec.x = dot(lightVec, tangent);
   Out.lightVec.y = dot(lightVec, binormal);
   Out.lightVec.z = dot(lightVec, normal);

   // Transform view vector into tangent space
   float3 viewVec = view_position - Pos;
   Out.viewVec.x = dot(viewVec, tangent);
   Out.viewVec.y = dot(viewVec, binormal);
   Out.viewVec.z = dot(viewVec, normal);

   // Direction to look up our fancy shading at
   float3 dir = -lightVec;

   // Create a rotation matrix from three Euler angles.
   // This work is better done on the CPU as it is constant for the
   // whole scene. In RenderMonkey we'll have to do it in the shader though.   
   float cosX,sinX;
   float cosY,sinY;
   float cosZ,sinZ;

   sincos(rotateX * time_0_X,sinX,cosX);
   sincos(rotateY * time_0_X,sinY,cosY);
   sincos(rotateZ * time_0_X,sinZ,cosZ);

   float3x3 rot = float3x3(
      cosY * cosZ + sinX * sinY * sinZ,   -cosX * sinZ,    sinX * cosY * sinZ - sinY * cosZ,
      cosY * sinZ - sinX * sinY * cosZ,    cosX * cosZ,   -sinY * sinZ - sinX * cosY * cosZ,
      cosX * sinY,                         sinX,           cosX * cosY
   );

   // Rotate our fancy shading
   Out.dir = mul(rot, dir);

   return Out;
}



texture WallTiles_Tex
<
   string ResourceName = "C:\Program Files (x86)\ATI Research Inc\RenderMonkey 1.6\Examples\Media\Textures\Tiles.dds";
>;
sampler Base = sampler_state
{
   Texture = (WallTiles_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture WallTiles_BumpMap_Tex
<
   string ResourceName = "C:\Program Files (x86)\ATI Research Inc\RenderMonkey 1.6\Examples\Media\Textures\TilesDOT3.tga";
>;
sampler Bump = sampler_state
{
   Texture = (WallTiles_BumpMap_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture LightCube_Tex
<
   string ResourceName = "C:\Program Files (x86)\ATI Research Inc\RenderMonkey 1.6\Examples\Media\Textures\LightCube.dds";
>;
sampler LightCube = sampler_state
{
   Texture = (LightCube_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 DiscoLighting_Walls_Pixel_Shader_main( float2 texCoord : TEXCOORD0, 
             float3 lightVec : TEXCOORD1, 
             float3 viewVec  : TEXCOORD2, 
             float3 dir      : TEXCOORD3 ) : COLOR 
{
   float4 base = tex2D(Base, texCoord);
   float3 bump = tex2D(Bump, texCoord) * 2 - 1;

   // Grab our fancy shading from a cubemap
   float4 light = texCUBE(LightCube, dir);

   bump = normalize(bump);
   lightVec = normalize(lightVec);
   viewVec = normalize(viewVec);

   // Standard lighting
   float diffuse = saturate(dot(lightVec, bump));
   float specular = pow(saturate(dot(reflect(-viewVec, bump), lightVec)), 12);

   return (diffuse * base + 0.7 * specular) * light + 0.3 * base;
}



//--------------------------------------------------------------//
// Object
//--------------------------------------------------------------//
string DiscoLighting_Object_Cracked_Quad_Object : ModelData = "C:/Program Files (x86)/ATI Research Inc/RenderMonkey 1.6/Examples/Media/Models/CrackedQuad.3ds";

float4x4 DiscoLighting_Object_Vertex_Shader_view_proj_matrix : ViewProjection;
float4 DiscoLighting_Object_Vertex_Shader_lightPos
<
   string UIName = "DiscoLighting_Object_Vertex_Shader_lightPos";
   string UIWidget = "Direction";
   bool UIVisible = " true";
   float4 UIMin = ( -100.00, -100.00, -100.00, -100.00 );
   float4 UIMax = ( 100.00, 100.00, 100.00, 100.00 );
   bool Normalize = " false";
> = ( 0.00, 28.00, 0.00, 1.00 );
float DiscoLighting_Object_Vertex_Shader_time_0_X : Time0_X;
float DiscoLighting_Object_Vertex_Shader_rotateX
<
   string UIName = "DiscoLighting_Object_Vertex_Shader_rotateX";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.48;
float DiscoLighting_Object_Vertex_Shader_rotateY
<
   string UIName = "DiscoLighting_Object_Vertex_Shader_rotateY";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.42;
float DiscoLighting_Object_Vertex_Shader_rotateZ
<
   string UIName = "DiscoLighting_Object_Vertex_Shader_rotateZ";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.45;
float4 DiscoLighting_Object_Vertex_Shader_view_position : ViewPosition;
struct DiscoLighting_Object_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos:      POSITION;
   float2 texCoord: TEXCOORD0;
   float3 lightVec: TEXCOORD1;
   float3 viewVec:  TEXCOORD2;
   float3 dir:      TEXCOORD3;
};

DiscoLighting_Object_Vertex_Shader_VS_OUTPUT DiscoLighting_Object_Vertex_Shader_main( float4 Pos     : POSITION, 
                float3 normal  : NORMAL,
                float3 tangent : TANGENT, 
                float3 binormal: BINORMAL, 
                float2 texCoord: TEXCOORD0 )
{
   DiscoLighting_Object_Vertex_Shader_VS_OUTPUT Out;

   Out.Pos = mul(DiscoLighting_Object_Vertex_Shader_view_proj_matrix, Pos);
   Out.texCoord = texCoord;

   // Transform light vector into tangent space
   float3 lightVec = DiscoLighting_Object_Vertex_Shader_lightPos - Pos;
   Out.lightVec.x = dot(lightVec, tangent);
   Out.lightVec.y = dot(lightVec, binormal);
   Out.lightVec.z = dot(lightVec, normal);

   // Transform view vector into tangent space
   float3 viewVec = DiscoLighting_Object_Vertex_Shader_view_position - Pos;
   Out.viewVec.x = dot(viewVec, tangent);
   Out.viewVec.y = dot(viewVec, binormal);
   Out.viewVec.z = dot(viewVec, normal);

   // Direction to look up our fancy shading at
   float3 dir = -lightVec;

   // Create a rotation matrix from three Euler angles.
   // This work is better done on the CPU as it is constant for the
   // whole scene. In RenderMonkey we'll have to do it in the shader though.
   float cosX,sinX;
   float cosY,sinY;
   float cosZ,sinZ;

   sincos(DiscoLighting_Object_Vertex_Shader_rotateX * DiscoLighting_Object_Vertex_Shader_time_0_X,sinX,cosX);
   sincos(DiscoLighting_Object_Vertex_Shader_rotateY * DiscoLighting_Object_Vertex_Shader_time_0_X,sinY,cosY);
   sincos(DiscoLighting_Object_Vertex_Shader_rotateZ * DiscoLighting_Object_Vertex_Shader_time_0_X,sinZ,cosZ);

   float3x3 rot = float3x3(
      cosY * cosZ + sinX * sinY * sinZ, -cosX * sinZ,  sinX * cosY * sinZ - sinY * cosZ,
      cosY * sinZ - sinX * sinY * cosZ,  cosX * cosZ, -sinY * sinZ - sinX * cosY * cosZ,
      cosX * sinY,                       sinX,         cosX * cosY
   );

   // Rotate our fancy shading
   Out.dir = mul(rot, dir);

   return Out;
}







texture FieldStone_Tex
<
   string ResourceName = "C:\Program Files (x86)\ATI Research Inc\RenderMonkey 1.6\Examples\Media\Textures\Fieldstone.tga";
>;
sampler Base2 = sampler_state
{
   Texture = (FieldStone_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture FieldStone_BumpMap_Tex
<
   string ResourceName = "C:\Program Files (x86)\ATI Research Inc\RenderMonkey 1.6\Examples\Media\Textures\FieldstoneBumpDOT3.tga";
>;
sampler Bump2 = sampler_state
{
   Texture = (FieldStone_BumpMap_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
sampler DiscoLighting_Object_Pixel_Shader_LightCube = sampler_state
{
   Texture = (LightCube_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 DiscoLighting_Object_Pixel_Shader_main( float2 texCoord: TEXCOORD0, 
             float3 lightVec: TEXCOORD1, 
             float3 viewVec : TEXCOORD2, 
             float3 dir     : TEXCOORD3 ) : COLOR
{
   float4 base = tex2D( Base2, texCoord );
   float3 bump = tex2D( Bump2, texCoord ) * 2 - 1;
   
   // Grab our fancy shading from a cubemap
   float4 light = texCUBE( DiscoLighting_Object_Pixel_Shader_LightCube, dir );

   bump = normalize( bump );
   lightVec = normalize( lightVec );
   viewVec = normalize( viewVec );

   // Standard lighting
   float diffuse = saturate( dot( lightVec, bump ));
   float specular = pow( saturate( dot(reflect( -viewVec, bump ), lightVec )), 12 );

   return ( diffuse * base + 0.7 * specular ) * light + 0.3 * base;
}





//--------------------------------------------------------------//
// Light
//--------------------------------------------------------------//
string DiscoLighting_Light_Light_Ball : ModelData = "C:/Program Files (x86)/ATI Research Inc/RenderMonkey 1.6/Examples/Media/Models/Sphere.3ds";

float4x4 DiscoLighting_Light_Vertex_Shader_view_proj_matrix : ViewProjection;
float4 DiscoLighting_Light_Vertex_Shader_lightPos
<
   string UIName = "DiscoLighting_Light_Vertex_Shader_lightPos";
   string UIWidget = "Direction";
   bool UIVisible = " true";
   float4 UIMin = ( -100.00, -100.00, -100.00, -100.00 );
   float4 UIMax = ( 100.00, 100.00, 100.00, 100.00 );
   bool Normalize = " false";
> = ( 0.00, 28.00, 0.00, 1.00 );
float DiscoLighting_Light_Vertex_Shader_time_0_X : Time0_X;
float DiscoLighting_Light_Vertex_Shader_rotateX
<
   string UIName = "DiscoLighting_Light_Vertex_Shader_rotateX";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.48;
float DiscoLighting_Light_Vertex_Shader_rotateY
<
   string UIName = "DiscoLighting_Light_Vertex_Shader_rotateY";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.42;
float DiscoLighting_Light_Vertex_Shader_rotateZ
<
   string UIName = "DiscoLighting_Light_Vertex_Shader_rotateZ";
   string UIWidget = "Numeric";
   bool UIVisible = " true";
   float UIMin = 0.00;
   float UIMax = 3.00;
> = 0.45;
struct DiscoLighting_Light_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos: POSITION;
   float3 dir: TEXCOORD0;
};

DiscoLighting_Light_Vertex_Shader_VS_OUTPUT DiscoLighting_Light_Vertex_Shader_main(float4 Pos: POSITION)
{
   DiscoLighting_Light_Vertex_Shader_VS_OUTPUT Out;

   float3 dir = normalize(Pos.xyz);
   Pos.xyz = 10 * dir + DiscoLighting_Light_Vertex_Shader_lightPos;   

   Out.Pos = mul(DiscoLighting_Light_Vertex_Shader_view_proj_matrix, Pos);

   // Create a rotation matrix from three Euler angles.
   // This work is better done on the CPU as it is constant for the
   // whole scene. In RenderMonkey we'll have to do it in the shader though.
   float cosX,sinX;
   float cosY,sinY;
   float cosZ,sinZ;

   sincos(DiscoLighting_Light_Vertex_Shader_rotateX * DiscoLighting_Light_Vertex_Shader_time_0_X,sinX,cosX);
   sincos(DiscoLighting_Light_Vertex_Shader_rotateY * DiscoLighting_Light_Vertex_Shader_time_0_X,sinY,cosY);
   sincos(DiscoLighting_Light_Vertex_Shader_rotateZ * DiscoLighting_Light_Vertex_Shader_time_0_X,sinZ,cosZ);

   float3x3 rot = float3x3(
      cosY * cosZ + sinX * sinY * sinZ, -cosX * sinZ,  sinX * cosY * sinZ - sinY * cosZ,
      cosY * sinZ - sinX * sinY * cosZ,  cosX * cosZ, -sinY * sinZ - sinX * cosY * cosZ,
      cosX * sinY,                       sinX,         cosX * cosY
   );


   Out.dir = mul(rot, dir);

   return Out;
}



sampler DiscoLighting_Light_Pixel_Shader_LightCube = sampler_state
{
   Texture = (LightCube_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 DiscoLighting_Light_Pixel_Shader_main(float3 dir: TEXCOORD0) : COLOR 
{
   return texCUBE(DiscoLighting_Light_Pixel_Shader_LightCube, dir);
}



//--------------------------------------------------------------//
// Technique Section for DiscoLighting
//--------------------------------------------------------------//
technique DiscoLighting
{
   pass Walls
   {
      VertexShader = compile vs_1_1 DiscoLighting_Walls_Vertex_Shader_main();
      PixelShader = compile ps_2_0 DiscoLighting_Walls_Pixel_Shader_main();
   }

   pass Object
   {
      VertexShader = compile vs_1_1 DiscoLighting_Object_Vertex_Shader_main();
      PixelShader = compile ps_2_0 DiscoLighting_Object_Pixel_Shader_main();
   }

   pass Light
   {
      VertexShader = compile vs_1_1 DiscoLighting_Light_Vertex_Shader_main();
      PixelShader = compile ps_2_0 DiscoLighting_Light_Pixel_Shader_main();
   }

}

