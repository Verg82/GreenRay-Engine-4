/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_MeshX.cpp
// It's The GreenRay Model Loader Interface.
// Made Specially For Implementing Loading And Rendering DirectX Meshes.
// Date Creation: 29 August 2005
// Last Changing: 28 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_MeshX.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRMeshX Class
///////////////////////////////////////////////////////////////////////////////
CGRMeshX::CGRMeshX()
{  
  m_pSysMemMesh = NULL;
  m_pLocalMesh = NULL;
  m_dwNumMaterials = 0L;
  m_pMaterials = NULL;
  m_pTextures = NULL;
  m_pSpecTextures = NULL;
  m_pNormSrc = NULL;
  m_pNormTextures = NULL;
  m_bUseMaterials = TRUE;
} //EndConstructionDirectives

CGRMeshX::~CGRMeshX()
{
  Destroy();
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRMeshX::Destroy()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::Destroy()
{
  InvalidateObjects();
  for (UINT I = 0; I < m_dwNumMaterials; I++) 
  {
	ReleaseClass(m_pTextures[I]);
	ReleaseClass(m_pSpecTextures[I]);
	ReleaseClass(m_pNormSrc[I]);
	ReleaseClass(m_pNormTextures[I]);
  }
  DeleteArray(m_pTextures);
  DeleteArray(m_pSpecTextures);
  DeleteArray(m_pNormSrc);
  DeleteArray(m_pNormTextures);
  DeleteArray(m_pMaterials);

  ReleaseClass(m_pSysMemMesh);
  m_dwNumMaterials = NULL;

  return S_OK;
} //EndDestroyFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name,UINT MapAmplitude)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  WCHAR StrPath[MAX_PATH];
  LPD3DXBUFFER pAdjacencyBuffer = NULL;
  LPD3DXBUFFER pMtrlBuffer = NULL;
  HRESULT Result;

  //Load The Mesh
  Result = D3DXLoadMeshFromXW(File_Name,D3DXMESH_SYSTEMMEM,d_pDevice, 
  &pAdjacencyBuffer,&pMtrlBuffer,NULL,&m_dwNumMaterials,&m_pSysMemMesh);

  //Optimize The Mesh For Performance
  if (FAILED(Result = m_pSysMemMesh->OptimizeInplace(
  D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE,
  (DWORD*)pAdjacencyBuffer->GetBufferPointer(),NULL,NULL,NULL)))
  {
    ReleaseClass(pAdjacencyBuffer);
    ReleaseClass(pMtrlBuffer);
    return Result;
  }

  //Set StrPath To The Path Of The Mesh File
  WCHAR* pLastBSlash = wcsrchr(StrPath,L'\\');
  if (pLastBSlash) *(pLastBSlash+1) = L'\0';
  else *StrPath = L'\0';

  Result = CreateMaterials(StrPath,pAdjacencyBuffer,pMtrlBuffer,MapAmplitude);

  ReleaseClass(pAdjacencyBuffer);
  ReleaseClass(pMtrlBuffer);

  return Result;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::CreateMaterials()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::CreateMaterials(LPCWSTR Str_Path,
ID3DXBuffer* pAdjacencyBuffer,ID3DXBuffer* pMtrlBuffer,UINT BumpAmplitude)
{
  char* Texture_Name;  //This Value Keep Our Texture Name
  char* TmpName;
  char* SpecTexture;   //Specular Map Texture Suffix
  char* NormTexture;   //Normal Map Texture Suffix
  char* HeightTexture; //Height Map Texture Suffix

  //Get Material Info For The Mesh
  //Get The Array Of Materials Out Of The Buffer
  if (pMtrlBuffer && m_dwNumMaterials > 0)
  {
    //Allocate Memory For The Materials And Textures
    D3DXMATERIAL* d3dxMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
    m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    if (m_pMaterials == NULL) return E_OUTOFMEMORY;

    //m_pTextures = new LPDIRECT3DBASETEXTURE9[m_dwNumMaterials];
    //Diffuse Textures
	m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pTextures == NULL) return E_OUTOFMEMORY;

	//Specular Textures
	m_pSpecTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pSpecTextures == NULL) return E_OUTOFMEMORY;

	//Normal Map Textures
	m_pNormTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pNormTextures == NULL) return E_OUTOFMEMORY;

    //Source Normal Map Textures
	m_pNormSrc = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pNormSrc == NULL) return E_OUTOFMEMORY;

	//Height Map Textures
	m_pHeightTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pHeightTextures == NULL) return E_OUTOFMEMORY;

    //Copy Each Material And Create It's Texture
    for (DWORD I = 0; I < m_dwNumMaterials; I++)
    {
      //Copy The Material
      m_pMaterials[I] = d3dxMtrls[I].MatD3D;
      m_pTextures[I] = NULL;
      m_pSpecTextures[I] = NULL;
	  m_pNormTextures[I] = NULL;
	  m_pNormSrc[I] = NULL;
	  m_pHeightTextures[I] = NULL;

	  //Create A Texture
	  if (d3dxMtrls[I].pTextureFilename)
      {
        Texture_Name = NULL;
        Texture_Name = new char[MAX_PATH];
        TmpName = NULL;
        TmpName = new char[MAX_PATH];
        SpecTexture = NULL;
        SpecTexture = new char[MAX_PATH];
		NormTexture = NULL;
        NormTexture = new char[MAX_PATH];
		HeightTexture = NULL;
        HeightTexture = new char[MAX_PATH];

		strcpy(Texture_Name,m_pTexPath);
        strcat(Texture_Name,d3dxMtrls[I].pTextureFilename);
 
        //Create And Load Diffuse Texture
        if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pTextures[I])))
	    {
          m_pTextures[I] = NULL;
	    }
