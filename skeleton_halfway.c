#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// ? This is used to move the location of a specific joint
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

void wait_until_done(int time) {
	usleep(time);
}

// ? Opens the grabber
void openGrabber(int connection) { 
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(2000000);
}

// ? Closes the grabber on the block
void closeGrabber(int connection) {
	move_to_location(connection,5,0x01,0x35);
	wait_until_done(2000000);
}

// ? This reset the arm back to the upright position
void resetArm(int connection) {
	move_to_location(connection, 2, 0x01, 0xff);
	move_to_location(connection, 3, 0x01, 0xff);
	move_to_location(connection, 4, 0x01, 0xff);
	wait_until_done(2000000);
}

// ? Performs movement ot the different piles
void lateralMovement(int connection, int pile) {
	unsigned int piles[] = {0xff, 0x95, 0x35};
	move_to_location(connection, 1, 0x01, piles[pile]);
	wait_until_done(2000000);
}

// Moves the arm first over the correct pile then the correct layer
void pickup(int connection, int layer, int pile) {
	// Resetting the arm to neutral
	resetArm(connection);

	// Making sure the grabber is open
	openGrabber(connection);

	// Moving arm over correct pile
	lateralMovement(connection, pile);

	// Moving the arm to the correct layer
	switch(layer) {
		case 0: layer0(connection, 0x00); break;
		case 1: layer1(connection, 0x00); break;
		default: layer2(connection, 0x00); break;
	}

	wait_until_done(3000000);
	// Grip the block
	closeGrabber(connection);
	wait_until_done(4000000);

	// Return the neutral position
	resetArm(connection);
}

// Moves the arm first over the correct pile then the correct layer
void dropoff(int connection, int layer, int pile) {
	// Resetting the arm to neutral
	resetArm(connection);

	// Moving arm over correct pile
	lateralMovement(connection, pile);


	// Moving the arm to the correct layer
	switch(layer) {
		case 0: layer0(connection, 0); break;
		case 1: layer1(connection, 0); break;
		default: layer2(connection, 0x10); break;
	}



	wait_until_done(3000000);
	

	// Grip the block
	openGrabber(connection);
	wait_until_done(1000000);

	// Return the neutral position
	resetArm(connection);
}


void layer2(int connection, int clearance) {
	move_to_location(connection,2,0x01,0x30 + clearance);
	move_to_location(connection,3,0x01,0x9a);
	move_to_location(connection,4,0x01,0x00);
}

void layer1(int connection, int clearance) {
	move_to_location(connection,2,0x01,0x10 + clearance);
	move_to_location(connection,3,0x01,0xb0);
	move_to_location(connection,4,0x01,0x00);

}

void layer0(int connection, int clearance) {
	move_to_location(connection,2,0x01,0x00 + clearance);
	move_to_location(connection,3,0x01,0xaa);
	move_to_location(connection,4,0x01,0x00);

}


int main(int argc, char* argv[]) {
	// ? Do we make the connection a global variable?
	int connection = open_connection("/dev/ttyUSB0",B1000000);

	openGrabber(connection);


	/* 
	? The sequence of movements we need to make
	! Start !
	* Step 0: Layer 2 Pile 0 -> Layer 0 Pile 2
	* Step 1: Layer 1 Pile 0 -> Layer 0 Pile 1
	* Step 2: Layer 0 Pile 2 -> Layer 2 Pile 1
	* Step 3: Layer 0 Pile 0 -> Layer 0 Pile 2
	* Step 4: Layer 1 Pile 1 -> Layer 0 Pile 0
	* Step 5: Layer 0 Pile 1 -> Layer 1 Pile 2
	* Step 6: Layer 0 Pile 0 -> Layer 2 Pile 2
	! Done !
	*/ 
	resetArm(connection);
	
	
	// Step 0
	pickup(connection, 2, 0);
	dropoff(connection, 0, 2);

	// Step 1
	pickup(connection, 1, 0);
	dropoff(connection, 0, 1);

	// Step 2
	pickup(connection, 0, 2);
	dropoff(connection, 2, 1);

	// Step 3
	pickup(connection, 0, 0);
	dropoff(connection, 0, 2);

	// Step 4 
	pickup(connection, 1, 1);
	dropoff(connection, 0, 0);

	// Step 5
	pickup(connection, 0, 1);
	dropoff(connection, 1, 2);

	// Step 6
	pickup(connection, 0, 0);
	dropoff(connection, 2, 2);




	return 0;
}





