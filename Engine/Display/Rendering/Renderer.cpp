// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "Renderer.h"

#include <algorithm>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/quaternion.hpp>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <Engine/Profiling/Logging.h>
#include <Engine/Profiling/Profiling.h>

#include "Renderable.h"
#include "Camera.h"

static const size_t nRenderableCapacity = 512;
static const size_t nTemporaryMeshCapacity = nRenderableCapacity;

CRenderer::CRenderer()
{
	Renderables.reserve( nRenderableCapacity );
	TemporaryMeshes.reserve( nTemporaryMeshCapacity );
}

CRenderer::~CRenderer()
{
	Renderables.clear();
}

void CRenderer::Initialize()
{
	CreateNamedShader( "Default", "Shaders/default" );

	static const uint32_t TriangleVertexCount = 3;
	static glm::vec3 TriangleVertices[TriangleVertexCount] =
	{
		glm::vec3( -1.0f, -1.0f, 0.0f ),
		glm::vec3( 1.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
	};

	Log::Event( "Vertex data:\n" );
	for( int i = 0; i < TriangleVertexCount; i++ )
	{
		Log::Event( "\t%f %f %f\n", TriangleVertices[i][0], TriangleVertices[i][1], TriangleVertices[i][2] );
	}

	CreateNamedMesh( "triangle", TriangleVertices, TriangleVertexCount );
	CreateNamedMesh( "Triangle", TriangleVertices, TriangleVertexCount );

	static const uint32_t SquareVertexCount = 4;
	static glm::vec3 SquareVertices[SquareVertexCount] =
	{
		glm::vec3( -1.0f, -1.0f, 0.0f ), // Bottom-left
		glm::vec3( 1.0f, -1.0f, 0.0f ), // Bottom-right
		glm::vec3( 1.0f, 1.0f, 0.0f ), // Top-right
		glm::vec3( -1.0f, 1.0f, 0.0f ), // Top-left
	};

	static const uint32_t SquareIndexCount = 6;
	static glm::uint SquareIndices[SquareIndexCount] =
	{
		2, 1, 0, // Top-right, Bottom-right, Bottom-left
		0, 3, 2, // 
	};

	CreateNamedMesh( "square", SquareVertices, SquareVertexCount, SquareIndices, SquareIndexCount );

	static const uint32_t LineSquareIndexCount = 5;
	static glm::uint LineSquareIndices[LineSquareIndexCount] =
	{
		2, 1, 0, // Top-right, Bottom-right, Bottom-left
		0, 3, // 
	};

	CMesh* LineSquareMesh = CreateNamedMesh( "LineSquare", SquareVertices, SquareVertexCount, LineSquareIndices, LineSquareIndexCount );
	if( LineSquareMesh )
	{
		FVertexBufferData& VertexBufferData = LineSquareMesh->GetVertexBufferData();
		VertexBufferData.DrawMode = GL_LINE_LOOP;
	}

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
}

CMesh* CRenderer::CreateNamedMesh( const char* Name, glm::vec3* Vertices, uint32_t VertexCount )
{
	// Transform given name into lower case string
	std::string NameString = Name;
	std::transform( NameString.begin(), NameString.end(), NameString.begin(), ::tolower );
	
	// Check if the mesh exists
	if( CMesh* ExistingMesh = FindMesh( NameString ) )
	{
		Log::Event( "Found existing mesh named \"%s\"\n", NameString );
		return ExistingMesh;
	}

	// Create a new mesh
	CMesh* NewMesh = new CMesh();
	NewMesh->Populate( Vertices, VertexCount );

	Meshes.insert_or_assign( NameString, NewMesh );

	return NewMesh;
}

CMesh* CRenderer::CreateNamedMesh( const char* Name, glm::vec3* Vertices, uint32_t VertexCount, glm::uint* Indices, uint32_t IndexCount )
{
	// Transform given name into lower case string
	std::string NameString = Name;
	std::transform( NameString.begin(), NameString.end(), NameString.begin(), ::tolower );

	// Check if the mesh exists
	if( CMesh* ExistingMesh = FindMesh( NameString ) )
	{
		Log::Event( "Found existing mesh named \"%s\"\n", NameString );
		return ExistingMesh;
	}

	// Create a new mesh
	CMesh* NewMesh = new CMesh();
	const bool bSuccessfulCreation = NewMesh->Populate( Vertices, VertexCount, Indices, IndexCount );

	if( bSuccessfulCreation )
	{
		Meshes.insert_or_assign( NameString, NewMesh );

		return NewMesh;
	}

	// This should never happen because we check for existing meshes before creating new ones, but you never know
	return nullptr;
}

CMesh* CRenderer::CreateTemporaryMesh( glm::vec3* Vertices, uint32_t VertexCount )
{
	// Create a new mesh
	CMesh* NewMesh = new CMesh();
	NewMesh->Populate( Vertices, VertexCount );

	TemporaryMeshes.push_back( NewMesh );

	return NewMesh;
}

CShader* CRenderer::CreateNamedShader( const char* Name, const char* FileLocation )
{
	// Transform given name into lower case string
	std::string NameString = Name;
	std::transform( NameString.begin(), NameString.end(), NameString.begin(), ::tolower );

	// Check if the mesh exists
	if( CShader* ExistingShader = FindShader( NameString ) )
	{
		Log::Event( "Found existing shader named \"%s\"\n", NameString );
		return ExistingShader;
	}

	CShader* NewShader = new CShader();
	const bool bSuccessfulCreation = NewShader->Load( FileLocation );

	if( bSuccessfulCreation )
	{
		Shaders.insert_or_assign( NameString, NewShader );

		return NewShader;
	}

	// This should never happen because we check for existing shaders before creating new ones, but you never know
	return nullptr;
}

CMesh* CRenderer::FindMesh( std::string Name )
{
	return Find<CMesh>( Name, Meshes );
}

CShader* CRenderer::FindShader( std::string Name )
{
	return Find<CShader>( Name, Shaders );
}

void CRenderer::RefreshFrame()
{
	// Clean up the renderable queue
	Renderables.clear();

	// Clean up dynamic renderables
	for( auto Renderable : DynamicRenderables )
	{
		delete Renderable;
	}
	DynamicRenderables.clear();

	// Clear out all temporary meshes
	TemporaryMeshes.clear();
}

void CRenderer::QueueRenderable( CRenderable* Renderable )
{
	Renderables.push_back( Renderable );
}

void CRenderer::QueueDynamicRenderable( CRenderable* Renderable )
{
	DynamicRenderables.push_back( Renderable );
}

static CShader* DefaultShader = nullptr;
GLuint LastProgramHandle = -1;

void CRenderer::DrawQueuedRenderables()
{
	if( !DefaultShader )
	{
		DefaultShader = FindShader( "default" );
	}

	if( DefaultShader->Handle != LastProgramHandle )
	{
		LastProgramHandle = DefaultShader->Activate();
	}

	FCameraSetup& CameraSetup = Camera.GetCameraSetup();

	glm::mat4 ProjectionMatrix = glm::perspective( glm::radians( CameraSetup.FieldOfView ), CameraSetup.AspectRatio, CameraSetup.NearPlaneDistance, CameraSetup.FarPlaneDistance );

	glm::vec3 CameraPosition = CameraSetup.CameraPosition;
	CameraPosition.x = -CameraPosition.x;

	glm::mat4 ViewMatrix = glm::lookAt(
		CameraPosition,
		CameraPosition + CameraSetup.CameraDirection,
		CameraSetup.CameraUpVector
		);

	int64_t DrawCalls = 0;

	for( auto Renderable : Renderables )
	{
		FRenderDataInstanced RenderData = Renderable->GetRenderData();
		RenderData.ShaderProgram = DefaultShader->Handle;

		glm::mat4 ModelMatrix = glm::mat4( 1.0f );

		ModelMatrix = glm::translate( ModelMatrix, RenderData.Position );

		static const glm::vec3 AxisX = glm::vec3( 1.0f, 0.0f, 0.0f );
		static const glm::vec3 AxisY = glm::vec3( 0.0f, 1.0f, 0.0f );
		static const glm::vec3 AxisZ = glm::vec3( 0.0f, 0.0f, 1.0f );
		
		const glm::quat ModelQuaternion = glm::quat( RenderData.Orientation );
		const glm::mat4 RotationMatrix = glm::toMat4( ModelQuaternion );

		ModelMatrix *= RotationMatrix;

		ModelMatrix = glm::scale( ModelMatrix, RenderData.Size );

		glm::mat4 ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

		GLuint MatrixLocation = glGetUniformLocation( RenderData.ShaderProgram, "ModelViewProjection" );
		glUniformMatrix4fv( MatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0] );

		GLuint ColorLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectColor" );
		glUniform4fv( ColorLocation, 1, glm::value_ptr( RenderData.Color ) );

		Renderable->Draw();
		DrawCalls++;
	}

	for( auto Renderable : DynamicRenderables )
	{
		FRenderDataInstanced RenderData = Renderable->GetRenderData();
		RenderData.ShaderProgram = DefaultShader->Handle;

		glm::mat4 ModelMatrix = glm::mat4( 1.0f );

		ModelMatrix = glm::translate( ModelMatrix, RenderData.Position );
		ModelMatrix = glm::scale( ModelMatrix, RenderData.Size );

		glm::mat4 ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

		GLuint MatrixLocation = glGetUniformLocation( RenderData.ShaderProgram, "ModelViewProjection" );
		glUniformMatrix4fv( MatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0] );

		GLuint ColorLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectColor" );
		glUniform4fv( ColorLocation, 1, glm::value_ptr( RenderData.Color ) );

		Renderable->Draw();
		DrawCalls++;
	}

	CProfileVisualisation& Profiler = CProfileVisualisation::GetInstance();

	Profiler.AddCounterEntry( FProfileTimeEntry( "Draw Calls", DrawCalls ), true );

	int64_t RenderablesSize = static_cast<int64_t>( Renderables.size() );
	Profiler.AddCounterEntry( FProfileTimeEntry( "Renderables", RenderablesSize ), true );

	int64_t DynamicRenderablesSize = static_cast<int64_t>( DynamicRenderables.size() );
	Profiler.AddCounterEntry( FProfileTimeEntry( "Renderables (Dynamic)", DynamicRenderablesSize ), true );
}

void CRenderer::ReloadShaders()
{
	Profile( __FUNCTION__ );
	Log::Event( "Reloading shaders.\n" );

	for( auto Shader : Shaders )
	{
		Shader.second->Reload();
	}
}

void CRenderer::SetCamera( CCamera& CameraIn )
{
	Camera = CameraIn;
}

size_t CRenderer::MeshCount() const
{
	return Meshes.size();
}
