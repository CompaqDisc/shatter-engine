// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "Precense.h"

#include <string>

#include <discord_rpc.h>
#include <Engine/Profiling/Logging.h>

namespace Presence
{
	bool Initialized = false;
	std::string State = "";
	std::string Details = "";
	std::string LargeImageKey = "";
	uint32_t Priority = 0;

	void Initialize( std::string ApplicationID )
	{
#if defined(DiscordPresence)
		DiscordEventHandlers Handlers;
		memset( &Handlers, 0, sizeof( Handlers ) );

		Discord_Initialize( ApplicationID.c_str(), &Handlers, 1, nullptr );

		Initialized = true;
#else
		Log::Event( Log::Warning, "Discord presence is disabled for debug builds.\n" );
#endif
	}

	void Tick()
	{
		Priority = 0;

#if defined(DiscordPresence)
		if( Initialized )
		{
			DiscordRichPresence Presence;
			memset( &Presence, 0, sizeof( Presence ) );
			Presence.state = State.c_str();
			Presence.details = Details.c_str();
			Presence.largeImageKey = LargeImageKey.c_str();
			Discord_UpdatePresence( &Presence );
		}
#endif
	}

	void Update( const char* NewState, const char* NewDetails, const char* NewLargeImageKey, uint32_t NewPriority )
	{
		if( Priority <= NewPriority )
		{
			State = NewState;
			Details = NewDetails;
			LargeImageKey = NewLargeImageKey;
			Priority = NewPriority;
		}
	}

	void Shutdown()
	{
#if defined(DiscordPresence)
		if( Initialized )
		{
#ifdef DISCORD_DISABLE_IO_THREAD
			Discord_UpdateConnection();
#endif
			Discord_RunCallbacks();
		}
#endif
	}
}
