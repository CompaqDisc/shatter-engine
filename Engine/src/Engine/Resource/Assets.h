// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include <Engine/Display/Rendering/TextureEnumerators.h>
#include <Engine/Utility/Primitive.h>

class CMesh;
class CShader;
class CTexture;
class CSound;
class CSequence;

struct FPrimitivePayload
{
	std::string Name;
	std::string Location;
	FPrimitive Primitive;
	bool Native;
};

namespace EAsset
{
	enum Type
	{
		Unknown = 0,
		Mesh,
		Shader,
		Texture,
		Sound,
		Sequence
	};
}

struct FGenericAssetPayload
{
	EAsset::Type Type;
	std::string Name;
	std::vector<std::string> Locations;
};

class CAssets
{
public:
	void Create( const std::string& Name, CMesh* NewMesh );
	void Create( const std::string& Name, CShader* NewShader );
	void Create( const std::string& Name, CTexture* NewTexture );
	void Create( const std::string& Name, CSound* NewSound );
	void Create( const std::string& Name, CSequence* NewSequence );

	void CreatedNamedAssets( std::vector<FPrimitivePayload>& Meshes, std::vector<FGenericAssetPayload>& GenericAssets );

	CMesh* CreateNamedMesh( const char* Name, const char* FileLocation, const bool ForceLoad = false );
	CMesh* CreateNamedMesh( const char* Name, const FPrimitive& Primitive );
	CShader* CreateNamedShader( const char* Name, const char* FileLocation );
	CShader* CreateNamedShader( const char* Name, const char* VertexLocation, const char* FragmentLocation );
	CTexture* CreateNamedTexture( const char* Name, const char* FileLocation, const EFilteringMode Mode = EFilteringMode::Linear, const EImageFormat Format = EImageFormat::RGB8 );
	CTexture* CreateNamedTexture( const char* Name, unsigned char* Data, const int Width, const int Height, const int Channels, const EFilteringMode Mode = EFilteringMode::Linear, const EImageFormat Format = EImageFormat::RGB8 );
	CSound* CreateNamedSound( const char* Name, const char* FileLocation );
	CSound* CreateNamedSound( const char* Name );

	CSound* CreateNamedStream( const char* Name, const char* FileLocation );
	CSound* CreateNamedStream( const char* Name );

	CSequence* CreateNamedSequence( const char* Name, const char* FileLocation );
	CSequence* CreateNamedSequence( const char* Name );

	CMesh* FindMesh( const std::string& Name );
	CShader* FindShader( const std::string& Name );
	CTexture* FindTexture( const std::string& Name );
	CSound* FindSound( const std::string& Name );
	CSequence* FindSequence( const std::string& Name );

	const std::string& GetReadableImageFormat( EImageFormat Format );

	template<class T>
	inline T* Find( const std::string& Name, std::unordered_map<std::string, T*> Data )
	{
		if( Data.find( Name ) != Data.end() )
		{
			return Data[Name];
		}

		return nullptr;
	};

	void ReloadShaders();

	const std::unordered_map<std::string, CMesh*>& GetMeshes() const
	{
		return Meshes;
	}

	const std::unordered_map<std::string, CShader*>& GetShaders() const
	{
		return Shaders;
	}

	const std::unordered_map<std::string, CTexture*>& GetTextures() const
	{
		return Textures;
	}

	const std::unordered_map<std::string, CSound*>& GetSounds() const
	{
		return Sounds;
	}

	const std::unordered_map<std::string, CSequence*>& GetSequences() const
	{
		return Sequences;
	}

private:
	std::unordered_map<std::string, CMesh*> Meshes;
	std::unordered_map<std::string, CShader*> Shaders;
	std::unordered_map<std::string, CTexture*> Textures;
	std::unordered_map<std::string, CSound*> Sounds;
	std::unordered_map<std::string, CSequence*> Sequences;

public:
	static CAssets& Get()
	{
		static CAssets StaticInstance;
		return StaticInstance;
	}
private:
	CAssets();

	CAssets( CAssets const& ) = delete;
	void operator=( CAssets const& ) = delete;
};
