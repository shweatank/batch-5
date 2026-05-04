// SPDX-License-Identifier: GPL-2.0
/*
 * user_test.c - User-space test for basic_char driver
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <errno.h>

#define DEVICE "/dev/basic_char"
#define BUF_SIZE 4096

#define BASIC_CHAR_MAGIC      'B'
#define IOCTL_CLEAR_BUFFER    _IO(BASIC_CHAR_MAGIC, 0)
#define IOCTL_GET_BUFLEN      _IOR(BASIC_CHAR_MAGIC, 1, int)

#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) do { printf("[FAIL] %s\n", msg); exit(1); } while (0)

int main(void)
{
    int fd, ret, len, poll_ret;
    char wbuf[] = "Hello, kernel!";
    char rbuf[BUF_SIZE] = {0};
    struct pollfd pfd;

    // Open device
    fd = open(DEVICE, O_RDWR);
    if (fd < 0)
        TEST_FAIL("open device");

    TEST_PASS("open device");

    // Write to device
    ret = write(fd, wbuf, strlen(wbuf));
    if (ret != (int)strlen(wbuf))
        TEST_FAIL("write device");
    TEST_PASS("write device");

    // Read from device
    lseek(fd, 0, SEEK_SET);
    ret = read(fd, rbuf, sizeof(rbuf));
    if (ret != (int)strlen(wbuf) || memcmp(rbuf, wbuf, strlen(wbuf)) != 0)
        TEST_FAIL("read device");
    TEST_PASS("read device");

    // IOCTL get buffer length
    len = 0;
    ret = ioctl(fd, IOCTL_GET_BUFLEN, &len);
    if (ret < 0 || len != (int)strlen(wbuf))
        TEST_FAIL("ioctl get buffer length");
    TEST_PASS("ioctl get buffer length");

    // IOCTL clear buffer
    ret = ioctl(fd, IOCTL_CLEAR_BUFFER);
    if (ret < 0)
        TEST_FAIL("ioctl clear buffer");
    TEST_PASS("ioctl clear buffer");

    // Read after clear (should be empty)
    lseek(fd, 0, SEEK_SET);
    ret = read(fd, rbuf, sizeof(rbuf));
    if (ret != 0)
        TEST_FAIL("read after clear");
    TEST_PASS("read after clear");

    // Poll test
    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT;
    poll_ret = poll(&pfd, 1, 1000);
    if (poll_ret < 0)
        TEST_FAIL("poll failed");
    if (!(pfd.revents & POLLOUT))
        TEST_FAIL("poll: POLLOUT not set");
    TEST_PASS("poll: POLLOUT set");

    // Write again for POLLIN
    ret = write(fd, wbuf, strlen(wbuf));
    if (ret != (int)strlen(wbuf))
        TEST_FAIL("write for poll");
    poll_ret = poll(&pfd, 1, 1000);
    if (poll_ret < 0)
        TEST_FAIL("poll after write failed");
    if (!(pfd.revents & POLLIN))
        TEST_FAIL("poll: POLLIN not set");
    TEST_PASS("poll: POLLIN set");

    // llseek test
    ret = lseek(fd, 2, SEEK_SET);
    if (ret != 2)
        TEST_FAIL("llseek set");
    ret = read(fd, rbuf, 2);
    if (ret != 2)
        TEST_FAIL("llseek read");
    TEST_PASS("llseek");

    close(fd);
    TEST_PASS("close device");
    return 0;
}
