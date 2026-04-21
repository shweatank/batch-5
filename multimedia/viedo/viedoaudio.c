#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#define DEVICE "/dev/ili9225_char"
#define FRAME_SIZE (176*220*2)

// 🎵 AUDIO THREAD
void *play_audio(void *arg)
{
    const char *filename = (const char *)arg;

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    FILE *fp;
    char *buffer;
    int rc;
    int size = 4096;

    snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);

    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);

    snd_pcm_hw_params(handle, params);

    fp = fopen(filename, "rb");
    if (!fp) {
        perror("Audio file open failed");
        return NULL;
    }

    fseek(fp, 44, SEEK_SET); // skip WAV header

    buffer = malloc(size);

    while ((rc = fread(buffer, 1, size, fp)) > 0)
    {
        snd_pcm_writei(handle, buffer, rc / 4);
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);

    fclose(fp);
    free(buffer);

    return NULL;
}

// 🖼 LCD DISPLAY
void display_image(const char *imgfile)
{
    int fd;
    FILE *img;
    unsigned char *frame;

    fd = open(DEVICE, O_WRONLY);
    if (fd < 0)
    {
        perror("LCD open failed");
        return;
    }

    frame = malloc(FRAME_SIZE);

    img = fopen(imgfile, "rb");
    if (!img)
    {
        perror("Image open failed");
        close(fd);
        return;
    }

    fread(frame, 1, FRAME_SIZE, img);

    write(fd, frame, FRAME_SIZE);

    close(fd);
    fclose(img);
    free(frame);
}

int main()
{
    pthread_t audio_thread;

    // 🔊 Start audio in parallel
    pthread_create(&audio_thread, NULL, play_audio, "sound.wav");

    // 🖼 Show image
    display_image("image.raw");

    // Wait for audio to finish
    pthread_join(audio_thread, NULL);

    return 0;
}
