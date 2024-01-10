#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// To be used for calculting disks on a specific pile

int blocksOnPiles[] = {2, -1, -1};

// Initialise connection with robot arm
int connection = open_connection("/dev/ttyUSB0", B1000000);

// This is used to move the location of a specific joint
void move_to_location(unsigned char id, unsigned char loc_h,
                      unsigned char loc_l) {

  unsigned char cs = ~(id + 0x07 + 0x03 + 0x1e + loc_l + loc_h + 0x30 + 0x00);

  unsigned char arr[] = {0xff,  0xff,  id,   0x07, 0x03, 0x1e,
                         loc_l, loc_h, 0x30, 0x00, cs};

  int buff_len = 100;
  unsigned char buff[buff_len];

  // Global connection passed here
  int bytes_read = write_to_connection(connection, arr, 11, buff, buff_len);
}

void wait_until_done(int time) { usleep(time); }

// Opens the grabber
void openGrabber() {
  move_to_location(5, 0x01, 0xff);
  wait_until_done(2000000);
}

// Closes the grabber on the block
void closeGrabber() {
  move_to_location(5, 0x01, 0x35);
  wait_until_done(2000000);
}

// This reset the arm back to the upright position
void resetArm() {
  move_to_location(2, 0x01, 0xff);
  move_to_location(3, 0x01, 0xff);
  move_to_location(4, 0x01, 0xff);
  wait_until_done(2000000);
}

// Performs movement to different piles
void lateralMovement(int pile) {
  unsigned int piles[] = {0xff, 0x95, 0x35};
  move_to_location(1, 0x01, piles[pile]);
  wait_until_done(2000000); 
}

// Performs movement to different layers
void verticalMovement(int layer) {
  unsigned int layers[3][3] = {
      {0x30, 0x9a, 0x00}, {0x10, 0xb0, 0x00}, {0x00, 0xaa, 0x00}};

  int i = 2;
  for (i = 2; i < 5; i++) {
    move_to_location(i, 0x01, layers[layer][i - 2]);
  }
}

// Moves the arm first over the correct pile then the correct layer
void pickup(int sourcePile) {
  // Resetting the arm to neutral
  resetArm();

  // Making sure the grabber is open
  openGrabber();

  // Moving arm over correct pile
  lateralMovement(sourcePile);

  // Moving the arm to the correct layer
  verticalMovement(blocksOnPiles[sourcePile]);

  wait_until_done(3000000);
  // Grip the block
  closeGrabber();
  wait_until_done(4000000);

  // Return the neutral position
  resetArm();
}

void dropoff(int targetPile) {
  // Resetting the arm to neutral
  resetArm();

  // Moving arm over correct pile
  lateralMovement(targetPile);

  // Moving the arm to the correct layer
  verticalMovement(blocksOnPiles[targetPile]);

  wait_until_done(3000000);

  // Grip the block
  openGrabber();
  wait_until_done(1000000);

  // Return the neutral position
  resetArm();
}

// Updates appropriate state regarding amount of disks on each pile
void updateDisksOnPiles(int sourcePile, int targetPile) {
  blocksOnPiles[targetPile]++;

  pickup(sourcePile);
  dropoff(targetPile);

  blocksOnPiles[sourcePile]--;
}

// This is the recursive algorithm that calculates which piles and layers to
// move to
char towerOfHanoi(int n, int sourcePile, int targetPile, int auxiliaryPile) {
  if (n == 0) {
    return 'D';
  }

  towerOfHanoi(n - 1, sourcePile, auxiliaryPile, targetPile);
  updateDisksOnPiles(sourcePile, targetPile);
  towerOfHanoi(n - 1, auxiliaryPile, targetPile, sourcePile);
}

// Main program flow
int main(void) {

  // Ensuring robot is in neutral state before starting
  openGrabber();
  resetArm();
  
  int n = 3
  towerOfHanoi(n, 0, 2, 1);
  return 0;
}
