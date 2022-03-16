//
// Created by aiden on 3/12/22.
//

#include "Terminal.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <qnamespace.h>
#include <qsize.h>
#include <qtextcursor.h>

Terminal::Terminal() : pty(nullptr){
    setAutoFillBackground(true);

    resize(QSize(800, 600));

    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::black);
    pal.setColor(QPalette::Text, Qt::white);
    setPalette(pal);

    setFrameStyle(QFrame::NoFrame);

    //viewport()->setCursor(Qt::CursorShape::BlankCursor);
    setFontFamily("monaco");
    setFontPointSize(10);

    //setReadOnly(true);

    moveCursor(QTextCursor::End);

    setLineWrapMode(LineWrapMode::NoWrap);

    pty = new Pty(
            100,//(unsigned short)(textCursor().columnNumber() + 1) ,
            100, //(unsigned short)(textCursor().blockNumber() + 1),
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
        //qDebug() << str << "\n";
        //qDebug() << (int)str.toStdString()[0] << "\n";
        // for(auto ch : str) {
        //     qDebug() << (int)ch.toLatin1() << " ";
        // }
        // qDebug() << "\n";
        // if(str[0] == 0x08) {
        //     qDebug() << 0x08 << "\n";
        //     textCursor().deletePreviousChar();

        // } else if(str[0] == 0x07) {
        //     // bell
        // } else {
        //     qDebug() << str << (int)str.toStdString()[0] << "\n";
        //     insertPlainText(str);
        // }
        auto tc = textCursor();
        for(auto ch : str) {
            printf("ch: %c %d\n", ch.toLatin1(), (int)ch.toLatin1());
            if(ch == 127) {
                qDebug("delete!\n");
                
                tc.movePosition(QTextCursor::MoveOperation::Left);
                
                textCursor().deletePreviousChar();
            } else if(ch == 0x08) {
                printf("Left!\n");
                
                tc.movePosition(QTextCursor::MoveOperation::Left);
                setTextCursor(tc);
                continue;
            } else if(ch == '\a') {
                //do nothing
                continue;
            }
            //insertPlainText(ch);
            if(!textCursor().atEnd()) {
                //printf("not at end! insert %d", (int)ch.toLatin1());
                tc.movePosition(QTextCursor::MoveOperation::Right);
                tc.deletePreviousChar();
                tc.insertText(ch);
                setTextCursor(tc);
            } else {
                //printf("at end! insert %d", (int)ch.toLatin1());
                textCursor().insertText(ch);
                setTextCursor(tc);
            }
        }
        setTextCursor(tc);
    }
}

void Terminal::keyPressEvent(QKeyEvent *e) {
    if(e->key() >= 0x20 && e->key() <= 0x0de) {
        pty->write(e->text());
    }

    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        pty->write(QString("\n"));
    }

    if(e->key() == Qt::Key_Backspace) {
        pty->write("\b");
        //moveCursor(QTextCursor::MoveOperation::Left);
    }

    switch (e->key()) {
        case Qt::Key_Left:
            moveCursor(QTextCursor::MoveOperation::Left);
            break;
        case Qt::Key_Right:
            moveCursor(QTextCursor::MoveOperation::Right);
            break;
    }
}

void Terminal::mousePressEvent(QMouseEvent *e) {
    //disable mouse
}

void Terminal::mouseReleaseEvent(QMouseEvent *e) {
    // disable mouse
}
