// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "SampleProject.h"

#include <cstdlib>

// Engine includes
#include <Engine/Application/Application.h>
#include <Engine/Audio/SimpleSound.h>
#include <Engine/Audio/Sound.h>
#include <Engine/Configuration/Configuration.h>
#include <Engine/Display/Window.h>
#include <Engine/Display/Rendering/Renderable.h>
#include <Engine/Utility/Locator/InputLocator.h>
#include <Engine/Resource/Assets.h>
#include <Engine/Profiling/Logging.h>
#include <Engine/Profiling/Profiling.h>
#include <Engine/Utility/Locator/InputLocator.h>
#include <Engine/Utility/Presence/Precense.h>
#include <Engine/World/World.h>
#include <Engine/World/Entity/MeshEntity/MeshEntity.h>

#include "Game/Game.h"

#if defined( IMGUI_ENABLED )
#include <imgui.h>
#endif

#if defined( ConsoleWindowDisabled )
#if defined( _MSC_VER )
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Game includes


CApplication Application;
CGameSample* GameInstance = new CGameSample();

CGameSample::CGameSample()
{
	World = nullptr;
}

CGameSample::~CGameSample()
{

}

static Vector3D CameraMoveDirection;
static float CameraSpeed = 10.0f;

static bool PositionInitialized = false;
static FFixedPosition2D PreviousMousePosition;
static float MouseSensitivity = 5.0f;

static float PreviousMouseDeltaX = 0.0f;
static float PreviousMouseDeltaY = 0.0f;

static Vector3D DefaultCameraPosition = Vector3D( 0.0f, 0.0f, 0.0f );
static Vector3D DefaultCameraDirection = Vector3D( -0.991688f, -0.060127f, 0.113748f );

// This function runs on startup.
void CGameSample::Initialize()
{
	Application.RegisterDebugUI( [this] ( bool Menu ) {DrawDebugMenu( Menu ); } );
	Application.EnableTools( true ); // Display toolbar.

	// Create the world.
	World = new CWorld();

	auto& Level = World->Add();
	CTimer LoadTimer;
	LoadTimer.Start();

	// Load the level.
	CFile LevelFile( "Levels/SampleLevel.sls" );
	if( LevelFile.Exists() )
	{
		LevelFile.Load();
		Level.Load( LevelFile );
	}

	LoadTimer.Stop();

	Log::Event( "Loaded level: %ims\n", LoadTimer.GetElapsedTimeMilliseconds() );

	World->Construct();

	// Configure camera.
	const float NearPlane = CConfiguration::Get().GetFloat( "defaultnearplane", 0.1f );
	const float FarPlane = CConfiguration::Get().GetFloat( "defaultfarplane", 2000.0f );
	const float AspectRatio = CConfiguration::Get().GetFloat( "width" ) / CConfiguration::Get().GetFloat( "height" );
	MouseSensitivity = CConfiguration::Get().GetFloat( "sensitivity", 5.0f );

	CameraSpeed = CConfiguration::Get().GetFloat( "defaultcameraspeed", 10.0f );

	Camera.SetCameraPosition( DefaultCameraPosition );
	Camera.SetCameraDirection( DefaultCameraDirection );
	Camera.SetNearPlaneDistance( NearPlane );
	Camera.SetFarPlaneDistance( FarPlane );
	Camera.SetAspectRatio( AspectRatio );
	Camera.SetFieldOfView( 65.0f );

	// Setup input bindings.
	IInput& Input = CInputLocator::Get();
	Input.AddActionBinding( EKey::W, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection += CameraSetup.CameraDirection * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::S, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection -= CameraSetup.CameraDirection * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::A, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection += CameraSetup.CameraRightVector * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::D, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection -= CameraSetup.CameraRightVector * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::Q, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection -= CameraSetup.CameraUpVector * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::E, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		CameraMoveDirection += CameraSetup.CameraUpVector * CameraSpeed;
	} );

	Input.AddActionBinding( EKey::C, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		Camera.SetFieldOfView( CameraSetup.FieldOfView - 0.25f );
	} );

	Input.AddActionBinding( EKey::Z, EAction::Press, [this] {
		FCameraSetup& CameraSetup = Camera.GetCameraSetup();
		Camera.SetFieldOfView( CameraSetup.FieldOfView + 0.25f );
	} );

	Input.AddActionBinding( EMouse::MouseScrollUp, EAction::Release, [&] {
		CameraSpeed *= 1.2f;
	} );

	Input.AddActionBinding( EMouse::MouseScrollDown, EAction::Release, [&] {
		CameraSpeed *= 0.8f;
		} );
}

