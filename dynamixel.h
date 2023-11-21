#ifndef _DYNAMIXEL_H
#define _DYNAMIXEL_H

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * opens a serial connection.
 * @param portname string literal name of serial device (for dynamixel usually "/dev/ttyUSB0").
 * @param baud the baudrate of the connection, possible values are found in termios.h (for dynamixel, value should be B1000000).
 * @return the file descriptor for the open connection.
 */
int open_connection(char *portname, speed_t baud);


/**
 * sets connection attributes such as baudrate and parity.
 * @param fd the file descriptor of the connection to be changed.
 * @param speed the baudrate of the connection, possible values are found in termios.h (for dynamixel, value should be B1000000).
 * @return zero on succes, non-zero otherwise.
 */
int set_interface_attribs(int fd, speed_t speed);

/**
 * changes a connection to blocking or non-blocking.
 * @param fd the file descriptor of the connection to be changed.
 * @param should_block a 0 or 1 value stating whether the connection should block.
 */
void set_blocking(int fd, int should_block);

/**
 * closes a serial connection.
 * @param fd the file descriptor of the connection to be closed.
 */
void close_connection(int fd);

/**
 * writes a packet to a given serial connection and puts the response into the given buffer.
 * @param fd the file descriptor of the connection to be written to.
 * @param raw_packet a pointer to the raw array of bytes that make up the packet.
 * @param raw_len the length of the raw packet.
 * @param buffer the buffer to store the response from the serial connection.
 * @param buff_size the size of the buffer.
 * @return the number of bytes that were read into the buffer.
 */
int write_to_connection(int fd, unsigned char *raw_packet, int raw_len, unsigned char *buffer, int buff_size);

#endif
