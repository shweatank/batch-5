//server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <stdint.h>

#define PORT 5000
#define SRC_WIDTH 640
#define SRC_HEIGHT 480
#define SRC_BYTES_PER_PIXEL 2
#define SRC_FRAME_SIZE (SRC_WIDTH * SRC_HEIGHT * SRC_BYTES_PER_PIXEL)

struct buffer {
	void *start;
	size_t length;
};

int main() {
	// TCP setup
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	listen(sockfd, 1);
	printf("Waiting for rasperripi to connect...\n");
	int clientfd = accept(sockfd, NULL, NULL);
	printf("rasperriPi connected!\n");

	// Open camera
	int fd = open("/dev/video0", O_RDWR);
	if (fd < 0) { perror("open camera"); return 1; }

	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = SRC_WIDTH;
	fmt.fmt.pix.height = SRC_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	ioctl(fd, VIDIOC_S_FMT, &fmt);

	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof(req));
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ioctl(fd, VIDIOC_REQBUFS, &req);

	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	ioctl(fd, VIDIOC_QUERYBUF, &buf);

	struct buffer buffer;
	buffer.length = buf.length;
	buffer.start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

	ioctl(fd, VIDIOC_QBUF, &buf);
	int type = buf.type;
	ioctl(fd, VIDIOC_STREAMON, &type);

	while (1) {
		if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) { 
perror("DQBUF"); break; }

		// Send frame header (size)
		uint32_t size = SRC_FRAME_SIZE;
		if (send(clientfd, &size, sizeof(size), 0) != sizeof(size)) {
			perror("send header"); break;
		}

		// Send frame data
		ssize_t sent = 0;
		while (sent < SRC_FRAME_SIZE) {
			ssize_t n = send(clientfd, buffer.start + sent, SRC_FRAME_SIZE - sent, 0);
			if (n <= 0) { perror("send data"); break; }
			sent += n;
		}
		printf("Frame sent: %ld bytes\n", sent);

		ioctl(fd, VIDIOC_QBUF, &buf);
	}

	ioctl(fd, VIDIOC_STREAMOFF, &type);
	munmap(buffer.start, buffer.length);
	close(fd);
	close(clientfd);
	close(sockfd);
	return 0;
}
