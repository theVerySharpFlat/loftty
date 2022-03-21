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

    struct TextFormat {
        unsigned int fg;
        unsigned int bg;
    };
    inline TextFormat& getTextFormat() { return m_textFormat; }

    void replaceCharWith(QChar ch);

    unsigned short getWidth();
    unsigned short getHeight();
private slots:
     void updateTerminal();
private:
    Emulator* m_emulator;
    Pty* m_pty;

    constexpr static const TextFormat defaultTextFormat {
        0xffffff,
        0x000000
    };

    TextFormat m_textFormat{
        defaultTextFormat.fg,
        defaultTextFormat.bg
    };
};


#endif //TERMINAL_TERMINAL_H
