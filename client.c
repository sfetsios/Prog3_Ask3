
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include "function.h"
#define PORT 8080

int main() {
    //Theloume na ftiaksoume 5 clients
    int n_clients = 5; 
     srand(time(NULL)); 

    //epanalipsi gia kathe client
    for (int i = 0; i < n_clients; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
            //Thigatriki diergasia
            int sock = 0;
            struct sockaddr_in serv_addr;

            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("Sfalma sth Dimiourgia Socket \n");
                return -1;
            }

            // Arxikopoiisi tou port mas
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);

            // An to address den einai egkyro
            if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
                printf("To address auto den uparxei. \n");
                return -1;
            }
            //Dimiourgia sundesis
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                printf("Sundesi apetuxe \n");
                return -1;
            }
            // Exoume kanei to socket,exoume elexgei to address kai exoume kanei sundesi
            // Wra gia tis paraggelies
            placeOrder(sock); 
            printf("Pelatis %d estile 10 paraggelies epituxws.\n", getpid());
            close(sock);
            exit(0);
        }
    }

    while (n_clients > 0) {
        // Anamonh gia to termatismo kathe diergasias 
        wait(NULL); 
        n_clients--;
    }

    printf("Telos programmatos Pelatwn! Ola pigan kala!");
    return 0;
}
