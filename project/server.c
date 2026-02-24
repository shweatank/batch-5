#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <alsa/asoundlib.h>

#define PORT 9000
#define BUFFER_SIZE 4

/* Play WAV file using ALSA */
void play_sound(const char *filename)
{
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    FILE *file;
    char buffer[4096];
    size_t read_size;
    unsigned int rate = 44100;
    int channels = 2;
    int dir;

    file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return;
    }

    /* Skip WAV header (44 bytes) */
    fseek(file, 44, SEEK_SET);

    if (snd_pcm_open(&pcm_handle, "default",
                     SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        printf("Error opening PCM device\n");
        fclose(file);
        return;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);

    snd_pcm_hw_params_set_access(pcm_handle,
        params,
        SND_PCM_ACCESS_RW_INTERLEAVED);

    snd_pcm_hw_params_set_format(pcm_handle,
        params,
        SND_PCM_FORMAT_S16_LE);

    snd_pcm_hw_params_set_channels(pcm_handle,
        params,
        channels);

    snd_pcm_hw_params_set_rate_near(pcm_handle,
        params,
        &rate,
        &dir);

    snd_pcm_hw_params(pcm_handle, params);

    while ((read_size = fread(buffer, 1,
            sizeof(buffer), file)) > 0) {

        snd_pcm_writei(pcm_handle,
            buffer,
            read_size / 4);  // 16bit stereo = 4 bytes/frame
    }

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    fclose(file);
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    int alert_value;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd,
            (struct sockaddr *)&address,
            sizeof(address)) < 0) {
        perror("bind");
        return -1;
    }

    listen(server_fd, 1);

    printf("Waiting for Raspberry Pi alert...\n");

    client_fd = accept(server_fd,
                      (struct sockaddr *)&address,
                      &addr_len);

    printf("Connected to Raspberry Pi!\n");

    while (1) {
        int ret = read(client_fd,
                       &alert_value,
                       sizeof(alert_value));

        if (ret <= 0) {
            printf("Connection closed\n");
            break;
        }

        printf("ALERT RECEIVED: %d\n", alert_value);

        play_sound("alert.wav");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