///////////////////////////////////////////////////////////////////////////////

        //Extract Textures File Names
        TmpName = strrchr(d3dxMtrls[I].pTextureFilename,'.');
        *TmpName = '\0';

        strcpy(SpecTexture,d3dxMtrls[I].pTextureFilename);
	    strcat(SpecTexture,"_Spec.tga");
        strcpy(Texture_Name,m_pTexPath);
        strcat(Texture_Name,SpecTexture);
      
       	//Create And Load Specular Texture
        if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pSpecTextures[I])))
	    {
          m_pSpecTextures[I] = NULL;
	    }
///////////////////////////////////////////////////////////////////////////////

		strcpy(NormTexture,d3dxMtrls[I].pTextureFilename);
	    strcat(NormTexture,"_Norm.tga");
		strcpy(Texture_Name,m_pTexPath);
        strcat(Texture_Name,NormTexture);

       	//Create And Load Normal Texture
        if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pNormTextures[I])))
	    {
          m_pNormTextures[I] = NULL;
	    }
///////////////////////////////////////////////////////////////////////////////

		strcpy(HeightTexture,d3dxMtrls[I].pTextureFilename);
	    strcat(HeightTexture,"_Height.tga");
		strcpy(Texture_Name,m_pTexPath);
        strcat(Texture_Name,HeightTexture);

       	//Create And Load Height Map Texture
        if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pNormSrc[I])))
	    {
          m_pNormSrc[I] = NULL;
	    }
		else
		{
          if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pHeightTextures[I])))
	      {
            m_pHeightTextures[I] = NULL;
	      }
		  else
		  {
		    D3DXComputeNormalMap(m_pHeightTextures[I], m_pNormSrc[I], 0, 0, D3DX_CHANNEL_LUMINANCE, BumpAmplitude); 
		  }
		}

///////////////////////////////////////////////////////////////////////////////

        TmpName = 0;
	  }
	}
