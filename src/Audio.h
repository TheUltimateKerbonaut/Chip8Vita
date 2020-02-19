#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <cstddef>

#include <psp2/audioout.h>

#include "Chip8.h"

// Mostly lifted from https://github.com/vitasdk/samples/blob/master/audio/src/main.c

typedef double (*wav_gen)(double);

class Audio
{
    public:

        Audio();
        ~Audio();

        void update(Chip8&);

    private:

        const int size = 256;
        const int frequency = 44100;

        void wave_set(int16_t*buffer, size_t size,  wav_gen generator);
        static double squareWave(double p);
        static double nullWave(double p);

        int maxVolumeArray[2] = {SCE_AUDIO_VOLUME_0DB, SCE_AUDIO_VOLUME_0DB};
        int minVolumeArray[2] = {0, 0};
        int16_t waveBuffer[SCE_AUDIO_MAX_LEN] = {0};
        wav_gen gen = 0;

        int port;

};

#endif