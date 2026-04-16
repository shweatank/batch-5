/*
 * ov7251_capture.c
 *
 * User-space V4L2 frame capture application for OV7251 MIPI camera.
 * Captures N frames using mmap-based DMA buffers and saves each
 * frame as a raw binary file (Y8 grayscale) and optionally as PGM
 * (Portable GrayMap) for easy viewing.
 *
 * Build:
 *   gcc -Wall -O2 -o ov7251_capture ov7251_capture.c
 *
 * Usage:
 *   ./ov7251_capture [device] [num_frames] [output_dir]
 *   ./ov7251_capture /dev/video0 10 ./frames
 *
 * Author: Linux Kernel Driver Developer
 * Target: Embedded Linux (ARM), OV7251 MIPI CSI-2 Camera
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/videodev2.h>

/* ─────────────────────────────────────────
 * Configuration
 * ───────────────────────────────────────── */
#define DEFAULT_DEVICE      "/dev/video0"
#define DEFAULT_NUM_FRAMES  10
#define DEFAULT_OUTPUT_DIR  "./frames"

#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480
#define PIXEL_FORMAT        V4L2_PIX_FMT_GREY   /* OV7251 Y8 monochrome */
#define NUM_BUFFERS         4                    /* DMA buffer pool size */
#define SELECT_TIMEOUT_SEC  2                    /* wait up to 2s per frame */

/* ─────────────────────────────────────────
 * DMA buffer descriptor
 * ───────────────────────────────────────── */
typedef struct {
    void   *start;   /* mmap'd virtual address */
    size_t  length;  /* buffer length in bytes  */
} DmaBuffer;

/* ─────────────────────────────────────────
 * Application context
 * ───────────────────────────────────────── */
typedef struct {
    int         fd;                    /* /dev/videoX file descriptor */
    DmaBuffer   buffers[NUM_BUFFERS];  /* mmap'd DMA buffer pool      */
    uint32_t    n_buffers;             /* actual buffers allocated    */
    uint32_t    width;
    uint32_t    height;
    uint32_t    frame_size;            /* bytes per frame             */
    char        output_dir[256];
} CaptureCtx;

/* ─────────────────────────────────────────
 * Error helpers
 * ───────────────────────────────────────── */
#define CHECK(expr, msg) \
    do { if ((expr) == -1) { perror(msg); goto err; } } while (0)

#define IOCTL(fd, req, arg, msg) \
    do { if (ioctl((fd), (req), (arg)) == -1) { perror(msg); goto err; } } while (0)

/* ─────────────────────────────────────────
 * Step 1: Open the V4L2 device
 * ───────────────────────────────────────── */
static int open_device(const char *dev_path)
{
    struct stat st;

    if (stat(dev_path, &st) == -1) {
        fprintf(stderr, "[ERROR] Cannot identify device '%s': %s\n",
                dev_path, strerror(errno));
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        fprintf(stderr, "[ERROR] '%s' is not a character device\n", dev_path);
        return -1;
    }

    int fd = open(dev_path, O_RDWR | O_NONBLOCK, 0);
    if (fd == -1) {
        fprintf(stderr, "[ERROR] Cannot open '%s': %s\n",
                dev_path, strerror(errno));
        return -1;
    }

    printf("[OK]  Opened device: %s (fd=%d)\n", dev_path, fd);
    return fd;
}

/* ─────────────────────────────────────────
 * Step 2: Query capabilities and verify
 *         device supports streaming
 * ───────────────────────────────────────── */
static int query_capabilities(int fd)
{
    struct v4l2_capability cap;
    IOCTL(fd, VIDIOC_QUERYCAP, &cap, "VIDIOC_QUERYCAP");

    printf("[OK]  Driver   : %s\n", cap.driver);
    printf("[OK]  Card     : %s\n", cap.card);
    printf("[OK]  Bus info : %s\n", cap.bus_info);
    printf("[OK]  Version  : %u.%u.%u\n",
           (cap.version >> 16) & 0xFF,
           (cap.version >>  8) & 0xFF,
           (cap.version      ) & 0xFF);

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "[ERROR] Device does not support video capture\n");
        goto err;
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "[ERROR] Device does not support streaming I/O\n");
        goto err;
    }

    printf("[OK]  Streaming and capture capabilities confirmed\n");
    return 0;
err:
    return -1;
}

/* ─────────────────────────────────────────
 * Step 3: Set pixel format and resolution
 * ───────────────────────────────────────── */
static int set_format(CaptureCtx *ctx)
{
    /* First, try to set the desired format */
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = FRAME_WIDTH;
    fmt.fmt.pix.height      = FRAME_HEIGHT;
    fmt.fmt.pix.pixelformat = PIXEL_FORMAT;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;  /* progressive scan */

    IOCTL(ctx->fd, VIDIOC_S_FMT, &fmt, "VIDIOC_S_FMT");

    /* Driver may adjust values — read back and confirm */
    ctx->width      = fmt.fmt.pix.width;
    ctx->height     = fmt.fmt.pix.height;
    ctx->frame_size = fmt.fmt.pix.sizeimage;

    /* Verify format was accepted */
    if (fmt.fmt.pix.pixelformat != PIXEL_FORMAT) {
        fprintf(stderr, "[WARN] Pixel format changed by driver to 0x%08X\n",
                fmt.fmt.pix.pixelformat);
    }

    printf("[OK]  Format   : %ux%u, %.4s, %u bytes/frame\n",
           ctx->width, ctx->height,
           (char *)&fmt.fmt.pix.pixelformat,
           ctx->frame_size);

    return 0;
err:
    return -1;
}

