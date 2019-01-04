// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Mesh.h"
#include "Shader.h"

struct FRenderData
{
	FVertexBufferData VertexBufferData;
	GLuint ShaderProgram = 0;

	glm::vec3 Position;
	glm::vec3 Orientation;
	glm::vec3 Size;
	glm::vec4 Color;

	FVertexData *VertexData = nullptr;
	EDrawMode DrawMode = None;
};

struct FRenderDataInstanced : public FRenderData
{
	GLuint PositionBufferObject = 0;
	GLuint ColorBufferObject = 0;
};

class CRenderable
{
public:
	CRenderable();
	~CRenderable();

	CMesh* GetMesh();
	void SetMesh( CMesh* Mesh );

	CShader* GetShader();
	void SetShader( CShader* Shader );

	virtual void Draw();

	FRenderDataInstanced& GetRenderData();
private:
	CShader* Shader;
	CMesh* Mesh;

	FRenderDataInstanced RenderData;
};
