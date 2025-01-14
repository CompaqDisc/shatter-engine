// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "Renderable.h"

#include <Engine/Display/Rendering/Shader.h>
#include <Engine/Profiling/Logging.h>

#include <glm/gtc/type_ptr.hpp>

CRenderable::CRenderable()
{
	Mesh = nullptr;
	Shader = nullptr;
	memset( Textures, 0, 32 * sizeof( CTexture* ) );
}

CRenderable::~CRenderable()
{

}

CMesh* CRenderable::GetMesh()
{
	return Mesh;
}


void CRenderable::SetMesh( CMesh* Mesh )
{
	if( Mesh )
	{
		this->Mesh = Mesh;

		FVertexBufferData& Data = Mesh->GetVertexBufferData();
		RenderData.VertexBufferObject = Data.VertexBufferObject;
		RenderData.IndexBufferObject = Data.IndexBufferObject;
	}
}

CShader* CRenderable::GetShader()
{
	return Shader;
}

void CRenderable::SetShader( CShader* Shader )
{
	if( Shader )
	{
		this->Shader = Shader;

		const FProgramHandles& Handles = Shader->GetHandles();
		RenderData.ShaderProgram = Handles.Program;
	}
}

CTexture* CRenderable::GetTexture( ETextureSlot Slot )
{
	if( Slot < ETextureSlot::Maximum )
	{
		const auto Index = static_cast<ETextureSlotType>( Slot );
		return Textures[Index];
	}

	return nullptr;
}

void CRenderable::SetTexture( CTexture* Texture, ETextureSlot Slot )
{
	if( Texture && Slot < ETextureSlot::Maximum )
	{
		const auto Index = static_cast<ETextureSlotType>( Slot );
		this->Textures[Index] = Texture;
	}
}

void CRenderable::Draw( FRenderData& RenderData, const FRenderData& PreviousRenderData, EDrawMode DrawModeOverride )
{
	if( Mesh )
	{
		const EDrawMode DrawMode = DrawModeOverride != None ? DrawModeOverride : RenderData.DrawMode;
		Prepare( RenderData );

		const glm::mat4 ModelMatrix = RenderData.Transform.GetTransformationMatrix();
		GLuint ModelMatrixLocation = glGetUniformLocation( RenderData.ShaderProgram, "Model" );
		glUniformMatrix4fv( ModelMatrixLocation, 1, GL_FALSE, &ModelMatrix[0][0] );

		GLuint ColorLocation = glGetUniformLocation( RenderData.ShaderProgram, "ObjectColor" );
		glUniform4fv( ColorLocation, 1, glm::value_ptr( RenderData.Color ) );

		const FVertexBufferData& Data = Mesh->GetVertexBufferData();
		const bool BindBuffers = PreviousRenderData.VertexBufferObject != Data.VertexBufferObject || PreviousRenderData.IndexBufferObject != Data.IndexBufferObject;
		if( BindBuffers )
		{
			Mesh->Prepare( DrawMode );
		}

		Mesh->Draw( DrawMode );
	}
}

FRenderDataInstanced& CRenderable::GetRenderData()
{
	return RenderData;
}

void CRenderable::Prepare( FRenderData& RenderData )
{
	if( Shader )
	{
		const FProgramHandles& Handles = Shader->GetHandles();
		if( Textures[0] )
		{
			for( ETextureSlot Slot = ETextureSlot::Slot0; Slot < ETextureSlot::Maximum; )
			{
				const auto Index = static_cast<ETextureSlotType>( Slot );

				CTexture* Texture = Textures[Index];
				if( Texture )
				{
					glUniform1i( glGetUniformLocation( Handles.Program, TextureSlotName[Index] ), Index );
					Texture->Bind( Slot );
				}

				Slot = static_cast<ETextureSlot>( Index + 1 );
			}
		}
	}
}
