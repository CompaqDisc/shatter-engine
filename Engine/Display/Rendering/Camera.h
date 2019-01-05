// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include "glm/glm.hpp"
#include <Engine/Utility/Math.h>

struct FCameraSetup
{
	FCameraSetup()
	{
		FieldOfView = 60.0f;
		AspectRatio = 1280.0f / 720.0f;
		NearPlaneDistance = 1.0f;
		FarPlaneDistance = 100000.0f;
		CameraPosition = glm::vec3( 0.0f, 0.0f, -600.0f );
		//CameraPosition = glm::vec3( 4.0f, 3.0f, 3.0f );
		CameraDirection = glm::vec3( 0.0f, 0.0f, 1.0f );
		CameraUpVector = glm::vec3( 0.0f, 1.0f, 0.0f );
	};

	float FieldOfView;
	float AspectRatio;
	float NearPlaneDistance;
	float FarPlaneDistance;
	glm::vec3 CameraPosition;
	glm::vec3 CameraDirection;
	glm::vec3 CameraUpVector;
};

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update();

	void SetFieldOfView( float& FieldOfView );
	void SetAspectRatio( float& AspectRatio);
	void SetNearPlaneDistance( float& NearPlaneDistance );
	void SetFarPlaneDistance( float& FarPlaneDistance );
	void SetCameraPosition( glm::vec3& CameraPosition );
	void SetCameraDirection( glm::vec3& CameraDirection );
	void SetCameraUpVector( glm::vec3& CameraUpVector );

	glm::mat4& GetProjectionMatrix();
	glm::mat4& GetViewMatrix();
	glm::mat4& GetViewProjectionInverse();

	FCameraSetup& GetCameraSetup();
private:
	FCameraSetup CameraSetup;
	Math::FFrustum Frustum;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	glm::mat4 ProjectionViewInverseMatrix;
};
