#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/types.h>
#include <termios.h>

#define BAUD_RATE B115200

int serial_open(const char *port, speed_t baud);
ssize_t serial_write(int serial_port, const char *buff);
void serial_close(int serial_port);

#ifdef __cplusplus
}
#endif /* __cplusplus */
