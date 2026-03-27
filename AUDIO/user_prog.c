#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_audio_file>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    char output[] = "/tmp/converted.wav";
    char cmd[512];

    // Find last dot in filename
    const char *ext = strrchr(input, '.');
    int is_wav = (ext && strcmp(ext, ".wav") == 0);

    if (!ext) {
        // No extension → check with ffprobe
        snprintf(cmd, sizeof(cmd),
                 "ffprobe -v error -show_entries stream=codec_name "
                 "-of default=noprint_wrappers=1:nokey=1 \"%s\" 2>/dev/null",
                 input);

        FILE *fp = popen(cmd, "r");
        if (!fp) {
            fprintf(stderr, "Failed to run ffprobe\n");
            return 1;
        }

        char codec[64];
        if (fgets(codec, sizeof(codec), fp) == NULL) {
            fprintf(stderr, "File is not a valid audio file\n");
            pclose(fp);
            return 1;
        }
        pclose(fp);

        // Trim newline
        codec[strcspn(codec, "\n")] = 0;

        if (strcmp(codec, "pcm_s16le") == 0) {
            // Already WAV PCM → play directly
            printf("File is already WAV PCM, playing directly...\n");
        } else {
            // Convert to WAV
            printf("File is audio (%s), converting to WAV...\n", codec);
            snprintf(cmd, sizeof(cmd),
                     "ffmpeg -y -i \"%s\" -ar 44100 -ac 2 -f wav \"%s\"",
                     input, output);

            if (system(cmd) != 0) {
                fprintf(stderr, "ffmpeg conversion failed\n");
                return 1;
            }
            input = output;
        }
    }
    else if (!is_wav) {
        // Extension exists but not WAV → convert
        printf("Converting %s -> %s\n", input, output);
        snprintf(cmd, sizeof(cmd),
                 "ffmpeg -y -i \"%s\" -ar 44100 -ac 2 -f wav \"%s\"",
                 input, output);

        if (system(cmd) != 0) {
            fprintf(stderr, "ffmpeg conversion failed\n");
            return 1;
        }
        input = output;
    }

    // Play WAV file using ALSA aplay
    snprintf(cmd, sizeof(cmd),
             "aplay -D hw:1,0 \"%s\"", input);

    printf("Playing %s via ALSA driver\n", input);
    if (system(cmd) != 0) {
        fprintf(stderr, "aplay failed\n");
        return 1;
    }

    return 0;
}

