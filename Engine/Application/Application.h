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

	std::string GetName() const;
	void SetName( const char* Name );

	CServiceRegistry ServiceRegistry;

private:
	std::string Name;
};
