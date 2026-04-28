#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#define WIDTH 640
#define HEIGHT 480
#define BUFFER_COUNT 4

struct buffer {
    void *start;
    size_t length;
};

struct buffer buffers[BUFFER_COUNT];

int main()
{
    int fd;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;
    int i;

    fd = open("/dev/video0", O_RDWR);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    /* Query device capabilities */

    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        perror("VIDIOC_QUERYCAP");
        return -1;
    }

    printf("Driver: %s\n", cap.driver);
    printf("Card: %s\n", cap.card);

    /* Set video format */

    memset(&fmt, 0, sizeof(fmt));

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT");
        return -1;
    }

    /* Request buffers */

    memset(&req, 0, sizeof(req));

    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS");
        return -1;
    }

    /* Map buffers */

    for (i = 0; i < BUFFER_COUNT; i++) {

        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF");
            return -1;
        }

        buffers[i].length = buf.length;

        buffers[i].start = mmap(NULL,
                                buf.length,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED,
                                fd,
                                buf.m.offset);

        if (buffers[i].start == MAP_FAILED) {
            perror("mmap");
            return -1;
        }
    }

    /* Queue buffers */

    for (i = 0; i < BUFFER_COUNT; i++) {

        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF");
            return -1;
        }
    }

    /* Start streaming */

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        return -1;
    }

    FILE *file = fopen("frame.raw", "wb");

    /* Capture 100 frames */

    for (i = 0; i < 100; i++) {

        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
            perror("VIDIOC_DQBUF");
            return -1;
        }

        fwrite(buffers[buf.index].start,
               buf.bytesused,
               1,
               file);

        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF");
            return -1;
        }

        printf("Captured frame %d\n", i);
    }

    fclose(file);

    /* Stop streaming */

    ioctl(fd, VIDIOC_STREAMOFF, &type);

    /* Unmap buffers */

    for (i = 0; i < BUFFER_COUNT; i++)
        munmap(buffers[i].start, buffers[i].length);

    close(fd);

    return 0;
}
