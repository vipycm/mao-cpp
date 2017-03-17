//
// Created by mao on 2016/11/29.
//

#ifndef MAO_SERVER_WIN_MAOSERVER_H
#define MAO_SERVER_WIN_MAOSERVER_H

#include <unistd.h>
#include <netinet/in.h>

#include "../commons/MaoLog.h"

//服务器端口
#define SERVER_PORT 5263
#define BACKLOG 16

#define MAO_HEAD_LENGTH 4
#define MSG_HEAD_LENGTH 8

//@MAO的AscII码
static const unsigned char MAO_HEAD[] = {0x23, 0x4d, 0x41, 0x4f};

class ServerWorker;

class MaoServer {
private:
    MaoLog *log = MaoLog::getLogger("MaoServer");
    int m_server_fd = -1;


public:
    ~MaoServer() {
        log->i("~MaoServer");
        if (m_server_fd != -1) {
            close(m_server_fd);
        }
        delete (log);
    }

    int start();
};

class ServerWorker {
private:
    MaoLog *log = NULL;
    int m_client_fd;
    struct sockaddr_in *m_client;

public:
    ServerWorker(int client_fd, sockaddr_in *client);

    ~ServerWorker();

    /**
     * 处理客户端消息
     */
    void work();
};

void *handle_connect(void *worker);

int parseMsgLength(char *head);

#endif //MAO_SERVER_WIN_MAOSERVER_H
