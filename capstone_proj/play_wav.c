/*
 * play_wav.c - Minimal WAV player using ALSA (Raspberry Pi friendly)
 *
 * Supports:
 *   - PCM (uncompressed) WAV only
 *   - 8-bit unsigned PCM, 16-bit signed PCM, 24-bit packed PCM, 32-bit signed PCM
 *   - mono/stereo (and more channels if your device supports)
 *
 * Build:
 *   gcc -O2 -Wall -o play_wav play_wav.c -lasound
 *
 * Run:
 *   ./play_wav test.wav
 *   ./play_wav test.wav "hw:0,0"
 *
 * Notes:
 * - If your WAV is MP3-in-WAV or some compressed WAV codec, this will reject it.
 */

#define _POSIX_C_SOURCE 200809L//Enable POSIX.1-2008 features
#include <alsa/asoundlib.h>// ALSA audio library APIs for PCM playback/control
#include <errno.h>// Provides error codes and errno variable
#include <inttypes.h>// Fixed-width integer types (uint32_t, uint16_t, etc.)
#include <stdio.h>// Standard I/O functions (printf, fopen, fread, etc.)
#include <stdlib.h>// Memory allocation, exit, general utilities
#include <string.h>
#include<alloca.h>// Stack-based dynamic memory allocation (alloca)

#pragma pack(push, 1)//To avoid structure padding
typedef struct {
    char     riff_id[4];     // "RIFF" file identifier
    uint32_t riff_size;		//4-bytes file size
    char     wave_id[4];     // "WAVE" format identifier
} wav_riff_t;

typedef struct {
    char     id[4];          // "fmt "
    uint32_t size;           // 16 for PCM
    uint16_t audio_format;   // 1=PCM
    uint16_t num_channels;   //No of channels(1=mono 2=stereo
    uint32_t sample_rate;     //samples per second(e.g 44100 Hz)
    uint32_t byte_rate;      //bytes per second(sample rate*channels*bits/8)
    uint16_t block_align;    //bytes per audio frame(channels*bits/8)
    uint16_t bits_per_sample;//8,16,24,32
    // may have extra bytes if size > 16
} wav_fmt_t;
#pragma pack(pop)//restores prev structure alignment setting

static int read_exact(FILE *fp, void *buf, size_t n)// funcn to read 'n' bytes from file
{
    return (fread(buf, 1, n, fp) == n) ? 0 : -1;// return 0 if 'n' bytes read successfully
}

static int skip_bytes(FILE *fp, size_t n) // funcn to skip 'n' bytes in file 
{
    return (fseek(fp, (long)n, SEEK_CUR) == 0) ? 0 : -1;// move file ptr forward by 'n' bytes
}


//funcn to convert the WAV bits/sample to corresponding ALSA PCM format
static snd_pcm_format_t bits_to_alsa_format(uint16_t bits)
{
    // For PCM WAV:
    // 8-bit is unsigned, 16/24/32 are signed little-endian
    switch (bits)//cheeck bit depth from wave header
    {
        case 8:  return SND_PCM_FORMAT_U8;
        case 16: return SND_PCM_FORMAT_S16_LE;
        case 24: return SND_PCM_FORMAT_S24_3LE; // packed 24-bit (3 bytes)
        case 32: return SND_PCM_FORMAT_S32_LE;
        default: return SND_PCM_FORMAT_UNKNOWN;//unsupported bit format
    }
}

