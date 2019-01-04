#version 330

layout (location = 0) in vec3 Position;

out vec3 WorldPosition;

uniform mat4 Model;
uniform mat4 ModelViewProjection;

void main()
{
	WorldPosition = (Model * vec4(Position, 1.0)).xyz;
    gl_Position = ModelViewProjection * vec4( Position, 1.0 );
}