/*
  //Create A Texture
  if (d3dxMtrls[i].pTextureFilename)
  {
    WCHAR strTexture[MAX_PATH];
    WCHAR strTextureTemp[MAX_PATH];
    D3DXIMAGE_INFO ImgInfo;

    //First Attempt To Look For Texture In The Same Folder As The Input Folder.
    MultiByteToWideChar(CP_ACP,0,d3dxMtrls[i].pTextureFilename,-1,strTextureTemp,MAX_PATH);
    strTextureTemp[MAX_PATH-1] = 0;

    StringCchCopy((char*)strTexture,MAX_PATH,(const char*)strPath);
    StringCchCat((char*)strTexture,MAX_PATH,(const char*)strTextureTemp);

    //Inspect The Texture File To Determine The Texture Type.
    if (FAILED(D3DXGetImageInfoFromFile(LPCSTR(strTexture),&ImgInfo)))
    {
      //Search The Media Folder
      if (FAILED(DXUTFindDXSDKMediaFileCch(strTexture,MAX_PATH,strTextureTemp)))
      continue; //Can't Find. Skip.

      D3DXGetImageInfoFromFile(LPCSTR(strTexture),&ImgInfo);
    }

    //Call The Appropriate Loader According To The Texture Type.
    switch (ImgInfo.ResourceType)
    {
      case D3DRTYPE_TEXTURE:
      {
        IDirect3DTexture9* pTex;
        if (SUCCEEDED(D3DXCreateTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
      
	  case D3DRTYPE_CUBETEXTURE:
      {
        IDirect3DCubeTexture9* pTex;
        if (SUCCEEDED(D3DXCreateCubeTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
      
	  case D3DRTYPE_VOLUMETEXTURE:
      {
        IDirect3DVolumeTexture9* pTex;
        if (SUCCEEDED(D3DXCreateVolumeTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
    }*/
  }
  delete Texture_Name;
  //delete TmpName;
  delete SpecTexture;
  delete NormTexture;
  delete HeightTexture;
  return S_OK;
} //EndCreateMaterialsFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRMeshX::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRMeshX::SetFVF()
// Desc: Set Flexible Vertex Format
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::SetFVF(DWORD dwFVF)
{
  LPD3DXMESH pTempSysMemMesh = NULL;
  LPD3DXMESH pTempLocalMesh = NULL;

  if (m_pSysMemMesh)
  {
    if (FAILED(m_pSysMemMesh->CloneMeshFVF(m_pSysMemMesh->GetOptions(),dwFVF,
	d_pDevice,&pTempSysMemMesh))) return E_FAIL;
  }
   
  if (m_pLocalMesh)
  {
    if (FAILED(m_pLocalMesh->CloneMeshFVF(m_pLocalMesh->GetOptions(),dwFVF,
    d_pDevice,&pTempLocalMesh)))
    {
      ReleaseClass(pTempSysMemMesh);
      return E_FAIL;
    }
  }

  DWORD dwOldFVF = 0;

  if (m_pSysMemMesh) dwOldFVF = m_pSysMemMesh->GetFVF();

  ReleaseClass(m_pSysMemMesh);
  ReleaseClass(m_pLocalMesh);

  if (pTempSysMemMesh) m_pSysMemMesh = pTempSysMemMesh;
  if (pTempLocalMesh) m_pLocalMesh = pTempLocalMesh;

  //Compute Normals If They Are Being Requested And The Old Mesh Does Not Have Them.
  if (!(dwOldFVF & D3DFVF_NORMAL) && dwFVF & D3DFVF_NORMAL)
  {
    if (m_pSysMemMesh) D3DXComputeNormals(m_pSysMemMesh,NULL);
    if (m_pLocalMesh) D3DXComputeNormals(m_pLocalMesh,NULL);
  }
  return S_OK;
} //EndSetFVFFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::SetVertexDecl
// Desc: Convert The Mesh To The Format Specified By The Given Vertex Declarations
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::SetVertexDecl(const D3DVERTEXELEMENT9* pDecl)
{
  LPD3DXMESH pTempSysMemMesh = NULL;
  LPD3DXMESH pTempLocalMesh = NULL;

  if (m_pSysMemMesh)
  {
    if (FAILED(m_pSysMemMesh->CloneMesh(m_pSysMemMesh->GetOptions(),pDecl,
    d_pDevice,&pTempSysMemMesh))) return E_FAIL;
  }

  if (m_pLocalMesh)
  {
    if (FAILED(m_pLocalMesh->CloneMesh(m_pLocalMesh->GetOptions(),pDecl,
    d_pDevice,&pTempLocalMesh)))
    {
      ReleaseClass(pTempSysMemMesh);
      return E_FAIL;
    }
  }
  
  //Check If The Old Declaration Contains A Normal.
  bool bHadNormal = false;
  D3DVERTEXELEMENT9 aOldDecl[MAX_FVF_DECL_SIZE];
  if (m_pSysMemMesh && SUCCEEDED(m_pSysMemMesh->GetDeclaration(aOldDecl)))
  {
    for (UINT index = 0; index < D3DXGetDeclLength(aOldDecl); ++index)
    if (aOldDecl[index].Usage == D3DDECLUSAGE_NORMAL)
    {
      bHadNormal = true;
      break;
    }
  }

  //Check If The New Declaration Contains A Normal.
  bool bHaveNormalNow = false;
  D3DVERTEXELEMENT9 aNewDecl[MAX_FVF_DECL_SIZE];
  if (pTempSysMemMesh && SUCCEEDED(pTempSysMemMesh->GetDeclaration(aNewDecl)))
  {
    for (UINT index = 0; index < D3DXGetDeclLength(aNewDecl); ++index)
    if (aNewDecl[index].Usage == D3DDECLUSAGE_NORMAL)
    {
      bHaveNormalNow = true;
      break;
    }
  }

  ReleaseClass(m_pSysMemMesh);
  ReleaseClass(m_pLocalMesh);

  if (pTempSysMemMesh)
  {
    m_pSysMemMesh = pTempSysMemMesh;

    if (!bHadNormal && bHaveNormalNow)
    {
      //Compute Normals In Case The Meshes Have Them
      D3DXComputeNormals(m_pSysMemMesh,NULL);
	  D3DXComputeTangentFrame(m_pSysMemMesh,NULL);
    }
  }

  if (pTempLocalMesh)
  {
    m_pLocalMesh = pTempLocalMesh;

    if (!bHadNormal && bHaveNormalNow)
    {
      //Compute Normals In Case The Meshes Have Them
      D3DXComputeNormals(m_pLocalMesh,NULL);
	  D3DXComputeTangentFrame(m_pLocalMesh,NULL);
    }
  }

  return S_OK;
} //EndSetVertexDeclFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::RestoreObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::RestoreObjects()
{
  if (NULL == m_pSysMemMesh) return E_FAIL;

  //Make A Local Memory Version Of The Mesh. 
  //Note: Because We Are Passing In No flags, The Default 
  //Behavior Is To Clone Into Local Memory.
  if (FAILED(m_pSysMemMesh->CloneMeshFVF(
  D3DXMESH_MANAGED|(m_pSysMemMesh->GetOptions()&~D3DXMESH_SYSTEMMEM),
  m_pSysMemMesh->GetFVF(),d_pDevice,&m_pLocalMesh))) return E_FAIL;

  return S_OK;
} //EndRestoreObjectsFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::InvalidateObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::InvalidateObjects()
{
  ReleaseClass(m_pLocalMesh);
  return S_OK;
} //EndInvalidateObjectsFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshX::Render1()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::Render(bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets)
{
  if (NULL == m_pLocalMesh) return E_FAIL;

  //First, Draw The Subsets Without Alpha
  if (bDrawOpaqueSubsets)
  {
    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
      if (m_bUseMaterials)
      {
        if (m_pMaterials[i].Diffuse.a < 1.0f) continue;
        d_pDevice->SetMaterial(&m_pMaterials[i]);
        d_pDevice->SetTexture(0,m_pTextures[i]);
      }
      m_pLocalMesh->DrawSubset(i);
    }
  }

  //Then, Draw The Subsets With Alpha
  if (bDrawAlphaSubsets && m_bUseMaterials)
  {
    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
      if (m_pMaterials[i].Diffuse.a == 1.0f) continue;

      //Set The Material And Texture
      d_pDevice->SetMaterial(&m_pMaterials[i]);
      d_pDevice->SetTexture(0,m_pTextures[i]);
      m_pLocalMesh->DrawSubset(i);
    }
  }

  return S_OK;
} //EndRender1Function

