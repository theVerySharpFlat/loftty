//
// Created by aiden on 3/12/22.
//

#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H

#include <QTextEdit>
#include "Pty.h"

class Terminal :  public QTextEdit{
    Q_OBJECT
public:
    explicit Terminal();
    ~Terminal();

    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
private slots:
     void updateTerminal();
private:

    Pty* pty;

};


#endif //TERMINAL_TERMINAL_H
