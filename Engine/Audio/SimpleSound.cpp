// Copyright � 2017, Christiaan Bakker, All rights reserved.
#include "SimpleSound.h"

#include <ThirdParty/SFML-2.5.1/include/SFML/Audio.hpp>

#include <Engine/Profiling/Logging.h>
#include <Engine/Profiling/Profiling.h>
#include <Game/Game.h>

std::vector<FSound> CSimpleSound::Sounds;
std::vector<sf::SoundBuffer*> CSimpleSound::SoundBuffers;
std::vector<FStream> CSimpleSound::Streams;
float CSimpleSound::GlobalVolume = 1.0f;

SoundBufferHandle CSimpleSound::Sound( const std::string& ResourcePath )
{
	sf::SoundBuffer* NewSoundBuffer = new sf::SoundBuffer();
	if( NewSoundBuffer->loadFromFile( ResourcePath ) )
	{
		Log::Event( "Loaded sound \"%s\"\n", ResourcePath.c_str() );
		SoundBuffers.push_back( NewSoundBuffer );

		SoundBufferHandle Handle;
		Handle.Handle = SoundBuffers.size() - 1;
		return Handle;
	}

	Log::Event( Log::Warning, "Failed to load sound \"%s\"\n", ResourcePath.c_str() );

	return EmptyHandle<SoundBufferHandle>();
}

StreamHandle CSimpleSound::Music( const std::string& ResourcePath )
{
	sf::Music* NewMusic = new sf::Music();
	if( NewMusic->openFromFile( ResourcePath ) )
	{
		Log::Event( "Opening stream \"%s\"\n", ResourcePath.c_str() );
		FStream Stream;
		Stream.Stream = NewMusic;
		Streams.push_back( Stream );

		StreamHandle Handle;
		Handle.Handle = Streams.size() - 1;
		return Handle;
	}

	Log::Event( Log::Warning, "Failed to open stream \"%s\"\n", ResourcePath.c_str() );

	return EmptyHandle<StreamHandle>();
}

SoundHandle CSimpleSound::Start( SoundBufferHandle Handle )
{
	if( Handle.Handle > InvalidHandle && GlobalVolume != 0.0f )
	{
		FSound NewSound;
		NewSound.Sound = new sf::Sound( *SoundBuffers[Handle.Handle] );
		NewSound.Playing = true;
		Sounds.push_back( NewSound );
		NewSound.Sound->setVolume( NewSound.Volume * GlobalVolume );
		NewSound.Sound->play();

		SoundHandle NewHandle;
		NewHandle.Handle = Sounds.size() - 1;
		return NewHandle;
	}

	return EmptyHandle<SoundHandle>();
}

void CSimpleSound::Start( StreamHandle Handle, const float FadeIn )
{
	if( GlobalVolume == 0.0f )
		return;

	if( Handle.Handle == InvalidHandle )
		return;

	if( FadeIn > 0.0f )
	{
		Streams[Handle.Handle].Stream->setVolume( 0.0f );
	}
	else
	{
		Streams[Handle.Handle].Stream->setVolume( Streams[Handle.Handle].Volume * GlobalVolume );
	}

	Streams[Handle.Handle].Stream->play();
	Streams[Handle.Handle].FadeDuration = FadeIn;
	Streams[Handle.Handle].FadeIn = true;
	Streams[Handle.Handle].StartTime = static_cast<float>( GameLayersInstance->GetCurrentTime() );
	Streams[Handle.Handle].Playing = true;
}

void CSimpleSound::Stop( SoundHandle Handle )
{
	if( Handle.Handle > InvalidHandle)
	{
		Sounds[Handle.Handle].Playing = false;

		if( Sounds[Handle.Handle].Sound )
		{
			Sounds[Handle.Handle].Sound->stop();
			delete Sounds[Handle.Handle].Sound;
			Sounds[Handle.Handle].Sound = nullptr;
		}
	}
}

void CSimpleSound::Stop( StreamHandle Handle, const float FadeOut )
{
	if( Handle.Handle > InvalidHandle)
	{
		if( FadeOut < 0.0f )
		{
			Streams[Handle.Handle].Stream->stop();
			Streams[Handle.Handle].Playing = false;
		}
		else
		{
			Streams[Handle.Handle].FadeDuration = FadeOut;
			Streams[Handle.Handle].FadeIn = false;
			Streams[Handle.Handle].StartTime = static_cast<float>( GameLayersInstance->GetCurrentTime() );
		}
	}
}

void CSimpleSound::StopSounds()
{
	for( auto& Sound : Sounds )
	{
		Sound.Playing = false;

		if( Sound.Sound )
		{
			Sound.Sound->stop();
			delete Sound.Sound;
			Sound.Sound = nullptr;
		}
	}
}

