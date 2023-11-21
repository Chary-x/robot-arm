#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void move_to_location(int connection, unsigned char id,
			unsigned char loc_h, unsigned char loc_l) {

	unsigned char cs = ~ ( id + 0x07 + 0x03 + 0x1e + loc_l + loc_h +
				0x30 + 0x00);

	unsigned char arr[] = { 0xff, 0xff, id, 0x07, 0x03, 0x1e, loc_l,
                                       loc_h, 0x30, 0x00, cs };

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,11,buff,buff_len);

}

void wait_until_done(int connection, unsigned char id) {
	usleep(2000000);
}

int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);

	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection,5);

	move_to_location(connection,1,0x01,0xff);
	move_to_location(connection,2,0x01,0xff);
	move_to_location(connection,3,0x01,0xff);
	move_to_location(connection,4,0x01,0xff);
	wait_until_done(connection,4);

	return 0;

}

// Opens the grabber
void open_grabber() { }

// grabs the block
void grab_block() { }


