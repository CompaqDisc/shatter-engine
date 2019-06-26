// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <Engine/Utility/Math.h>
#include <Engine/Display/Rendering/Camera.h>

namespace UI
{
	namespace Color
	{
		enum Type
		{
			Red,
			Green,
			Blue,
			White,
			Black
		};
	}

	Vector3D ScreenPositionToWorld( const Vector2D& ScreenPosition );
	Vector2D WorldToScreenPosition( const Vector3D& WorldPosition, bool* IsInFront = nullptr );

	void AddLine( const Vector2D& Start, const Vector2D& End, Color::Type Color = Color::Green );
	void AddLine( const Vector3D& Start, const Vector3D& End, Color::Type Color = Color::Green );
	// void AddRect( const Vector2D& a, const Vector2D& b, uint32_t col, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f );   // a: upper-left, b: lower-right, rounding_corners_flags: 4-bits corresponding to which corner to round
	// void AddRectFilled( const Vector2D& a, const Vector2D& b, uint32_t col, float rounding = 0.0f, int rounding_corners_flags = ~0 );                     // a: upper-left, b: lower-right
	// void AddRectFilledMultiColor( const Vector2D& a, const Vector2D& b, uint32_t col_upr_left, uint32_t col_upr_right, uint32_t col_bot_right, uint32_t col_bot_left );
	// void AddQuad( const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, uint32_t col, float thickness = 1.0f );
	// void AddQuadFilled( const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, uint32_t col );
	// void AddTriangle( const Vector2D& a, const Vector2D& b, const Vector2D& c, uint32_t col, float thickness = 1.0f );
	// void AddTriangleFilled( const Vector2D& a, const Vector2D& b, const Vector2D& c, uint32_t col );
	void AddCircle( const Vector2D& Position, float Radius, Color::Type Color = Color::Red );
	void AddCircle( const Vector3D& Position, float Radius, Color::Type Color = Color::Red );
	// void AddCircleFilled( const Vector2D& centre, float radius, uint32_t col, int num_segments = 12 );
	void AddText( const Vector2D& Position, const char* Start, const char* End = nullptr, Color::Type Color = Color::White );
	void AddText( const Vector3D& Position, const char* Start, const char* End = nullptr, Color::Type Color = Color::White );
	// void AddText( const ImFont* font, float font_size, const Vector2D& pos, uint32_t col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const Vector4D * cpu_fine_clip_rect = NULL );
	// void AddImage( ImTextureID user_texture_id, const Vector2D& a, const Vector2D& b, const Vector2D& uv_a = Vector2D( 0, 0 ), const Vector2D& uv_b = Vector2D( 1, 1 ), uint32_t col = 0xFFFFFFFF );
	// void AddImageQuad( ImTextureID user_texture_id, const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& d, const Vector2D& uv_a = Vector2D( 0, 0 ), const Vector2D& uv_b = Vector2D( 1, 0 ), const Vector2D& uv_c = Vector2D( 1, 1 ), const Vector2D& uv_d = Vector2D( 0, 1 ), uint32_t col = 0xFFFFFFFF );
	// void AddPolyline( const Vector2D* points, const int num_points, uint32_t col, bool closed, float thickness, bool anti_aliased );
	// void AddConvexPolyFilled( const Vector2D* points, const int num_points, uint32_t col, bool anti_aliased );
	// void AddBezierCurve( const Vector2D& pos0, const Vector2D& cp0, const Vector2D& cp1, const Vector2D& pos1, uint32_t col, float thickness, int num_segments = 0 );

	void AddAABB( const Vector3D& Minimum, const Vector3D& Maximum, Color::Type Color = Color::Blue );
	void AddBox( const Vector3D& Center, const Vector3D& Size, Color::Type Color = Color::Blue );

	void Reset();

	void Refresh();
	void Frame();

	void SetCamera( const CCamera& Camera );
}