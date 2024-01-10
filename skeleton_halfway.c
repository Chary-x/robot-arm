#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int disksOnPile[] = {2, -1, -1};

// This is used to move the location of a specific joint
void move_to_location(int connection, unsigned char id, unsigned char loc_h,
                      unsigned char loc_l) {

  unsigned char cs = ~(id + 0x07 + 0x03 + 0x1e + loc_l + loc_h + 0x30 + 0x00);

  unsigned char arr[] = {0xff,  0xff,  id,   0x07, 0x03, 0x1e,
                         loc_l, loc_h, 0x30, 0x00, cs};

  int buff_len = 100;
  unsigned char buff[buff_len];

  int bytes_read = write_to_connection(connection, arr, 11, buff, buff_len);
}

void wait_until_done(int time) { usleep(time); }

// Opens the grabber
void openGrabber(int connection) {
  move_to_location(connection, 5, 0x01, 0xff);
  wait_until_done(2000000);
}

// Closes the grabber on the block
void closeGrabber(int connection) {
  move_to_location(connection, 5, 0x01, 0x35);
  wait_until_done(2000000);
}

// This reset the arm back to the upright position
void resetArm(int connection) {
  move_to_location(connection, 2, 0x01, 0xff);
  move_to_location(connection, 3, 0x01, 0xff);
  move_to_location(connection, 4, 0x01, 0xff);
  wait_until_done(2000000);
}

// Performs movement to different piles
void lateralMovement(int connection, int pile) {
  unsigned int piles[] = {0xff, 0x95, 0x35};
  move_to_location(connection, 1, 0x01, piles[pile]);
  wait_until_done(2000000);
}

// Performs movement to different layers
void verticalMovement(int connection, int layer) {
  unsigned int layers[3][3] = {
      {0x30, 0x9a, 0x00}, {0x10, 0xb0, 0x00}, {0x00, 0xaa, 0x00}};

  int i = 2;
  for (i = 2; i < 5; i++) {
    move_to_location(connection, i, 0x01, layers[layer][i - 2]);
  }
}

// Moves the arm first over the correct pile then the correct layer
void pickup(int connection, int from) {
  // Resetting the arm to neutral
  resetArm(connection);

  // Making sure the grabber is open
  openGrabber(connection);

  // Moving arm over correct pile
  lateralMovement(connection, from);

  // Moving the arm to the correct layer
  verticalMovement(connection, disksOnPile[from]);

  wait_until_done(3000000);
  // Grip the block
  closeGrabber(connection);
  wait_until_done(4000000);

  // Return the neutral position
  resetArm(connection);
}

void dropoff(int connection, int to) {
  // Resetting the arm to neutral
  resetArm(connection);

  // Moving arm over correct pile
  lateralMovement(connection, to);

  // Moving the arm to the correct layer
  verticalMovement(connection, disksOnPile[to]);

  wait_until_done(3000000);

  // Grip the block
  openGrabber(connection);
  wait_until_done(1000000);

  // Return the neutral position
  resetArm(connection);
}

// Updates appropriate state regarding amount of disks on each pile
void updateDisksOnPiles(int connection, int from, int to) {
  disksOnPile[to]++;

  pickup(connection, from);
  dropoff(connection, to);

  disksOnPile[from]--;
}

// This is the recursive algorithm that calculates which piles and layers to
// move to
char towerOfHanoi(int connection, int n, int from, int to, int aux) {
  if (n == 0) {
    return 'D';
  }

  towerOfHanoi(connection, n - 1, from, aux, to);
  updateDisksOnPiles(connection, from, to);
  towerOfHanoi(connection, n - 1, aux, to, from);
}

// Main program flow
int main(void) {
  int connection = open_connection("/dev/ttyUSB0", B1000000);

  // Ensuring robot is in neutral state before starting
  openGrabber(connection);
  resetArm(connection);

  towerOfHanoi(connection, 3, 0, 2, 1);
  return 0;
}
