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
    double dolarTopeso;
    double dolarToeuro;
    double btc;
    double eth;
    char string[50];
    int flag =4;
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

        if(flag == 4){
            dolarTopeso = atof(buf);
            //printf("El valor de dolarTopeso: %lf\n",dolarTopeso);
        }

        if(flag == 3){
            dolarToeuro = atof(buf);
            //printf("El valor de dolarToeuro: %lf\n",dolarToeuro);

        }

        if(flag == 2){

            btc = atof(buf);
            printf("BTC en pesos: %lf\n",btc*dolarTopeso);
            sprintf(string,"%lf",btc*dolarTopeso);
            j =(int) sendto(sfd, string, strlen(string), 0, (struct sockaddr*) &claddr, len);
            printf("BTC en euro: %lf\n",btc*dolarToeuro);
            printf("BTC en dolar: %lf\n",btc);
        }
        if(flag == 1){
            eth = atoi(buf);
            printf("\n\n");
            printf("ETH en pesos: %lf\n",eth*dolarTopeso);
            printf("ETH en euro: %lf\n",eth*dolarToeuro);
            printf("ETH en dolar: %lf\n",eth);

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

}