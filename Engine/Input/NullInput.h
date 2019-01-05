// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <Engine/Input/InputInterface.h>

class CNullInput : public IInput
{
public:
	virtual void RegisterKeyInput( int KeyInput, int ScanCode, int Action, int Modifiers ) override;
	virtual void RegisterMouseButtonInput( int MouseButton, int Action, int Modifiers ) override;
	virtual void RegisterMousePositionInput( double PositionX, double PositionY ) override;
	virtual void RegisterScrollInput( int OffsetX, int OffsetY ) override;
	virtual void RegisterJoystickStatus( int Joystick, int Event ) override;

	virtual void AddActionBinding( FActionBinding ActionBinding ) override;
	virtual void AddActionBinding( EActionBindingType BindingType, int KeyInput, int Action, ActionTarget TargetFunc ) override;
	virtual void ClearActionBindings() override;

	virtual void Tick() override;

	virtual bool IsKeyDown( int KeyInput ) const override;
	virtual bool IsAnyKeyDown() const override;
	virtual FFixedPosition2D GetMousePosition() const override;

};