// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <Engine/Utility/Service/ServiceRegistry.h>

class CApplication
{
public:
	CApplication();
	~CApplication();

	void Initialize();
	void Run();

	void InitializeDefaultInputs();

	std::string GetName() const;
	void SetName( const char* Name );

	const bool ToolsEnabled() const;
	void EnableTools( const bool Enable );

	CServiceRegistry ServiceRegistry;

private:
	std::string Name;
	bool Tools;
};
