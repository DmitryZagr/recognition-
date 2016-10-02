/*#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include  <fcntl.h>
#include <unistd.h>

#include <pigpio.h>

#define NAME        "socket1"
#define MIN_SIGNAL  1000
#define AVR_SIGNAL  1500
#define MAX_SIGNAL  2000
#define DELAY       600000
#define SIGNAL_STEP 50
#define CMD_SIZE    5

#define THROTTLE_GPIO 19
#define YAW_GPIO      26
#define PITCH_GPIO    13
#define ROLL_GPIO     6

#define INCR_SIGNAL  9999
#define DECR_SIGNAL -9999

struct params
{
    int throttle; // fly       up || down
    int pitch;   //  move forward || backward
    int roll;   //   strave  left || right
    int yaw;   //    rotate  left || right
};
typedef struct params State;

void setState(int *param, int value, int GPIO)
{
    int setValue = value;
    if(value == INCR_SIGNAL) setValue = *param + SIGNAL_STEP;
    else if(value == DECR_SIGNAL) setValue = *param - SIGNAL_STEP;
    else
    {
        if(value > MAX_SIGNAL) setValue = MAX_SIGNAL;
        if(value < MIN_SIGNAL) setValue = MIN_SIGNAL;
    }
    *param = setValue; //RPI Delete
    if(gpioServo(GPIO, setValue) == 0) *param = setValue;
    else printf("Error GPIO set servo\n");
    if(value == INCR_SIGNAL || value == DECR_SIGNAL)
    {
        if(GPIO != THROTTLE_GPIO && value < MAX_SIGNAL && value > MIN_SIGNAL)
        {
            usleep(DELAY);
//            *param = value; //RPI Delete
            if(gpioServo(GPIO, value) == 0) *param = value;
            else printf("GPIO hasn't returned to old value, it's really bad!\n");
        }
        
    }
}

int initDevice(State *state)
{
    //Init GPIO
    if (gpioInitialise() < 0)
    {
        return 0;
    }
    //Set output to default
    setState(&state->throttle, MIN_SIGNAL, THROTTLE_GPIO);
    setState(&state->pitch,    AVR_SIGNAL,    PITCH_GPIO);
    setState(&state->roll,     AVR_SIGNAL,     ROLL_GPIO);
    setState(&state->yaw,      AVR_SIGNAL,      YAW_GPIO);
    return 1;
}

void deinitDevice()
{
    gpioTerminate();
}

void stickCentre(int *value, int centre, int GPIO)
{
    if((*value -= *value%SIGNAL_STEP) > centre)
    {
        while (*value != centre)
        {
            *value -= SIGNAL_STEP;
            if(gpioServo(GPIO, *value) != 0)
            {
                *value += SIGNAL_STEP;
                printf("Error GPIO set servo\n");
                break;
            }
            printf("Cur state: %i\n", *value);
            usleep(DELAY);
        }
    }
    else
    {
        while (*value != centre)
        {
            *value += SIGNAL_STEP;
            if(gpioServo(GPIO, *value) != 0)
            {
                *value -= SIGNAL_STEP;
                printf("Error GPIO set servo\n");
                break;
            }
            printf("Cur state: %i\n", *value);
            usleep(DELAY);
        }
    }
}

void freezeDevice(State *state)
{
    stickCentre(&state->yaw,   AVR_SIGNAL,   YAW_GPIO);
    stickCentre(&state->pitch, AVR_SIGNAL, PITCH_GPIO);
    stickCentre(&state->roll,  AVR_SIGNAL,  ROLL_GPIO);
}

void landDevice(State *state)
{
    freezeDevice(state);
    state->throttle -= state->throttle%SIGNAL_STEP;
    while (state->throttle != MIN_SIGNAL)
    {
        state->throttle -= SIGNAL_STEP;
        printf("Cur state throttle: %i\n", state->throttle);
        setState(&state->throttle, state->throttle, THROTTLE_GPIO);
        usleep(DELAY);
    }
}

void initConnection(int *sock, char *socketName, int *msgsock, struct sockaddr_un *server)
{
    unlink(socketName); //Just in case something is using it ¯\_(ツ)_/¯
    *sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*sock < 0)
    {
        perror("opening stream socket");
        exit(1);
    }
    server->sun_family = AF_UNIX;
    strcpy(server->sun_path, socketName);
    if (bind(*sock, (struct sockaddr *) server, sizeof(struct sockaddr_un)))
    {
        perror("binding stream socket");
        exit(1);
    }
    printf("Socket has name %s\n", server->sun_path);
    listen(*sock, 5);
    *msgsock = accept(*sock, 0, 0);
}

void deinitConnection(int *msgsock, int *sock, char *socketName)
{
    close(*msgsock);
    close(*sock);
    unlink(socketName);
}

int commandHandler(char* command, int value, State *state)
{
    if(strcmp(command,"kill") == 0)
    {
        landDevice(state);
        printf("Landed\n");
        printf("Daemon killed\n");
        return 0;
    }
    if(strcmp(command,"thro") == 0)
    {
        printf("Throttle changed from %i ", state->throttle);
        setState(&state->throttle, value, THROTTLE_GPIO);
        printf("to %i\n", state->throttle);
        return 1;
    }
    if(strcmp(command,"pitc") == 0)
    {
        printf("Pitch changed from %i ", state->pitch);
        setState(&state->pitch, value, PITCH_GPIO);
        printf("to %i\n", state->pitch);
        return 1;
    }
    if(strcmp(command,"roll") == 0)
    {
        printf("Roll changed from %i ", state->roll);
        setState(&state->roll, value, ROLL_GPIO);
        printf("to %i\n", state->roll);
        return 1;
    }
    if(strcmp(command,"yaw") == 0)
    {
        printf("Yaw changed from %i ", state->yaw);
        setState(&state->yaw, value, YAW_GPIO);
        printf("to %i\n", state->yaw);
        return 1;
    }
    if(strcmp(command,"land") == 0)
    {
        landDevice(state);
        printf("Landed\n");
        return 1;
    }
    if(strcmp(command,"frez") == 0)
    {
        freezeDevice(state);
        printf("Device freezed in air\n");
        return 1;
    }
    return 2;
}

int main(void)
{
    int sock, msgsock, rval;
    struct sockaddr_un server;
    char cmd[CMD_SIZE];
    int value;
    State state;
    if(initDevice(&state) == 0) return 1;
    initConnection(&sock, NAME, &msgsock, &server);
    while (commandHandler(cmd, value, &state) != 0)
    {
        if (msgsock == -1) perror("accept");
        else do
        {
            bzero(cmd, CMD_SIZE);
            if (rval = recv(msgsock, cmd, CMD_SIZE, 0) < 0) perror("reading stream message");
            if (rval = recv(msgsock, &value, sizeof(int), 0) < 0) perror("reading stream message");
            else printf("Received: %s %i\n", cmd, value);
        } while (rval > 0);
        //Sending notifications (disabled for now)
//        char condition = 1;
//        if (send(msgsock, &condition, 1, 0) < 0) perror("writing on stream socket");
    }
    deinitConnection(&msgsock, &sock, NAME);
    deinitDevice();
    return 0;
}*/