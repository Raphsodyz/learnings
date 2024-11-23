//source: https://www.youtube.com/watch?v=2HrYIl6GpYg
//to download the file: wget localhost:5000/index.html

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

int MAX_CONNECTION_QUEUE = 10;
int PORT = 5000;
char NULL_TERMINATOR = ' ';

void main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = { AF_INET, htons(PORT), 0 }; 

    bind(socket_fd, &address, sizeof(address));
    listen(socket_fd, MAX_CONNECTION_QUEUE);
    
    int client_fd = accept(socket_fd, 0, 0);
    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    //GET /file.html[NULL_TERMINATOR]...
    char* content = buffer + 5;
    *strchr(content, NULL_TERMINATOR) = 0;
    int opened_fd = open(content, O_RDONLY);
    sendfile(client_fd, opened_fd, 0, 47);

    close(opened_fd);
    close(client_fd);
    close(socket_fd);
}