//-----------------------------------------------------------------------------
// Name: CGRMeshX::Render2()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRMeshX::Render(ID3DXEffect* pEffect,
D3DXHANDLE hTexture,D3DXHANDLE hDiffuse,
D3DXHANDLE hAmbient,D3DXHANDLE hSpecular,
D3DXHANDLE hEmissive,D3DXHANDLE hPower,
bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets)
{
  if (NULL == m_pLocalMesh) return E_FAIL;

  UINT cPasses;
  //First, Draw The Subsets Without alpha
  if (bDrawOpaqueSubsets)
  {
    pEffect->Begin(&cPasses,0);
    for (UINT p = 0; p < cPasses; ++p)
    {
      pEffect->BeginPass(p);
      for (DWORD i = 0; i < m_dwNumMaterials; i++)
      {
        if (m_bUseMaterials)
        {
          if (m_pMaterials[i].Diffuse.a < 1.0f) continue;
          if (hTexture) pEffect->SetTexture(hTexture,m_pTextures[i]);
          //D3DCOLORVALUE And D3DXVECTOR4 Are Data-Wise Identical.
          //No Conversion Is Needed.
          if (hDiffuse) pEffect->SetVector(hDiffuse,(D3DXVECTOR4*)&m_pMaterials[i].Diffuse);
          if (hAmbient) pEffect->SetVector(hAmbient,(D3DXVECTOR4*)&m_pMaterials[i].Ambient);
          if (hSpecular) pEffect->SetVector(hSpecular,(D3DXVECTOR4*)&m_pMaterials[i].Specular);
          if (hEmissive) pEffect->SetVector(hEmissive,(D3DXVECTOR4*)&m_pMaterials[i].Emissive);
          if (hPower) pEffect->SetVector(hPower,(D3DXVECTOR4*)&m_pMaterials[i].Power);
          pEffect->CommitChanges();
        }
        m_pLocalMesh->DrawSubset(i);
      }
      pEffect->EndPass();
    }
    pEffect->End();
  }

  //Then, Draw The Subsets With Alpha
  if (bDrawAlphaSubsets && m_bUseMaterials)
  {
    pEffect->Begin(&cPasses,0);
    for (UINT p = 0; p < cPasses; ++p)
    {
      pEffect->BeginPass(p);
      for (DWORD i = 0; i < m_dwNumMaterials; i++)
      {
        if (m_bUseMaterials)
        {
          if (m_pMaterials[i].Diffuse.a == 1.0f) continue;
          if (hTexture) pEffect->SetTexture(hTexture,m_pTextures[i]);
          //D3DCOLORVALUE And D3DXVECTOR4 Are Data-Wise Identical.
          //No Conversion Is Needed.
          if (hDiffuse) pEffect->SetVector(hDiffuse,(D3DXVECTOR4*)&m_pMaterials[i].Diffuse);
          if (hAmbient) pEffect->SetVector(hAmbient,(D3DXVECTOR4*)&m_pMaterials[i].Ambient);
          if (hSpecular) pEffect->SetVector(hSpecular,(D3DXVECTOR4*)&m_pMaterials[i].Specular);
          if (hEmissive) pEffect->SetVector(hEmissive,(D3DXVECTOR4*)&m_pMaterials[i].Emissive);
          if (hPower) pEffect->SetVector(hPower,(D3DXVECTOR4*)&m_pMaterials[i].Power);
          pEffect->CommitChanges();
        }
        m_pLocalMesh->DrawSubset(i);
      }
      pEffect->EndPass();
    }
    pEffect->End();
  }

  return S_OK;
} //EndRender2Function

//-----------------------------------------------------------------------------