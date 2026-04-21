#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define WIDTH 320
#define HEIGHT 240
#define FRAME_SIZE (WIDTH * HEIGHT * 2)  // RGB565 = 2 bytes per pixel
#define BUFFER_COUNT 4

struct buffer {
    void *start;
    size_t length;
};

int main() {
    int fd_v4l, fd_lcd;
    struct buffer buffers[BUFFER_COUNT];

    // Open V4L2 device
    fd_v4l = open("/dev/video0", O_RDWR);
    if(fd_v4l < 0) {
        perror("Open /dev/video0");
        return 1;
    }

    // Set format
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(ioctl(fd_v4l, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT");
        close(fd_v4l);
        return 1;
    }

    // Request mmap buffers
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd_v4l, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS");
        close(fd_v4l);
        return 1;
    }

    // Map buffers
    for(int i=0; i<BUFFER_COUNT; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if(ioctl(fd_v4l, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF");
            close(fd_v4l);
            return 1;
        }

        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd_v4l, buf.m.offset);
        if(buffers[i].start == MAP_FAILED) {
            perror("mmap");
            close(fd_v4l);
            return 1;
        }

        // Queue buffer
        if(ioctl(fd_v4l, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF");
            close(fd_v4l);
            return 1;
        }
    }

    // Start streaming
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd_v4l, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        close(fd_v4l);
        return 1;
    }

    // Open ILI9225 device
    fd_lcd = open("/dev/ili9225_char", O_WRONLY);
    if(fd_lcd < 0) {
        perror("Open /dev/ili9225_char");
        close(fd_v4l);
        return 1;
    }

    printf("Streaming video to ILI9225...\n");

    // Main capture loop
    while(1) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        // Dequeue buffer
        if(ioctl(fd_v4l, VIDIOC_DQBUF, &buf) < 0) {
            perror("VIDIOC_DQBUF");
            break;
        }

        // Write frame to LCD
        if(write(fd_lcd, buffers[buf.index].start, FRAME_SIZE) != FRAME_SIZE) {
            perror("LCD write failed");
            break;
        }

        // Requeue buffer
        if(ioctl(fd_v4l, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF");
            break;
        }
    }

    // Stop streaming
    ioctl(fd_v4l, VIDIOC_STREAMOFF, &type);
    close(fd_lcd);
    close(fd_v4l);

    return 0;
}
