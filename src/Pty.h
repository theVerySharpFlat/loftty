//
// Created by aiden on 3/13/22.
//

#ifndef TERMINAL_PTY_H
#define TERMINAL_PTY_H

#include <QString>

class Pty {
public:
    Pty(unsigned short width, unsigned short height, const char* shell);
    ~Pty();

    bool read(QString& str);
    void write(const QString& data);

private:
    int m_master;
    int m_slave;
    char* m_name;
};


#endif //TERMINAL_PTY_H
