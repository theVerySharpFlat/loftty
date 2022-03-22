//
// Created by aiden on 3/12/22.
//

#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H

#include <QTextEdit>
#include <qevent.h>
#include "Pty.h"
#include <qdebug.h>

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

    enum TextFormatOp {
        Bold = 0,
        Faint,
        Italic,
        Underlined,
        Strikethrough,
        InvertedColors,
    };

    struct TextFormat {
        unsigned int fg;
        unsigned int bg;
        unsigned int format;
    };
    inline TextFormat& getTextFormat() { return m_textFormat; }
    inline void setTextOp(TextFormatOp op, bool val) {
        if(val) {
            m_textFormat.format |= 1 << (int) op;
        } else {
            m_textFormat.format &= ~(1 << (int) op);
        }
        qDebug() << "FORMAT: " << bin << m_textFormat.format;
    }
    inline bool getTextOp(TextFormatOp op) const {
        //qDebug() << "RET: " << bin << (m_textFormat.format & (1 << (int) op));
        return (bool)(m_textFormat.format & (1 << (int) op));
    }

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
        0x000000,
        0x00000000
    };

    TextFormat m_textFormat{
        defaultTextFormat.fg,
        defaultTextFormat.bg
    };
};


#endif //TERMINAL_TERMINAL_H