void CSimpleSound::StopMusic()
{
	for( auto& Stream : Streams )
	{
		Stream.Stream->stop();
		Stream.Playing = false;
	}
}

void CSimpleSound::StopAll()
{
	StopSounds();
	StopMusic();
}

void CSimpleSound::Loop( SoundHandle Handle, const bool Loop )
{
	Sounds[Handle.Handle].Sound->setLoop(Loop);
}

void CSimpleSound::Loop( StreamHandle Handle, const bool Loop )
{
	Streams[Handle.Handle].Stream->setLoop( Loop );
}

void CSimpleSound::Rate( SoundHandle Handle, const float Rate )
{
	Sounds[Handle.Handle].Sound->setPitch( Rate );
}

void CSimpleSound::Rate( StreamHandle Handle, const float Rate )
{
	Streams[Handle.Handle].Stream->setPitch( Rate );
}

bool CSimpleSound::Playing( SoundHandle Handle )
{
	if( !Sounds[Handle.Handle].Sound )
		return false;

	bool Playing = Sounds[Handle.Handle].Sound->getStatus() == sf::SoundSource::Status::Playing;
	Sounds[Handle.Handle].Playing = Playing;

	if( !Playing )
	{
		delete Sounds[Handle.Handle].Sound;
		Sounds[Handle.Handle].Sound = nullptr;
	}

	return Playing;
}

bool CSimpleSound::Playing( StreamHandle Handle )
{
	bool Playing = Streams[Handle.Handle].Stream->getStatus() == sf::SoundSource::Status::Playing;
	Streams[Handle.Handle].Playing = Playing;
	return Playing;
}

void CSimpleSound::Volume( SoundHandle Handle, const float Volume )
{
	Sounds[Handle.Handle].Volume = Volume;
}

void CSimpleSound::Volume( StreamHandle Handle, const float Volume )
{
	Streams[Handle.Handle].Volume = Volume;
}

void CSimpleSound::Volume( const float GlobalVolumeIn )
{
	GlobalVolume = GlobalVolumeIn * 0.01f;
}

void CSimpleSound::Tick()
{
	Profile( "Sound" );

	CProfiler& Profiler = CProfiler::Get();
	Profiler.AddCounterEntry( FProfileTimeEntry( "Sound Buffers", Sounds.size() ), true );
	Profiler.AddCounterEntry( FProfileTimeEntry( "Stream Buffers", Streams.size() ), true );

	const float CurrentTime = static_cast<float>( GameLayersInstance->GetCurrentTime() );
	for( auto& Stream : Streams )
	{
		if( Stream.Stream && Stream.Playing )
		{
			Stream.Playing = Stream.Stream->getStatus() == sf::SoundSource::Status::Playing;

			if( Stream.FadeDuration > 0.0f )
			{
				if( Stream.FadeIn )
				{
					const float DeltaTime = CurrentTime - Stream.StartTime;
					float Alpha = DeltaTime / Stream.FadeDuration;
					Alpha *= Alpha * Alpha;
					if( Alpha < 1.0f && Alpha > 0.0f )
					{
						Stream.Stream->setVolume( Alpha * Stream.Volume * GlobalVolume );
					}
				}
				else
				{
					const float DeltaTime = CurrentTime - Stream.StartTime;
					float Alpha = DeltaTime / Stream.FadeDuration;
					// Alpha *= Alpha * Alpha;
					Alpha = 1.0f - Alpha;
					if( Alpha < 1.0f && Alpha > 0.0f )
					{
						Stream.Stream->setVolume( Alpha * Stream.Volume * GlobalVolume );
					}
					else if( Alpha < 0.0f )
					{
						Stream.Stream->stop();
						Stream.Playing = false;
					}
				}
			}
			else
			{
				Stream.Stream->setVolume( Stream.Volume * GlobalVolume );
			}
		}
	}

	size_t ActiveSounds = 0;

	for( auto& Sound : Sounds )
	{
		if( Sound.Sound && Sound.Playing )
		{
			ActiveSounds++;

			if( ActiveSounds > 200 )
			{
				Sound.Sound->stop();
				Sound.Playing = false;

				delete Sound.Sound;
				Sound.Sound = nullptr;
				continue;
			}

			Sound.Playing = Sound.Sound->getStatus() == sf::SoundSource::Status::Playing;
			Sound.Sound->setVolume( Sound.Volume * GlobalVolume );
		}
	}

	Profiler.AddCounterEntry( FProfileTimeEntry( "Active Sound Buffers", ActiveSounds ), true );
}

void CSimpleSound::Shutdown()
{
	StopAll();
	Sounds.clear();
	Streams.clear();
}
