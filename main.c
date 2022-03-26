#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 100
#define SV_SOCK_PATH "socket"

void *pythonExecute(){

    //sleep(1);
    system("python3 request.py");

    return NULL;
}

int main()
{
    pthread_t hilo;
    double btc;
    double eth;
    int flag =2;
    pthread_create(&hilo, NULL, &pythonExecute, NULL);
    //pthread_join(hilo,NULL);

    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sfd == -1)
    {
        fprintf(stderr, "error opening socket");
        return 1;
    }

    if(remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    {
        fprintf(stderr, "error removing socket %d", errno);
        return 2;
    }

    memset(&svaddr,0,sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path)-1);

    if(bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_un)) == -1)
    {
        fprintf(stderr, "error binding to socket");
        return 3;
    }

    for(;;)
    {
        memset( buf, 0, BUF_SIZE );
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr*) &claddr, &len);

        if(numBytes == -1)
        {
            fprintf(stderr, "error recvfrom");
            return 4;
        }

        //fprintf(stdout, "server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

        if(flag == 2){
            printf("Valor de buf: %s\n",buf);
            btc = atof(buf);

        }
        if(flag == 1){
            printf("Valor de buf: %s\n",buf);
            eth = atoi(buf);
        }

        flag--;

        for(j=0; j<numBytes; j++){
            buf[j] = (char) toupper((unsigned char) buf[j]);
        }
        j =(int) sendto(sfd, buf,(size_t) numBytes, 0, (struct sockaddr*) &claddr, len);

        if(flag == 0){
            break;
        }
        if(j != numBytes){
            fprintf(stderr, strerror(errno), "error");
        }
    }

    printf("El valor de BTC: %lf\n",btc);
    printf("El valor de ETH: %lf\n",eth);

}