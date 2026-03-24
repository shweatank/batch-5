#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"

int main() {
    snd_pcm_t *capture_handle, *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    int err;

    unsigned int rate = 44100;
    int channels = 2;
    snd_pcm_uframes_t frames = 1024;

    int duration = 5; // seconds
    int total_frames = rate * duration;

    int16_t *buffer;
    buffer = malloc(frames * channels * sizeof(int16_t));

    printf("Opening capture device...\n");

    // Open capture device
    if ((err = snd_pcm_open(&capture_handle, PCM_DEVICE,
                            SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "Cannot open capture device: %s\n", snd_strerror(err));
        return 1;
    }

    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(capture_handle, hw_params);
    snd_pcm_hw_params_set_access(capture_handle, hw_params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capture_handle, hw_params,
                                 SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(capture_handle, hw_params, channels);
    snd_pcm_hw_params(capture_handle, hw_params);

    snd_pcm_prepare(capture_handle);

    printf("Recording for %d seconds...\n", duration);

    FILE *fp = fopen("recorded.raw", "wb");

    int frames_to_capture = total_frames / frames;

    for (int i = 0; i < frames_to_capture; i++) {
        err = snd_pcm_readi(capture_handle, buffer, frames);
        if (err < 0)
            err = snd_pcm_recover(capture_handle, err, 0);
        if (err < 0) {
            fprintf(stderr, "Read error: %s\n", snd_strerror(err));
            break;
        }
        fwrite(buffer, sizeof(int16_t), frames * channels, fp);
    }

    fclose(fp);
    snd_pcm_close(capture_handle);

    printf("Recording complete.\n");

    // ================= PLAYBACK =================

    printf("Opening playback device...\n");

    if ((err = snd_pcm_open(&playback_handle, PCM_DEVICE,
                            SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "Cannot open playback device: %s\n", snd_strerror(err));
        return 1;
    }

    snd_pcm_hw_params_any(playback_handle, hw_params);
    snd_pcm_hw_params_set_access(playback_handle, hw_params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback_handle, hw_params,
                                 SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(playback_handle, hw_params, channels);
    snd_pcm_hw_params(playback_handle, hw_params);

    snd_pcm_prepare(playback_handle);

    printf("Playing recorded audio...\n");

    fp = fopen("recorded.raw", "rb");

    while ((err = fread(buffer, sizeof(int16_t),
                        frames * channels, fp)) > 0) {

        int frames_read = err / channels;

        err = snd_pcm_writei(playback_handle, buffer, frames_read);
        if (err < 0)
            err = snd_pcm_recover(playback_handle, err, 0);
        if (err < 0) {
            fprintf(stderr, "Write error: %s\n", snd_strerror(err));
            break;
        }
    }

    fclose(fp);
    snd_pcm_drain(playback_handle);
    snd_pcm_close(playback_handle);

    free(buffer);

    printf("Playback complete.\n");

    return 0;
}
