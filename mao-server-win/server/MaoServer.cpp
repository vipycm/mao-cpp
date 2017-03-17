//
// Created by mao on 2016/11/29.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "MaoServer.h"
#include <iostream>

int MaoServer::start() {
    struct sockaddr_in server;
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        log->e("creating  socket error");
        exit(1);
    }

    int opt = SO_REUSEADDR;
    setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(m_server_fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        log->e("bind error");
        exit(1);
    }
    if (listen(m_server_fd, BACKLOG) == -1) {
        log->e("listen error");
        exit(1);
    }

    log->i("server listening at port:%d, pid:%d", SERVER_PORT, getpid());

    while (true) {
        int client_fd;
        struct sockaddr_in client;
        socklen_t socklen = sizeof(client);
        if ((client_fd = accept(m_server_fd, (struct sockaddr *) &client, &socklen)) == -1) {
            log->e("accept error");
            break;
        }

        ServerWorker *worker = new ServerWorker(client_fd, &client);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_connect, worker);
    }
    log->i("server end");
    return 0;
}

void *handle_connect(void *worker) {
    ServerWorker *sw = (ServerWorker *) worker;
    sw->work();
    delete (sw);
}

ServerWorker::ServerWorker(int client_fd, sockaddr_in *client) {
    m_client_fd = client_fd;
    m_client = client;
    string tag = "ServerWorker-";
    tag.append(inet_ntoa(m_client->sin_addr));
    tag.append(":");
    tag.append(to_string(htons(m_client->sin_port)));
    log = MaoLog::getLogger(tag.c_str());
}

ServerWorker::~ServerWorker() {
    if (m_client_fd != -1) {
        close(m_client_fd);
    }
    if (log != NULL) {
        delete (log);
    }
}

void ServerWorker::work() {
    log->i("work start. pid:%d", getpid());

    char head[MSG_HEAD_LENGTH];
    memset(head, 0, MSG_HEAD_LENGTH);
    int len = 0;
    while (len < MSG_HEAD_LENGTH) {
        ssize_t read = recv(m_client_fd, head + len, MSG_HEAD_LENGTH - len, 0);
        len += read;
    }

    const int msgLen = parseMsgLength(head);
    if (msgLen < 0) {
        log->w("parseMsgLength error:%d", msgLen);
        return;
    }

    len = 0;
    char msg[msgLen];
    memset(msg, 0, msgLen);
    while (len < msgLen) {
        ssize_t read = recv(m_client_fd, msg + len, MSG_HEAD_LENGTH - len, 0);
        len += read;
    }

    log->d("recv msg: %s", msg);

    string resp;
    if (strcmp(msg, "exit") == 0) {
        //234d414f0400000065786974
        send(m_client_fd, msg, msgLen, 0);
        exit(0);
    } else if (strcmp(msg, "lock") == 0) {
        system("rundll32.exe user32.dll,LockWorkStation");
        resp = "lock success";
    }

    if (resp.empty()) {
        resp = "unsupported command: ";
        resp.append(msg);
    }
    log->i("response: %s", resp.c_str());

    send(m_client_fd, resp.c_str(), resp.length(), 0);
    log->i("work end. pid:%d", getpid());
}

/**
 * 解析协议头，获取消息正文的长度
 * @param head
 * @return
 */
int parseMsgLength(char *head) {
    if (head == NULL) {
        return -1;
    }

    for (int i = 0; i < MAO_HEAD_LENGTH; i++) {
        if (head[i] != MAO_HEAD[i]) {
            return -2;
        }
    }

    int msgLen = 0;
    for (int i = MAO_HEAD_LENGTH; i < MSG_HEAD_LENGTH; i++) {
        int x = head[i] & 0xff;
        msgLen += x << ((i - MAO_HEAD_LENGTH) * 8);
    }

    return msgLen;
}