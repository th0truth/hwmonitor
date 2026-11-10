#include "base.h"

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int
serial_open(const char *port, speed_t baud)
{
    if (port == NULL) {
        fprintf(stderr, "error: serial port path is NULL\n");
        return -1;
    }

    int serial_port = open(port, O_RDWR | O_NOCTTY | O_SYNC);

    if (serial_port < 0) {
        fprintf(stderr, "error: failed to open '%s': %s\n", port, strerror(errno));
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    
    if (tcgetattr(serial_port, &tty) != 0) {
        fprintf(stderr, "error: tcgetattr failed on '%s': %s\n", port, strerror(errno));
        close(serial_port);
        return -1;
    }

    /* Set up number of 8 bits per byte */
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB; 
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    
    /* Ignore modem control lines and enable receiver*/
    tty.c_cflag |= (CLOCAL | CREAD);

    /* Disable canonical mode */
    tty.c_lflag &= ~ICANON;

    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    
    /* Disable interpretation of INTR, QUIT and SUSP */
    tty.c_lflag &= ~ISIG; 

    /* Disable software control flow (XON/XOFF)*/
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    /* Disable any special handling of output chars/bytes*/
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN]  = 0;

    /* Set up baud rate */
    cfsetispeed(&tty, baud);
    cfsetospeed(&tty, baud);

    tcflush(serial_port, TCIOFLUSH);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        fprintf(stderr, "error: tcsetattr on '%s': %s\n", port, strerror(errno));
        close(serial_port);
        return -1;
    }

    return serial_port;
}


ssize_t
serial_write(int serial_port, const char *buff)
{
    if (serial_port < 0 || serial_port == NULL) {
        return -1;
    }

    size_t total_written = 0;
    size_t buff_size = sizeof(buff);

    while (total_written < buff_size) {
        size_t written = write(serial_port, buff, buff_size);

        if (written == 0) {
            if (errno == EINTR) {
                continue;
            }
            
            fprintf(stderr, "error: write to serial port failed: %s\n", strerror(errno));
            return -1;
        }
        
        total_written += written;
    }

    return (ssize_t)total_written;
}

void
serial_close(int serial_port)
{
    if (serial_port >= 0) {
        tcdrain(serial_port);
        close(serial_port);
    }
}