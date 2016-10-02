#include "sender.h"

void heightValue(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 10;
    value += MIN_SIGNAL;
    if (send(*sock, THROTTLE_CMD, CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value,    sizeof(int), 0) < 0) perror("writing on stream socket");
}

void yawValue(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 10;
    value += MIN_SIGNAL;
    if (send(*sock, YAW_CMD,   CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void pitchValue(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 10;
    value += MIN_SIGNAL;
    if (send(*sock, PITCH_CMD, CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void rollValue(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 10;
    value += MIN_SIGNAL;
    if (send(*sock, ROLL_CMD,  CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void turnRight(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += AVR_SIGNAL;
    if (send(*sock, YAW_CMD,   CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void turnLeft(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += MIN_SIGNAL;
    if (send(*sock, YAW_CMD,   CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void straveRight(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += AVR_SIGNAL;
    if (send(*sock, ROLL_CMD,  CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void straveLeft(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += MIN_SIGNAL;
    if (send(*sock, ROLL_CMD,  CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveForward(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += AVR_SIGNAL;
    if (send(*sock, PITCH_CMD, CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveBackward(int *sock, int value)
{
    if(value > 100) value = 100;
    if(value < 0) value = 0;
    value *= 5;
    value += MIN_SIGNAL;
    if (send(*sock, PITCH_CMD, CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveStepForward(int *sock)
{
    int direction = INCR_SIGNAL;
    if (send(*sock, PITCH_CMD,      CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveStepBackward(int *sock)
{
    int direction = DECR_SIGNAL;
    if (send(*sock, PITCH_CMD,      CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void straveStepRight(int *sock)
{
    int direction = INCR_SIGNAL;
    if (send(*sock, ROLL_CMD,       CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void straveStepLeft(int *sock)
{
    int direction = DECR_SIGNAL;
    if (send(*sock, ROLL_CMD,       CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveStepUp(int *sock)
{
    int direction = INCR_SIGNAL;
    if (send(*sock, THROTTLE_CMD,   CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void moveStepDown(int *sock)
{
    int direction = DECR_SIGNAL;
    if (send(*sock, THROTTLE_CMD,   CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void turnStepRight(int *sock)
{
    int direction = INCR_SIGNAL;
    if (send(*sock, YAW_CMD,        CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void turnStepLeft(int *sock)
{
    int direction = DECR_SIGNAL;
    if (send(*sock, YAW_CMD,        CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &direction,  sizeof(int), 0) < 0) perror("writing on stream socket");
}

void freezeDeviceInAir(int *sock)
{
    int param = NO_PARAMETER;
    if (send(*sock, "frez",    CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &param, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void landDevice(int *sock)
{
    int param = NO_PARAMETER;
    if (send(*sock, "land",    CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &param, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void killDeviceServer(int *sock)
{
    int param = NO_PARAMETER;
    if (send(*sock, "kill",    CMD_SIZE, 0) < 0) perror("writing on stream socket");
    if (send(*sock, &param, sizeof(int), 0) < 0) perror("writing on stream socket");
}

void initConnection(int *sock, char *socketName, struct sockaddr_un *server)
{
    *sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*sock < 0)
    {
        perror("opening stream socket");
        exit(1);
    }
    server->sun_family = AF_UNIX;
    strcpy(server->sun_path, socketName);
    if (connect(*sock, (struct sockaddr *) server, sizeof(struct sockaddr_un)) < 0)
    {
        close(*sock);
        perror("connecting stream socket");
        exit(1);
    }
}

void deinitConnection(int *sock)
{
    close(*sock);
}

/*
All you need to send messages:

    int sock;
    struct sockaddr_un server;
    initConnection(&sock, NAME, &server); (NAME defined in header)
 
    DO SOME LOGIC HERE!
 
    deinitConnection(&sock);
 
*/


//Simple Client example:

//int main(void)
//{
//    int sock;
//    struct sockaddr_un server;
//    char cmd[5];
//    int value = 0;
//    /*
//    sock = socket(AF_UNIX, SOCK_STREAM, 0);
//    if (sock < 0) {
//        perror("opening stream socket");
//        exit(1);
//    }
//    server.sun_family = AF_UNIX;
//    strcpy(server.sun_path, NAME);
//    
//    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0)
//    {
//        close(sock);
//        perror("connecting stream socket");
//        exit(1);
//    }
//     */
//    initConnection(&sock, NAME, &server);
//    //LOGIC
//    while (1)
//    {
//        bzero(cmd, 5);
//        scanf("%s", cmd);
//        printf("Command: %s\n", cmd);
//        scanf("%i", &value);
//        printf("Value: %i\n", value);
//        if(send(sock, cmd, 5, 0) < 0) perror("writing on stream socket");
//        if(send(sock, &value, sizeof(int), 0) < 0) perror("writing on stream socket");
//        char resp;
//        int respVal;
//        //Receiving execution notification (not needed now)
//        /*
//        FILE *fp = fdopen (sock, "r");
//        resp = fgetc(fp);
//        if(resp == 1) printf("Success\n");
//        if(resp == 0) printf("Fail\n");
//         */
//        if(strcmp(cmd,"kill") == 0)
//        {
//            break;
//        }
//    }
//    
//    deinitConnection(&sock);
//    return 0;
//}