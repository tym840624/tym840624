
/*
当write函数返回EPIPE或ETIMEDOUT错误码时，关闭连接并清理相应的资源，并向客户端发送一条错误信息

当write函数返回EPIPE或ETIMEDOUT错误码时，关闭连接并清理相应的资源，并向客户端发送一条错误信息。
同时，当read函数返回0时，也需要关闭连接并清理相应的资源。在实际应用中，可以根据具体的需求进行相应的修改。

系统调用write会返回-1的情况有很多，常见的有以下几种：
网络错误：例如网络中断、连接超时等，这时write函数会返回-1，并设置errno为EPIPE或ETIMEDOUT等错误码。
资源不足：例如内存不足、文件描述符不足等，这时write函数会返回-1，并设置errno为ENOMEM或EMFILE等错误码。
中断：当write函数被信号中断时，会返回-1，并设置errno为EINTR。
当write函数返回-1时，需要根据errno的值来判断具体的错误类型，并采取相应的措施。例如：
如果errno为EPIPE或ETIMEDOUT等网络错误码，说明网络连接出现了问题，可以尝试重新连接或者关闭连接。
如果errno为ENOMEM或EMFILE等资源不足的错误码，说明系统资源不足，可以尝试释放一些资源或者增加系统资源限制。
如果errno为EINTR，说明write函数被信号中断，可以忽略该错误，重新调用write函数。
需要注意的是，当write函数返回-1时，并不一定意味着连接已经断开。因此，在处理write函数返回-1时，
需要根据具体情况来决定是否关闭连接。如果是网络错误或者资源不足等无法恢复的错误，可以考虑关闭连接；如果是中断等可恢复的错误，可以尝试重新调用write函数。


为什么说read conn_fd返回为0时，认为客户端己断开连接？
在TCP协议中，当一端发送FIN报文给另一端时，表示它将不再发送数据，但仍然可以接收数据。接收到FIN报文的一端会发送ACK报文给发送端，
表示已经收到了FIN报文，并且也不会再发送数据。当发送端收到ACK报文后，就知道对方已经不再发送数据了，但仍然可以接收数据。接收端在收到最后的数据后，
也会发送FIN报文给发送端，表示自己也将不再发送数据。发送端收到FIN报文后，也会发送ACK报文给接收端，表示已经收到了FIN报文。当双方都收到对方的FIN报文并且发送了ACK报文后，连接就被关闭了。

在C++中，使用read函数从socket中读取数据时，如果返回值为0，表示对端已经关闭了连接。这是因为当对端发送了FIN报文后，如果接收端没有收到最后的数据，
那么read函数会一直阻塞等待数据的到来。但是，当对端发送了FIN报文后，接收端会收到一个EOF信号，此时read函数会返回0，表示对端已经关闭了连接。

因此，在服务器端中，当read函数返回0时，可以认为客户端已经断开连接了。此时，服务器端可以关闭连接，并清理相应的资源。

如果客户端一直与服务端保持连接但没有发送任何数据，那么服务端调用read函数时会一直阻塞，直到有数据到来或者连接断开才会返回。
如果客户端在这段时间内主动断开连接，那么服务端的read函数会返回0，表示连接已经断开。如果客户端一直不主动断开连接，
服务端的read函数将会一直阻塞，直到有数据到来或者连接超时。如果服务端设置了超时时间，那么当超过超时时间时，read函数会返回-1，表示读取失败。
此时，服务端可以关闭连接，并清理相应的资源。

需要注意的是，如果客户端一直与服务端保持连接但没有发送任何数据，这可能会导致服务端资源的浪费。因此，可以在服务端设置一个超时时间，
如果超过一定时间客户端没有发送任何数据，那么服务端可以主动断开连接，以释放资源。
*/
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "bind error: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 10) < 0) {
        std::cerr << "listen error: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    while (true) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            std::cerr << "accept error: " << strerror(errno) << std::endl;
            continue;
        }

        char buf[1024] = "Welcome to my server!";
        if (write(connfd, buf, strlen(buf)) < 0) {
            std::cerr << "write error: " << strerror(errno) << std::endl;
            close(connfd);
            continue;
        }

        while (true) {
            memset(buf, 0, sizeof(buf));
            int n = read(connfd, buf, sizeof(buf));
            if (n < 0) {
                std::cerr << "read error: " << strerror(errno) << std::endl;
                close(connfd);
                break;
            } else if (n == 0) {
                std::cout << "client closed the connection." << std::endl;
                close(connfd);
                break;
            } else {
                std::cout << "received message from client: " << buf << std::endl;
                if (write(connfd, buf, strlen(buf)) < 0) {
                    if (errno == EPIPE || errno == ETIMEDOUT) {
                        std::cerr << "write error: " << strerror(errno) << std::endl;
                        close(connfd);
                        break;
                    } else {
                        std::cerr << "write error: " << strerror(errno) << std::endl;
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}