// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "RenderPass.h"

#include <Engine/Display/Rendering/Camera.h>
#include <Engine/Display/Rendering/Mesh.h>
#include <Engine/Display/Rendering/Shader.h>
#include <Engine/Display/Rendering/Texture.h>
#include <Engine/Display/Rendering/RenderTexture.h>
#include <Engine/Profiling/Profiling.h>
#include <Engine/Utility/Math.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint ShaderProgramHandle = -1;

static const GLenum DepthTestToEnum[EDepthTest::Maximum]
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

CRenderPass::CRenderPass( const std::string& Name, int Width, int Height, const CCamera& CameraIn, const bool AlwaysClearIn )
{
	ViewportWidth = Width;
	ViewportHeight = Height;
	Camera = CameraIn;
	AlwaysClear = AlwaysClearIn;
	Target = nullptr;
	BlendMode = EBlendMode::Opaque;
	DepthMask = EDepthMask::Write;
	DepthTest = EDepthTest::Less;
	PassName = Name;
}

uint32_t CRenderPass::RenderRenderable( CRenderable* Renderable )
{
	Profile( PassName.c_str() );
	Begin();

	Draw( Renderable );

	End();

	return Calls;
}

uint32_t CRenderPass::RenderRenderable( CRenderable* Renderable, const std::unordered_map<std::string, Vector4D>& Uniforms )
{
	Profile( PassName.c_str() );
	Begin();

	Setup( Renderable, Uniforms );
	Draw( Renderable );

	End();

	return Calls;
}

uint32_t CRenderPass::Render( const std::vector<CRenderable*>& Renderables )
{
	Profile( PassName.c_str() );
	Begin();

	for( auto Renderable : Renderables )
	{
		Draw( Renderable );
	}

	End();

	return Calls;
}

uint32_t CRenderPass::Render( const std::vector<CRenderable*>& Renderables, const std::unordered_map<std::string, Vector4D>& Uniforms )
{
	Profile( PassName.c_str() );
	Begin();

	for( auto Renderable : Renderables )
	{
		Setup( Renderable, Uniforms );
		Draw( Renderable);
	}

	End();

	return Calls;
}

uint32_t CRenderPass::Render( const std::unordered_map<std::string, Vector4D>& Uniforms )
{
	Log::Event( Log::Warning, "CRenderPass doesn't have a Render() implementation.\n" );
	return 0;
}

