#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include  <fcntl.h>
#include <unistd.h>

#define NAME "socket1"

#define MIN_SIGNAL  1000
#define AVR_SIGNAL  1500
#define MAX_SIGNAL  2000
#define SIGNAL_STEP 50
#define CMD_SIZE    5

#define THROTTLE_CMD "thro"
#define YAW_CMD      "yaw"
#define PITCH_CMD    "pitc"
#define ROLL_CMD     "roll"

#define INCR_SIGNAL  9999
#define DECR_SIGNAL -9999

#define NO_PARAMETER -1

/*
 All you need to send messages:
 
 int sock;
 struct sockaddr_un server;
 initConnection(&sock, NAME, &server); (NAME defined in header)
 
 DO SOME LOGIC HERE!
 
 deinitConnection(&sock);
 
 */

//Initialize / deinitialize connection to device server
void initConnection(int *sock, char *socketName, struct sockaddr_un *server);
void deinitConnection(int *sock);

/*
 Next functions move device in any
 direction, taking movement parameter
 in percents from 0 to 100
 */

/*
 These functions suitable for
 RC like controll (without dividing
 vector of direction)
 */

void heightValue(int *sock, int value);
void yawValue(int *sock, int value);
void pitchValue(int *sock, int value);
void rollValue(int *sock, int value);

/*
 These functions has
 dividing by vector
 of direction
 */

void turnRight(int *sock, int value);
void turnLeft(int *sock, int value);

void straveRight(int *sock, int value);
void straveLeft(int *sock, int value);

void moveForward(int *sock, int value);
void moveBackward(int *sock, int value);

/*
 Next functions move device in any
 direction, but for a little distance
 (or angle). Take no value of movement
 */

//Move forward / backward for a bit
void moveStepForward(int *sock);
void moveStepBackward(int *sock);

//Strave right / left for a bit
void straveStepRight(int *sock);
void straveStepLeft(int *sock);

//Go up / down for a bit
void moveStepUp(int *sock);
void moveStepDown(int *sock);

//Turn right / left for a bit
void turnStepRight(int *sock);
void turnStepLeft(int *sock);

//Device saves altitude of flight
//but stays still in the air
void freezeDeviceInAir(int *sock);

//Just land the device
void landDevice(int *sock);

//Land device and kills its server,
//making it UNREACHEABLE!
void killDeviceServer(int *sock);