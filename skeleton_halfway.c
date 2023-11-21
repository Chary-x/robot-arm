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

// ? Opens the grabber
void open_grabber(int connection) { 
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection,5);
}

// ? grabs the block
void close_grabber(int connection) {
	move_to_location(connection,5,0x01,0x55);
	wait_until_done(connection,5);
}


void reset_arm(int connection) {
	move_to_location(connection, 2, 0x01, 0xff);
	move_to_location(connection, 3, 0x01, 0xff);
	move_to_location(connection, 4, 0x01, 0xff);
	wait_until_done(connection, 4);
}

void lateral_movement(int connection, int pile) {
	// todo figure out how to make it move to the right
	//unsigned int offset = pile * 0x55;
	//move_to_location(connection,1,0x01,offset);
	unsigned int[] piles = {0xff, 0x35, 0x95};
	move_to_location(connection, 1, 0x01, piles[pile]);
	wait_until_done(connection, 1);
}

void pickup(int connection, int pile, int layer) {
	unsigned int pile_offset, layer_offset;

}

void LAYER_2_PILE_0(int connection) {
	move_to_location(connection,2,0x01,0x11);
	move_to_location(connection,3,0x01,0xcf);
	move_to_location(connection,4,0x01,0x00);
	wait_until_done(connection,4);
}

void LAYER_1_PILE_0(int connection) {
	move_to_location(connection,2,0x01,0x00);
	move_to_location(connection,3,0x01,0xcf);
	move_to_location(connection,4,0x01,0x00);
	wait_until_done(connection,4);
}

void LAYER_0_PILE_0(int connection) {
	move_to_location(connection,2,0x01,0x00);
	move_to_location(connection,3,0x01,0x93);
	move_to_location(connection,4,0x01,0x50);
	wait_until_done(connection,4);
}

int main(int argc, char* argv[]) {

	// LAYER_2_PILE_0 = move_to_location(connection,1,0x01,0xff);
	//					move_to_location(connection,2,0x01,0x11);
	//					move_to_location(connection,3,0x01,0xcf);
	//					move_to_location(connection,4,0x01,0x00);
	//					wait_until_done(connection,4);
	//

	// LAYER_1_PILE_0 = move_to_location(connection,1,0x01,0xff);
	//					move_to_location(connection,2,0x01,0x00);
	//					move_to_location(connection,3,0x01,0xcf);
	//					move_to_location(connection,4,0x01,0x00);
	//					wait_until_done(connection,4);

	// LAYER_0_PILE_0 = move_to_location(connection,1,0x01,0xff);
	//					move_to_location(connection,2,0x01,0x00);
	//					move_to_location(connection,3,0x01,0x93);
	//					move_to_location(connection,4,0x01,0x50);
	//

	int connection = open_connection("/dev/ttyUSB0",B1000000);

	reset_arm(connection);
	//pen_grabber(connection);
	//close_grabber(connection);
	lateral_movement(connection, 0);
	open_grabber(connection);
	LAYER_2_PILE_0(connection);
	close_grabber(connection);
	reset_arm(connection);
	lateral_movement(connection, 1);
	LAYER_0_PILE_0(connection);
	open_grabber(connection);

	reset_arm(connection);

	//lateral_movement(connection, 1);

	return 0;
}