//print ALSA error message and terminate program
static void die_alsa(const char *msg, int err)
{
    fprintf(stderr, "%s: %s\n", msg, snd_strerror(err));//custom messagae + readable alsa error string
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.wav> [alsa_device]\n", argv[0]);
        fprintf(stderr, "Example: %s test.wav\n", argv[0]);
        fprintf(stderr, "Example: %s test.wav hw:0,0\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];	//store WAV file path from cmd line arg
    const char *dev  = (argc >= 3) ? argv[2] : "default";//Use given ALSA device or fallback to "default"

    FILE *fp = fopen(path, "rb"); //wave file in binary read mode
    if (!fp) {
        perror("fopen");
        return 1;
    }

    // ---- Parse RIFF header ----
    wav_riff_t riff;// declare the dtructure to store first 12bytes(RIFF header)
    if (read_exact(fp, &riff, sizeof(riff)) != 0)// read RIFF header from file
    {
     fprintf(stderr, "Failed to read RIFF header\n");
        fclose(fp);
        return 1;
    }
//check 1st 4bytes are RIFF                     check fromat identifier is WAVE
    if (memcmp(riff.riff_id, "RIFF", 4) != 0 || memcmp(riff.wave_id, "WAVE", 4) != 0)
    {
        fprintf(stderr, "Not a RIFF/WAVE file\n");
        fclose(fp);
        return 1;
    }
  // ---- Find "fmt " and "data" chunks ----
    wav_fmt_t fmt = {0};
    int have_fmt = 0;
    uint32_t data_size = 0;
    long data_offset = -1;

    while (!have_fmt || data_offset < 0)//loop until both "fmt" chunk and "data" chunk are found
 {
        char chunk_id[4];//buf to store current chunk id
        uint32_t chunk_size;// var to store current chunk size

        if (read_exact(fp, chunk_id, 4) != 0)//read chunk id
	break;
        if (read_exact(fp, &chunk_size, 4) != 0)//read chunk size
	break;

        if (memcmp(chunk_id, "fmt ", 4) == 0)//check if the chunk is "fmt" 
	{
            // Read at least the base fmt struct without the "id" and "size"
            // We've already read id + size, so read the rest fields:
            if (chunk_size < 16)//validate min fmt chunk size for PCM
 {
                fprintf(stderr, "Invalid fmt chunk size: %u\n", chunk_size);
                fclose(fp);
                return 1;
            }

            fmt.size = chunk_size;//store fmt chunk from the header
            if (read_exact(fp, &fmt.audio_format, 16) != 0)
	 {
                fprintf(stderr, "Failed to read fmt chunk\n");
                fclose(fp);
                return 1;
            }

            // Skip extra fmt bytes if present
            if (chunk_size > 16) {
                if (skip_bytes(fp, chunk_size - 16) != 0) {
                    fprintf(stderr, "Failed to skip extra fmt bytes\n");
                    fclose(fp);
                    return 1;
                }
            }

            have_fmt = 1;//fmt found
        } else if (memcmp(chunk_id, "data", 4) == 0)//check if current chunk is data
 {
            data_size = chunk_size;//store size of audio data
            data_offset = ftell(fp);//record file pos when audio data begins
            // Do not skip now; we want to stream from here
            break;
        } else {
            // Skip unknown chunk
            if (skip_bytes(fp, chunk_size) != 0) break;
        }

        // Chunks are word-aligned in RIFF; if odd size, skip pad byte
        if (chunk_size & 1) (void)skip_bytes(fp, 1);//skip one padding byte if chunk size is odd
    }

    if (!have_fmt || data_offset < 0) {
        fprintf(stderr, "Could not find required fmt/data chunks\n");
        fclose(fp);
        return 1;
    }

    if (fmt.audio_format != 1) {
        fprintf(stderr, "Unsupported WAV codec (audio_format=%u). Need PCM (1).\n", fmt.audio_format);
        fclose(fp);
        return 1;
    }

    snd_pcm_format_t pcm_format = bits_to_alsa_format(fmt.bits_per_sample);
    if (pcm_format == SND_PCM_FORMAT_UNKNOWN) {
        fprintf(stderr, "Unsupported bits/sample: %u\n", fmt.bits_per_sample);
        fclose(fp);
        return 1;
    }

    // Seek to data start
    if (fseek(fp, data_offset, SEEK_SET) != 0) {
        perror("fseek data");
        fclose(fp);
        return 1;
    }

    fprintf(stderr, "WAV: %u Hz, %u ch, %u bits, data=%u bytes, ALSA dev=%s\n",
            fmt.sample_rate, fmt.num_channels, fmt.bits_per_sample, data_size, dev);

    // ---- ALSA setup ----
    snd_pcm_t *pcm = NULL;//ptr to alsa pcm blayback
    snd_pcm_hw_params_t *hw = NULL;

    int err = snd_pcm_open(&pcm, dev, SND_PCM_STREAM_PLAYBACK, 0);//open alsa device for playback
    if (err < 0) die_alsa("snd_pcm_open", err);

    snd_pcm_hw_params_alloca(&hw);
    err = snd_pcm_hw_params_any(pcm, hw);
    if (err < 0) die_alsa("hw_params_any", err);

    err = snd_pcm_hw_params_set_access(pcm, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) die_alsa("set_access", err);

    err = snd_pcm_hw_params_set_format(pcm, hw, pcm_format);//u8,s16,s24/s32
    if (err < 0) die_alsa("set_format", err);

    err = snd_pcm_hw_params_set_channels(pcm, hw, fmt.num_channels);//set no of channels
    if (err < 0) die_alsa("set_channels", err);

    unsigned int rate = fmt.sample_rate;//copy wav sample rate
    err = snd_pcm_hw_params_set_rate_near(pcm, hw, &rate, NULL);//set sample rate
    if (err < 0) die_alsa("set_rate_near", err);

    // Apply
    err = snd_pcm_hw_params(pcm, hw);
    if (err < 0) die_alsa("snd_pcm_hw_params", err);

    err = snd_pcm_prepare(pcm);
    if (err < 0) die_alsa("snd_pcm_prepare", err);

    // ---- Stream data ----
    const unsigned bytes_per_sample = (fmt.bits_per_sample == 24) ? 3 : (fmt.bits_per_sample / 8);
    const unsigned bytes_per_frame  = bytes_per_sample * fmt.num_channels;

    // chunk frames
    const snd_pcm_uframes_t chunk_frames = 1024;
    const size_t chunk_bytes = (size_t)chunk_frames * bytes_per_frame;

    uint8_t *buf = (uint8_t *)malloc(chunk_bytes);//total bytes per cunk
    if (!buf) {
        perror("malloc");
        snd_pcm_close(pcm);
        fclose(fp);
        return 1;
    }

    uint32_t remaining = data_size;//audio bytes left to play
    while (remaining > 0)//loop untill all audio data is sent
 {
        size_t to_read = chunk_bytes;// default 1 chunk
        if (to_read > remaining) to_read = remaining;

        size_t n = fread(buf, 1, to_read, fp); // Read audio data from WAV file into buffer
        if (n == 0) break; // Stop if no more data

        remaining -= (uint32_t)n;

        snd_pcm_uframes_t frames = (snd_pcm_uframes_t)(n / bytes_per_frame);
        uint8_t *p = buf;

        while (frames > 0)//write all frames to alsa
 {
            snd_pcm_sframes_t w = snd_pcm_writei(pcm, p, frames);
            if (w == -EPIPE) {
                snd_pcm_prepare(pcm); // underrun recovery
                continue;
            } else if (w < 0) {
                w = snd_pcm_recover(pcm, (int)w, 1);//automatic recovery
                if (w < 0) die_alsa("snd_pcm_writei/recover", (int)w);
                continue;
            }

            snd_pcm_uframes_t written = (snd_pcm_uframes_t)w;
            frames -= written;
            p += (size_t)written * bytes_per_frame;//mov buf ptr frwd by written frames
        }
    }

    snd_pcm_drain(pcm);//Wait until all queued audio samples are played

    free(buf);
    snd_pcm_close(pcm);
    fclose(fp);

    fprintf(stderr, "Done.\n");
    return 0;
}