/* ─────────────────────────────────────────
 * Step 4: Request and mmap DMA buffers
 * ───────────────────────────────────────── */
static int init_buffers(CaptureCtx *ctx)
{
    /* Request kernel to allocate DMA buffers */
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count  = NUM_BUFFERS;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    IOCTL(ctx->fd, VIDIOC_REQBUFS, &req, "VIDIOC_REQBUFS");

    if (req.count < 2) {
        fprintf(stderr, "[ERROR] Insufficient DMA buffers: got %u, need 2\n",
                req.count);
        goto err;
    }

    ctx->n_buffers = req.count;
    printf("[OK]  Allocated %u DMA buffers\n", ctx->n_buffers);

    /* mmap each buffer into user-space virtual address space */
    for (uint32_t i = 0; i < ctx->n_buffers; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;

        IOCTL(ctx->fd, VIDIOC_QUERYBUF, &buf, "VIDIOC_QUERYBUF");

        ctx->buffers[i].length = buf.length;
        ctx->buffers[i].start  = mmap(
            NULL,                   /* kernel chooses virtual address */
            buf.length,
            PROT_READ | PROT_WRITE, /* read pixel data, write optional */
            MAP_SHARED,             /* share physical pages with kernel */
            ctx->fd,
            buf.m.offset            /* buffer's physical page offset   */
        );

        if (ctx->buffers[i].start == MAP_FAILED) {
            perror("mmap");
            /* unmap already-mapped buffers */
            for (uint32_t j = 0; j < i; j++)
                munmap(ctx->buffers[j].start, ctx->buffers[j].length);
            goto err;
        }

        printf("[OK]  Buffer[%u] mmap'd at %p, length=%zu bytes\n",
               i, ctx->buffers[i].start, ctx->buffers[i].length);
    }

    return 0;
err:
    return -1;
}

/* ─────────────────────────────────────────
 * Step 5: Enqueue all buffers and start
 *         the capture stream
 * ───────────────────────────────────────── */
static int start_streaming(CaptureCtx *ctx)
{
    /* Enqueue all buffers into the driver's incoming queue */
    for (uint32_t i = 0; i < ctx->n_buffers; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;

        IOCTL(ctx->fd, VIDIOC_QBUF, &buf, "VIDIOC_QBUF (init)");
    }

    /* Start streaming */
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    IOCTL(ctx->fd, VIDIOC_STREAMON, &type, "VIDIOC_STREAMON");

    printf("[OK]  Streaming started\n");
    return 0;
err:
    return -1;
}

/* ─────────────────────────────────────────
 * Save frame as PGM (Portable GrayMap)
 * Viewable with: eog, feh, GIMP, etc.
 * ───────────────────────────────────────── */
static int save_pgm(const char *path,
                    const uint8_t *data,
                    uint32_t width,
                    uint32_t height)
{
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "[ERROR] Cannot create file '%s': %s\n",
                path, strerror(errno));
        return -1;
    }

    /* PGM header: magic, width, height, max_val */
    fprintf(f, "P5\n%u %u\n255\n", width, height);
    fwrite(data, 1, width * height, f);
    fclose(f);
    return 0;
}

/* ─────────────────────────────────────────
 * Save frame as raw Y8 binary
 * ───────────────────────────────────────── */
static int save_raw(const char *path,
                    const uint8_t *data,
                    size_t size)
{
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "[ERROR] Cannot create file '%s': %s\n",
                path, strerror(errno));
        return -1;
    }
    fwrite(data, 1, size, f);
    fclose(f);
    return 0;
}

/* ─────────────────────────────────────────
 * Step 6: Capture loop
 *   - select() waits for a frame
 *   - DQBUF dequeues the filled buffer
 *   - frame data is written to disk
 *   - QBUF recycles the buffer
 * ───────────────────────────────────────── */
