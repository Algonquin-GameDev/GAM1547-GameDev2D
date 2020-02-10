#pragma once

#include "Audio.h"
#include "../Events/AudioEvent.h"
#include <xaudio2.h>


namespace GameDev2D
{
	struct WaveData
	{
		WaveData()
		{
			ZeroMemory(&waveFormat, sizeof(waveFormat));
			ZeroMemory(&buffer, sizeof(buffer));
			data = nullptr;
		}

		~WaveData()
		{
			delete[] data;
		}

		WAVEFORMATEX waveFormat;
		XAUDIO2_BUFFER buffer;
		void* data;
	};

	struct AudioPlaybackCallback : public IXAudio2VoiceCallback
	{
		AudioPlaybackCallback()
		{
		}

		virtual ~AudioPlaybackCallback()
		{
		}

		__declspec(nothrow) void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
		__declspec(nothrow) void __stdcall OnVoiceProcessingPassEnd() {}
		__declspec(nothrow) void __stdcall OnStreamEnd() {}
		__declspec(nothrow) void __stdcall OnBufferStart(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_PLAYBACK_STARTED));
			}
		}

		__declspec(nothrow) void __stdcall OnBufferEnd(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_PLAYBACK_ENDED));
				if (audio->GetCallback() != nullptr)
				{
					audio->GetCallback()->AudioIsDone(audio);
				}
			}
		}

		__declspec(nothrow) void __stdcall OnLoopEnd(void* context) 
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_LOOP_ENDED));
				if (audio->GetCallback() != nullptr)
				{
					audio->GetCallback()->AudioDidLoop(audio);
				}
			}
		}
		__declspec(nothrow) void __stdcall OnVoiceError(void* context, HRESULT) {}
	};
}