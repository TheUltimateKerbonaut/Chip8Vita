#include "Audio.h"

#include <cstddef>

// Mostly lifted from https://github.com/vitasdk/samples/blob/master/audio/src/main.c

Audio::Audio()
{
    port = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, size, frequency, SCE_AUDIO_OUT_MODE_MONO);
    sceAudioOutSetVolume(port, (SceAudioOutChannelFlag)(SCE_AUDIO_VOLUME_FLAG_L_CH |SCE_AUDIO_VOLUME_FLAG_R_CH), maxVolumeArray);
    gen = squareWave;
    wave_set(waveBuffer, size, gen);
}

void Audio::wave_set(int16_t*buffer, size_t size,  wav_gen generator)
{
	for (size_t smpl = 0; smpl < size; ++smpl)
		buffer[smpl] = 0x7FFF*generator((float)smpl/(float)size);
}

double Audio::squareWave(double p)
{
    return p > 0.5 ? -1.0 : 1.0;
}

double Audio::nullWave(double p)
{
    return 0;
}


void Audio::update(Chip8& chip8)
{
    //if (chip8.timer.soundTimer > 0) sceAudioOutSetVolume(port, (SceAudioOutChannelFlag)(SCE_AUDIO_VOLUME_FLAG_L_CH |SCE_AUDIO_VOLUME_FLAG_R_CH), maxVolumeArray);
    //else sceAudioOutSetVolume(port, (SceAudioOutChannelFlag)(SCE_AUDIO_VOLUME_FLAG_L_CH |SCE_AUDIO_VOLUME_FLAG_R_CH), minVolumeArray);

    if (chip8.timer.soundTimer > 0) gen = squareWave;
    else gen = nullWave;
    wave_set(waveBuffer, size, gen);
    
    sceAudioOutOutput(port, waveBuffer);
}

Audio::~Audio()
{
    sceAudioOutReleasePort(port);
}