// This function runs every frame.
void CGameSample::Frame()
{
	FCameraSetup& CameraSetup = Camera.GetCameraSetup();

	auto DebugVector = [] ( const char* Name, Vector3D& Vector )
	{
		std::stringstream DebugString;
		DebugString << Vector[0] << " " << Vector[1] << " " << Vector[2];
		CProfiler::Get().AddDebugMessage( Name, DebugString.str().c_str() );
	};

	DebugVector( "CameraPosition", CameraSetup.CameraPosition );
	DebugVector( "CameraDirection", CameraSetup.CameraDirection );
	DebugVector( "CameraRightVector", CameraSetup.CameraRightVector );
	DebugVector( "CameraUpVector", CameraSetup.CameraUpVector );
	DebugVector( "CameraTarget", CameraMoveDirection );

	if( World )
	{
		World->Frame();
	}
}

// This function runs every tick.
void CGameSample::Tick()
{
	CRenderer& Renderer = CWindow::Get().GetRenderer();
	CAssets& Assets = CAssets::Get();

	// Calling this tick here is required for now. (isn't handled by the engine yet)
	CSimpleSound::Tick();

	const float CurrentTime = static_cast<float>( GameLayersInstance->GetCurrentTime() );
	const float DeltaTime = static_cast<float>( GameLayersInstance->GetDeltaTime() );

	FCameraSetup& CameraSetup = Camera.GetCameraSetup();

	Vector3D CameraMove = CameraMoveDirection * DeltaTime;
	Camera.SetCameraPosition( CameraSetup.CameraPosition + CameraMove );
	CameraMoveDirection *= 0.9f;

	// Pressing - on the Numpad will allow you to show and hide the cursor and take control of the camera.
	if( !CWindow::Get().IsCursorEnabled() )
	{
		IInput& Input = CInputLocator::Get();
		FFixedPosition2D MousePosition = Input.GetMousePosition();

		if( !PositionInitialized )
		{
			PositionInitialized = true;
			PreviousMousePosition = MousePosition;
		}

		const float MouseDeltaX = ( static_cast<float>( MousePosition.X - PreviousMousePosition.X ) + PreviousMouseDeltaX ) * 0.5f;
		const float MouseDeltaY = ( static_cast<float>( PreviousMousePosition.Y - MousePosition.Y ) + PreviousMouseDeltaY ) * 0.5f;
		PreviousMouseDeltaX = MouseDeltaX;
		PreviousMouseDeltaY = MouseDeltaY;

		Camera.CameraOrientation[0] += MouseDeltaY * MouseSensitivity * DeltaTime;
		Camera.CameraOrientation[1] += MouseDeltaX * MouseSensitivity * DeltaTime;

		Camera.CameraOrientation[0] = fmin( Camera.CameraOrientation[0], 89.0f );
		Camera.CameraOrientation[0] = fmax( Camera.CameraOrientation[0], -89.0f );
		Camera.SetCameraOrientation( Camera.CameraOrientation );

		PreviousMousePosition = MousePosition;
	}

	if( !Application.ToolsEnabled() )
	{
		if( CWindow::Get().IsCursorEnabled() )
		{
			CWindow::Get().EnableCursor( false );

			IInput& Input = CInputLocator::Get();
			FFixedPosition2D MousePosition = Input.GetMousePosition();
			PreviousMousePosition = MousePosition;
		}
	}
	else
	{
		if( !CWindow::Get().IsCursorEnabled() )
		{
			CWindow::Get().EnableCursor( true );

			IInput& Input = CInputLocator::Get();
			FFixedPosition2D MousePosition = Input.GetMousePosition();
			PreviousMousePosition = MousePosition;
		}
	}

	Camera.Update();
	Renderer.SetCamera( Camera );
	World->SetActiveCamera( &Camera );

	World->Tick();
}

void CGameSample::Shutdown()
{
	CSimpleSound::StopAll();

	if( World )
	{
		/*CData Data;
		Data << *World;

		CFile File( "Levels/World.lw" );
		File.Load( Data );
		File.Save();*/

		World->Destroy();
		delete World;
		World = nullptr;
	}
}

Version CGameSample::GetVersion() const
{
	static Version Number;
	Number.Major = 0;
	Number.Minor = 0;
	Number.Hot = 0;

	return Number;
}

static void MenuItem( const char* Label, bool* Selected )
{
	if( ImGui::MenuItem( Label, "", Selected ) )
	{
		*Selected = true;
	}
}

void CGameSample::DrawDebugMenu( const bool Menu )
{
#if defined( IMGUI_ENABLED )
	static bool ShowDebugWindow = false;

	if( Menu )
	{
		MenuItem( "Sample Debug Menu", &ShowDebugWindow );
	}
	else
	{
		if( ShowDebugWindow )
		{
			if( ImGui::Begin( "Sample Debug Menu", &ShowDebugWindow, ImVec2( 500.0f, 700.0f ) ) )
			{
				ImGui::Text( "Sample Text" );
			}

			ImGui::End();
		}
	}
#endif
}

int main( int argc, char** argv )
{
	if( !GameLayersInstance )
	{
		Log::Event( Log::Fatal, "Game layers instance does not exist!\n" );
	}

	Application.SetName( "Sample Game" );
	Application.EnableDefaultExit( true );
	Application.ProcessCommandLine( argc, argv );

	GameLayersInstance->Add( GameInstance );

	Application.Run();

	return 0;
}
