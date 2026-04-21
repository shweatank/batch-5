#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#define PCM_PLAYBACK_DEVICE "plughw:0,0"  // wired headphones

int main() {
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    int err;

    unsigned int rate = 44100;
    int channels = 2;
    snd_pcm_uframes_t frames = 1024;

    int16_t *buffer = malloc(frames * channels * sizeof(int16_t));
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Opening playback device...\n");
    if ((err = snd_pcm_open(&playback_handle, PCM_PLAYBACK_DEVICE,
                            SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "Cannot open playback device: %s\n", snd_strerror(err));
        return 1;
    }

    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(playback_handle, hw_params);
    snd_pcm_hw_params_set_access(playback_handle, hw_params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback_handle, hw_params,
                                 SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(playback_handle, hw_params, channels);
    snd_pcm_hw_params(playback_handle, hw_params);

    snd_pcm_prepare(playback_handle);

    // Example: fill buffer with silence (or load audio data)
    for (int i = 0; i < frames * channels; i++)
        buffer[i] = 0;

    printf("Playing audio...\n");
    for (int i = 0; i < 100; i++)  // just repeat for demo
        snd_pcm_writei(playback_handle, buffer, frames);

    snd_pcm_drain(playback_handle);
    snd_pcm_close(playback_handle);
    free(buffer);

    printf("Playback complete.\n");
    return 0;
}