static int capture_frames(CaptureCtx *ctx, uint32_t num_frames)
{
    uint32_t frame_count = 0;
    char pgm_path[512];
    char raw_path[512];

    printf("[OK]  Capturing %u frames → %s/\n", num_frames, ctx->output_dir);

    while (frame_count < num_frames) {

        /* ── Wait for a buffer to become ready ── */
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(ctx->fd, &fds);

        struct timeval tv = {
            .tv_sec  = SELECT_TIMEOUT_SEC,
            .tv_usec = 0
        };

        int ret = select(ctx->fd + 1, &fds, NULL, NULL, &tv);
        if (ret == -1) {
            if (errno == EINTR)
                continue;   /* interrupted by signal, retry */
            perror("select");
            return -1;
        }
        if (ret == 0) {
            fprintf(stderr, "[ERROR] select() timeout — no frame received\n");
            return -1;
        }

        /* ── Dequeue the filled buffer ── */
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (ioctl(ctx->fd, VIDIOC_DQBUF, &buf) == -1) {
            if (errno == EAGAIN)
                continue;   /* no buffer ready yet (non-blocking) */
            perror("VIDIOC_DQBUF");
            return -1;
        }

        /* ── Access frame data via mmap pointer ── */
        const uint8_t *frame_data =
            (const uint8_t *)ctx->buffers[buf.index].start;
        uint32_t bytes_used = buf.bytesused;

        printf("[FRAME %03u] buf_index=%u, bytes=%u, timestamp=%ld.%06ld\n",
               frame_count,
               buf.index,
               bytes_used,
               buf.timestamp.tv_sec,
               buf.timestamp.tv_usec);

        /* ── Save as PGM (viewable) ── */
        snprintf(pgm_path, sizeof(pgm_path),
                 "%s/frame_%04u.pgm", ctx->output_dir, frame_count);
        if (save_pgm(pgm_path, frame_data,
                     ctx->width, ctx->height) == 0)
            printf("         → Saved PGM : %s\n", pgm_path);

        /* ── Save as raw Y8 binary ── */
        snprintf(raw_path, sizeof(raw_path),
                 "%s/frame_%04u.raw", ctx->output_dir, frame_count);
        if (save_raw(raw_path, frame_data, bytes_used) == 0)
            printf("         → Saved RAW : %s\n", raw_path);

        /* ── Re-enqueue buffer for next capture ── */
        if (ioctl(ctx->fd, VIDIOC_QBUF, &buf) == -1) {
            perror("VIDIOC_QBUF (recycle)");
            return -1;
        }

        frame_count++;
    }

    printf("[OK]  Captured %u frames successfully\n", frame_count);
    return 0;
}

/* ─────────────────────────────────────────
 * Step 7: Stop streaming
 * ───────────────────────────────────────── */
static void stop_streaming(CaptureCtx *ctx)
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(ctx->fd, VIDIOC_STREAMOFF, &type) == -1)
        perror("VIDIOC_STREAMOFF");
    else
        printf("[OK]  Streaming stopped\n");
}

/* ─────────────────────────────────────────
 * Cleanup: unmap buffers, close device
 * ───────────────────────────────────────── */
static void cleanup(CaptureCtx *ctx)
{
    for (uint32_t i = 0; i < ctx->n_buffers; i++) {
        if (ctx->buffers[i].start &&
            ctx->buffers[i].start != MAP_FAILED) {
            munmap(ctx->buffers[i].start, ctx->buffers[i].length);
        }
    }

    if (ctx->fd > 0) {
        close(ctx->fd);
        printf("[OK]  Device closed\n");
    }
}

/* ─────────────────────────────────────────
 * main()
 * ───────────────────────────────────────── */
int main(int argc, char *argv[])
{
    const char *device     = (argc > 1) ? argv[1] : DEFAULT_DEVICE;
    uint32_t    num_frames = (argc > 2) ? (uint32_t)atoi(argv[2])
                                        : DEFAULT_NUM_FRAMES;
    const char *output_dir = (argc > 3) ? argv[3] : DEFAULT_OUTPUT_DIR;

    printf("===========================================\n");
    printf(" OV7251 V4L2 Frame Capture\n");
    printf(" Device     : %s\n", device);
    printf(" Frames     : %u\n", num_frames);
    printf(" Output dir : %s\n", output_dir);
    printf("===========================================\n");

    /* Create output directory if it doesn't exist */
    mkdir(output_dir, 0755);

    CaptureCtx ctx;
    memset(&ctx, 0, sizeof(ctx));
    snprintf(ctx.output_dir, sizeof(ctx.output_dir), "%s", output_dir);
    ctx.fd = -1;

    /* ── Step 1: Open device ── */
    ctx.fd = open_device(device);
    if (ctx.fd == -1) return EXIT_FAILURE;

    /* ── Step 2: Check capabilities ── */
    if (query_capabilities(ctx.fd) == -1) goto fail;

    /* ── Step 3: Set format ── */
    if (set_format(&ctx) == -1) goto fail;

    /* ── Step 4: Allocate & mmap DMA buffers ── */
    if (init_buffers(&ctx) == -1) goto fail;

    /* ── Step 5: Enqueue buffers & start stream ── */
    if (start_streaming(&ctx) == -1) goto fail;

    /* ── Step 6: Capture loop ── */
    if (capture_frames(&ctx, num_frames) == -1) {
        stop_streaming(&ctx);
        goto fail;
    }

    /* ── Step 7: Stop stream & cleanup ── */
    stop_streaming(&ctx);
    cleanup(&ctx);

    printf("===========================================\n");
    printf(" Done. Frames saved to: %s/\n", output_dir);
    printf("===========================================\n");
    return EXIT_SUCCESS;

fail:
    stop_streaming(&ctx);
    cleanup(&ctx);
    return EXIT_FAILURE;
}
