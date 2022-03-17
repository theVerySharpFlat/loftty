//
// Created by aiden on 3/13/22.
//

#include "Pty.h"

#include <asm-generic/ioctls.h>
#include <cstdlib>
#include <qglobal.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <pty.h>
#include <fcntl.h>
#include <cstdio>
#include <iostream>
#include <poll.h>

Pty::Pty(unsigned short width, unsigned short height, const char* shell)
    : m_name(nullptr), m_master(-1), m_slave(-1){

    winsize winOps = {
        height,
        width,
        0, 0 // unused
    };
    openpty(&m_master, &m_slave, m_name, NULL, &winOps);

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork error!");
        exit(-1);
    } else if(pid == 0) { // child
        close(m_master);

        pid_t pgid = setsid();
        if(pgid == -1) {
            perror("setsid() error!");
            exit(-1);
        }

        if (ioctl(m_slave, TIOCSCTTY, NULL) == -1) {
            perror("ioctl()");
        }

        dup2(m_slave, STDIN_FILENO);
        dup2(m_slave, STDOUT_FILENO);
        dup2(m_slave, STDERR_FILENO);
        close(m_slave);

        const char* env[] = {
                "TERM=dumb",
                NULL
        };

        execle(shell, shell, (char*) NULL, env);
        exit(0);

    } else { // parent
        close(m_slave);
        // int flags = fcntl(m_master, F_GETFL, 0);
        // fcntl(m_master, flags | O_NONBLOCK);
    }
}

bool Pty::read(QString& str) {

    int bytesToRead;
    if(!::ioctl(m_master, TIOCINQ, &bytesToRead)) { // TIOCOUTQ finds the number of bytes in the buffer to read
        if(bytesToRead) {
            //printf("%d bytes\n", bytesToRead);
            str.resize(bytesToRead);
            char* data = (char*)alloca(bytesToRead + 1);
            ::read(m_master, data, bytesToRead);
            data[bytesToRead] = 0x00;
            
            str = QString::fromLocal8Bit(data);

            return true;
        }
        return false;
    } else {
        perror("ioctl() TIOCOUTQ!");
        return false;
    }

}

void Pty::write(const QString& data) {
    //printf("write! %d\n", data.toLocal8Bit().length());
    //printf("%s\n", data.toLocal8Bit().data());
    ::write(m_master, data.toLocal8Bit().data(), data.toLocal8Bit().length());
}

void Pty::resize(unsigned short width, unsigned short height) {
    //printf("(%hu, %hu)\n", height, width);
    struct winsize ws {
        height, width, 
        0, 0
    };
    ::ioctl(m_master, TIOCSWINSZ, &ws);

    struct winsize wsi;
    ::ioctl(m_master, TIOCGWINSZ, &wsi);
    //printf("(%hu, %hu) + (%hu, %hu)\n", ws.ws_col, ws.ws_row, ws.ws_xpixel, ws.ws_ypixel);
}

Pty::~Pty() {
    close(m_master);
}
