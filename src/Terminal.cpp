//
// Created by aiden on 3/12/22.
//

#include "Terminal.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <qevent.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qnamespace.h>
#include <qsize.h>
#include <qtextcursor.h>
#include <QScrollBar>

Terminal::Terminal() : pty(nullptr){
    resize(QSize(400, 300));

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::black);
    pal.setColor(QPalette::Text, Qt::white);
    setPalette(pal);

    setFrameStyle(QFrame::NoFrame);

    auto font = QFont("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    setFont(font);
    setFontPointSize(10);

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalScrollBar()->setEnabled(false);

    setReadOnly(true);

    moveCursor(QTextCursor::End);

    setLineWrapMode(LineWrapMode::NoWrap);


    pty = new Pty(
            getWidth(),
            getHeight(),
            "/bin/bash"
            );

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTerminal()));
    timer->start(30);
}

Terminal::~Terminal() {

}

void Terminal::updateTerminal() {
    QString str = "";
    if(pty->read(str) && str.length() > 0) {
        for(auto ch : str) {
            if(ch == 0x08) {
                moveCursorLeft();
                continue;

            } else if(ch == '\a') {
                //do nothing
                continue;
            } else if(ch == '\n') {
                moveCursorDown();
                continue;
                //printf("newline\n");
            } else if(ch == '\r') {
                moveCursorToBeginning();
                continue;
            }
            if(textCursor().columnNumber() >= getWidth()-1) {
                replaceCharWith('\n');
            }
            replaceCharWith(ch);
        }
    }
}

void Terminal::keyPressEvent(QKeyEvent *e) {
    if(textCursor().columnNumber() + 1 >= getWidth()) {
        replaceCharWith('\n');
    }
    if(e->key() >= 0x20 && e->key() <= 0x0de) {
        pty->write(e->text());
    }

    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        pty->write("\n");
    }

    if(e->key() == Qt::Key_Backspace) {
        pty->write("\b");
    }

    switch (e->key()) {
        case Qt::Key_Left:
            moveCursor(QTextCursor::MoveOperation::Left);
            break;
        case Qt::Key_Right:
            moveCursor(QTextCursor::MoveOperation::Right);
            break;
        case Qt::Key_Down:
            moveCursorDown();
            break;
    }
}

void Terminal::resizeEvent(QResizeEvent* e) {
    pty->resize(
        getWidth(), 
        getHeight()
    );
}

void Terminal::mousePressEvent(QMouseEvent *e) {
    //disable mouse
}

void Terminal::mouseReleaseEvent(QMouseEvent *e) {
    // disable mouse
}

void Terminal::moveCursorLeft() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::Left);
    setTextCursor(tc);
}

void Terminal::moveCursorRight() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::Right);
    setTextCursor(tc);
}

void Terminal::moveCursorDown() {
    auto tc = textCursor();
    int blockNum = tc.blockNumber();
    tc.movePosition(QTextCursor::MoveOperation::Down);
    setTextCursor(tc);

    if(tc.blockNumber() == blockNum) {
        tc.movePosition(QTextCursor::MoveOperation::EndOfLine);
        tc.insertText(QChar('\n'));
        setTextCursor(tc);
    }
}

void Terminal::moveCursorToBeginning() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::StartOfBlock);
    setTextCursor(tc);
}

void Terminal::replaceCharWith(QChar ch) {
    auto tc = textCursor();
    if(!textCursor().atEnd()) {
        //printf("not at end!\n");
        tc.movePosition(QTextCursor::MoveOperation::Right);
        setTextCursor(tc);
        tc.deletePreviousChar();
        tc.insertText(ch);
        setTextCursor(tc);
    } else {
        //printf("at end!\n");
        tc.insertText(ch); 
        setTextCursor(tc);
    }
}

unsigned short Terminal::getHeight() {
    return (int)viewport() -> height() / (int)fontMetrics().height();
}

unsigned short Terminal::getWidth() {
    //printf("average char width: %d\n", fontMetrics().averageCharWidth());
    return (window()->size().width()) / fontMetrics().averageCharWidth();
    //return 47;
}