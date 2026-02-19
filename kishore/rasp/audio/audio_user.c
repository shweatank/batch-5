#include <alsa/asoundlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PCM_DEVICE "default"
#define SAMPLE_RATE 44100
#define CHANNELS 2
#define DURATION 3  // seconds

int main() {
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames = 32;
    int dir;
    int rc;
    int size;
    unsigned int sample_rate = SAMPLE_RATE;
    
    // Open PCM device for playback
    rc = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(rc));
        return 1;
    }
    
    // Allocate hardware parameters object
    snd_pcm_hw_params_alloca(&params);
    
    // Fill it with default values
    snd_pcm_hw_params_any(pcm_handle, params);
    
    // Set hardware parameters
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, CHANNELS);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &sample_rate, &dir);
    snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &frames, &dir);
    
    // Write parameters to device
    rc = snd_pcm_hw_params(pcm_handle, params);
    if (rc < 0) {
        fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(rc));
        return 1;
    }
    
    // Get period size
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * CHANNELS * 2; // 2 bytes per sample (S16_LE)
    
    // Allocate buffer
    short *buffer = (short *)malloc(size);
    
    printf("Playing 440 Hz sine wave for %d seconds...\n", DURATION);
    printf("Sample rate: %d Hz, Channels: %d, Period size: %ld frames\n", 
           sample_rate, CHANNELS, frames);
    
    // Generate and play sine wave (440 Hz)
    double frequency = 440.0;
    int total_frames = SAMPLE_RATE * DURATION;
    int frames_played = 0;
    
    while (frames_played < total_frames) {
        // Generate sine wave data
        for (int i = 0; i < frames; i++) {
            double t = (double)(frames_played + i) / SAMPLE_RATE;
            short sample = (short)(32767.0 * 0.5 * sin(2.0 * M_PI * frequency * t));
            
            // Write to both channels (stereo)
            buffer[i * CHANNELS] = sample;      // Left channel
            buffer[i * CHANNELS + 1] = sample;  // Right channel
        }
        
        // Write to PCM device
        rc = snd_pcm_writei(pcm_handle, buffer, frames);
        if (rc == -EPIPE) {
            // Underrun
            fprintf(stderr, "Underrun occurred\n");
            snd_pcm_prepare(pcm_handle);
        } else if (rc < 0) {
            fprintf(stderr, "Error writing to PCM device: %s\n", snd_strerror(rc));
            break;
        } else if (rc != (int)frames) {
            fprintf(stderr, "Short write (expected %ld, wrote %d)\n", frames, rc);
        }
        
        frames_played += frames;
    }
    
    // Drain and close
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buffer);
    
    printf("Playback complete!\n");
    
    return 0;
}
