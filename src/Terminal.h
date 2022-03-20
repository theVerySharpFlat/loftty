//
// Created by aiden on 3/12/22.
//

#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H

#include <QTextEdit>
#include <qevent.h>
#include "Pty.h"

class Emulator;

class Terminal :  public QTextEdit{
    Q_OBJECT
public:
    friend class Emulator;

    explicit Terminal();
    ~Terminal();

    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorDown();
    void moveCursorToBeginningOfLine();
    void moveCursorToEndOfLine();
    void moveCursorToEnd();
    void moveToLine(int line, bool moveAnchor);

    void deleteLine(int line);

    void clearFromCursorToEnd();
    void clearFromCursorToBeginning(bool deleteNewlines = false);
    void clearAll();

    void replaceCharWith(QChar ch);

    unsigned short getWidth();
    unsigned short getHeight();
private slots:
     void updateTerminal();
private:
    Emulator* m_emulator;
    Pty* m_pty;

};


#endif //TERMINAL_TERMINAL_H
