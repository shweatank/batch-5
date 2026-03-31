//client.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define SERVER_IP "10.40.97.202"   // replace with your PC's IP
#define PORT 5000

#define SRC_WIDTH 640
#define SRC_HEIGHT 480
#define SRC_BYTES_PER_PIXEL 2
#define SRC_FRAME_SIZE (SRC_WIDTH * SRC_HEIGHT * SRC_BYTES_PER_PIXEL)

#define DST_WIDTH 176
#define DST_HEIGHT 220
#define DST_BYTES_PER_PIXEL 2
#define DST_FRAME_SIZE (DST_WIDTH * DST_HEIGHT * DST_BYTES_PER_PIXEL)

static void yuyv_to_rgb565(unsigned char *yuyv, unsigned char *rgb) {
	int y0 = yuyv[0];
	int u  = yuyv[1] - 128;
	int y1 = yuyv[2];
	int v  = yuyv[3] - 128;

	int r0 = y0 + 1.402 * v;
	int g0 = y0 - 0.344 * u - 0.714 * v;
	int b0 = y0 + 1.772 * u;

	int r1 = y1 + 1.402 * v;
	int g1 = y1 - 0.344 * u - 0.714 * v;
	int b1 = y1 + 1.772 * u;

	r0 = r0 < 0 ? 0 : r0 > 255 ? 255 : r0;
	g0 = g0 < 0 ? 0 : g0 > 255 ? 255 : g0;
	b0 = b0 < 0 ? 0 : b0 > 255 ? 255 : b0;

	r1 = r1 < 0 ? 0 : r1 > 255 ? 255 : r1;
	g1 = g1 < 0 ? 0 : g1 > 255 ? 255 : g1;
	b1 = b1 < 0 ? 0 : b1 > 255 ? 255 : b1;

	unsigned short pixel0 = ((r0 & 0xF8) << 8) | ((g0 & 0xFC) << 3) | (b0 >> 3);
	unsigned short pixel1 = ((r1 & 0xF8) << 8) | ((g1 & 0xFC) << 3) | (b1 >> 3);

	rgb[0] = pixel0 & 0xFF;
	rgb[1] = pixel0 >> 8;
	rgb[2] = pixel1 & 0xFF;
	rgb[3] = pixel1 >> 8;
}

int main() {
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);

	while (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect");
		sleep(1); // retry until server is ready
	}
	printf("Connected to server\n");

	int fd = open("/dev/video1", O_WRONLY);
	if (fd < 0) { perror("open"); return 1; }

	unsigned char *srcbuf = malloc(SRC_FRAME_SIZE);
	unsigned char *dstbuf = malloc(DST_FRAME_SIZE);

	int x_ratio = (SRC_WIDTH << 16) / DST_WIDTH + 1;
	int y_ratio = (SRC_HEIGHT << 16) / DST_HEIGHT + 1;

	while (1) {
		// Read frame header
		uint32_t size;
		ssize_t n = recv(sockfd, &size, sizeof(size), MSG_WAITALL);
		if (n != sizeof(size)) { perror("recv header"); break; }
		if (size != SRC_FRAME_SIZE) { fprintf(stderr, "Unexpected frame size\n"); break; }

		// Read frame data
		ssize_t received = 0;
		while (received < SRC_FRAME_SIZE) {
			ssize_t r = recv(sockfd, srcbuf + received, SRC_FRAME_SIZE - received, 0);
			if (r <= 0) { perror("recv data"); goto cleanup; }
			received += r;
		}
		printf("Frame received: %d bytes\n", received);

		// Convert + resize
		int out_index = 0,src_y,src_x,src_index,y,x;
		for ( y = 0; y < DST_HEIGHT; y++) {
			 src_y = (y * y_ratio) >> 16;
			for ( x = 0; x < DST_WIDTH; x += 2) {
				 src_x = (x * x_ratio) >> 16;
				 src_index = (src_y * SRC_WIDTH + src_x) * 2;
				if (src_index + 3 < SRC_FRAME_SIZE) {
					yuyv_to_rgb565(srcbuf + src_index, dstbuf + out_index);
				}
				out_index += 4;
			}
		}

		write(fd, dstbuf, DST_FRAME_SIZE);
	}

cleanup:
	free(srcbuf);
	free(dstbuf);
	close(fd);
	close(sockfd);
	return 0;
}
