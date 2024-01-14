#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>

int main() {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        AF_INET, 
        htons(9999),
        0
    };


    if (bind(sockfd, &address, sizeof(address)) == -1) {
        printf("Bind failed. errno: %d\n", errno);
        return 1;
    }

    listen(sockfd, 10); // Listen for connections and set what is the maximum size of queue to listen for. 

    int clientfd = accept(sockfd, 0, 0); //Client file descripter. 


    //stdin - 0
    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0
        },
        {
            clientfd,
            POLLIN,
            0
        }
    };

    for (;;) {
        // Buffer for message.
        char buffer[256] = { 0 };

        if ( poll(fds, 2, 50000) == -1) {
            printf("Poll failed.");
        }

        // Send to the server.
        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(clientfd, buffer, 255, 0);
        }

        // If there is something to be read in from the client. 
        else if (fds[1].revents & POLLIN ) { 

            //If an empty message is sent, quit the client.
            if (recv(clientfd, buffer, 255, 0) == 0) {
                return 0; 
            }

            printf("%s\n", buffer);
        }
            
    }

    return 0;
}