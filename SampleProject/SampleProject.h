// Copyright © 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <memory>
#include <vector>

#include <Game/Game.h>

#include <Engine/Display/Rendering/Camera.h>

class CWorld;
class CEntity;

class CGameSample : public IGameLayer
{
public:
	CGameSample();
	virtual ~CGameSample();

	virtual void Initialize() override;
	virtual void Frame() override;
	virtual void Tick() override;
	virtual void Shutdown() override;

	virtual Version GetVersion() const override;

	void DrawDebugMenu( const bool Menu );

private:
	CWorld* World;
	CCamera Camera;
};