void CRenderPass::Clear()
{
	Begin();

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClearDepth( 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	End();
}

void CRenderPass::Begin()
{
	Calls = 0;
	glViewport( 0, 0, ViewportWidth, ViewportHeight );

	// Reset the render data.
	PreviousRenderData = FRenderDataInstanced();

	if( Target )
	{
		if( !Target->Ready() )
		{
			Target->Initialize();
		}

		if( Target->Ready() )
		{
			Target->Push();
		}
	}

	if( AlwaysClear )
	{
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClearDepth( 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	}

	glDepthMask( DepthMask == EDepthMask::Write ? GL_TRUE : GL_FALSE );
	glDepthFunc( DepthTestToEnum[DepthTest] );
}

void CRenderPass::End()
{
	glDisable( GL_BLEND );

	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LESS );

	if( Target && Target->Ready() )
	{
		Target->Pop();
	}
}

void CRenderPass::Setup( CRenderable* Renderable, const std::unordered_map<std::string, Vector4D>& Uniforms )
{
	FRenderDataInstanced& RenderData = Renderable->GetRenderData();

	CShader* Shader = Renderable->GetShader();
	if( Shader )
	{
		const FProgramHandles& Handles = Shader->GetHandles();
		if( Handles.Program != ShaderProgramHandle )
		{
			ShaderProgramHandle = Shader->Activate();
		}

		RenderData.ShaderProgram = ShaderProgramHandle;

		for( auto& UniformBuffer : Uniforms )
		{
			const GLint UniformBufferLocation = glGetUniformLocation( RenderData.ShaderProgram, UniformBuffer.first.c_str() );
			if( UniformBufferLocation > -1 )
			{
				glUniform4fv( UniformBufferLocation, 1, UniformBuffer.second.Base() );
			}
		}
	}
}

void CRenderPass::Draw( CRenderable* Renderable )
{
	CShader* Shader = Renderable->GetShader();
	if( Shader )
	{
		ConfigureBlendMode( Shader );
		ConfigureDepthMask( Shader );
		ConfigureDepthTest( Shader );

		FRenderDataInstanced& RenderData = Renderable->GetRenderData();

		const FProgramHandles& Handles = Shader->GetHandles();
		if( Handles.Program != ShaderProgramHandle )
		{
			ShaderProgramHandle = Shader->Activate();
		}

		RenderData.ShaderProgram = ShaderProgramHandle;

		const FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		const glm::mat4& ViewMatrix = Camera.GetViewMatrix();
		const glm::mat4& ProjectionMatrix = Camera.GetProjectionMatrix();

		const GLint ViewMatrixLocation = glGetUniformLocation( RenderData.ShaderProgram, "View" );
		if( ViewMatrixLocation > -1 )
		{
			glUniformMatrix4fv( ViewMatrixLocation, 1, GL_FALSE, &ViewMatrix[0][0] );
		}

		const GLint ProjectionMatrixLocation = glGetUniformLocation( RenderData.ShaderProgram, "Projection" );
		if( ProjectionMatrixLocation > -1 )
		{
			glUniformMatrix4fv( ProjectionMatrixLocation, 1, GL_FALSE, &ProjectionMatrix[0][0] );
		}

		const GLint CameraPositionLocation = glGetUniformLocation( RenderData.ShaderProgram, "CameraPosition" );
		if( CameraPositionLocation > -1 )
		{
			glUniform3fv( CameraPositionLocation, 1, CameraSetup.CameraPosition.Base() );
		}

		const GLint CameraDirectionLocation = glGetUniformLocation( RenderData.ShaderProgram, "CameraDirection" );
		if( CameraDirectionLocation > -1 )
		{
			glUniform3fv( CameraDirectionLocation, 1, CameraSetup.CameraDirection.Base() );
		}

		const GLint ObjectPositionLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectPosition" );
		if( ObjectPositionLocation > -1 )
		{
			glUniform3fv( ObjectPositionLocation, 1, RenderData.Transform.GetPosition().Base() );
		}

		CMesh* Mesh = Renderable->GetMesh();
		if( Mesh )
		{
			auto& AABB = Mesh->GetBounds();
			const GLint ObjectBoundsMinimumLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectBoundsMinimum" );
			if( ObjectBoundsMinimumLocation > -1 )
			{
				glUniform3fv( ObjectBoundsMinimumLocation, 1, AABB.Minimum.Base() );
			}

			const GLint ObjectBoundsMaximumLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectBoundsMaximum" );
			if( ObjectBoundsMaximumLocation > -1 )
			{
				glUniform3fv( ObjectBoundsMaximumLocation, 1, AABB.Maximum.Base() );
			}
		}

		// Viewport coordinates
		{
			const glm::vec4 Viewport = glm::vec4( ViewportWidth, ViewportHeight, 1.0f / ViewportWidth, 1.0f / ViewportHeight );
			const GLint ViewportLocation = glGetUniformLocation( RenderData.ShaderProgram, "Viewport" );
			if( ViewportLocation > -1 )
			{
				glUniform4fv( ViewportLocation, 1, glm::value_ptr( Viewport ) );
			}
		}

		Renderable->Draw( RenderData, PreviousRenderData );
		PreviousRenderData = RenderData;

		Calls++;
	}
}

void CRenderPass::SetCamera( const CCamera& CameraIn )
{
	Camera = CameraIn;
}

void CRenderPass::ConfigureBlendMode( CShader* Shader )
{
	auto NextBlendMode = Shader->GetBlendMode();
	if( NextBlendMode != BlendMode )
	{
		if( NextBlendMode == EBlendMode::Opaque )
		{
			glDisable( GL_BLEND );
		}
		else if( NextBlendMode == EBlendMode::Alpha )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		else if( NextBlendMode == EBlendMode::Additive )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
		}

		BlendMode = NextBlendMode;
	}
}

void CRenderPass::ConfigureDepthMask( CShader* Shader )
{
	auto NextDepthMask = Shader->GetDepthMask();
	if( NextDepthMask != DepthMask )
	{
		glDepthMask( NextDepthMask == EDepthMask::Write ? GL_TRUE : GL_FALSE );
		DepthMask = NextDepthMask;
	}
}

void CRenderPass::ConfigureDepthTest( CShader* Shader )
{
	auto NextDepthTest = Shader->GetDepthTest();
	if( NextDepthTest != DepthTest )
	{
		glDepthFunc( DepthTestToEnum[NextDepthTest] );

		DepthTest = NextDepthTest;
	}
}
