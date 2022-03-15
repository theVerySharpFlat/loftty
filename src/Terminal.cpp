//
// Created by aiden on 3/12/22.
//

#include "Terminal.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <qnamespace.h>
#include <qsize.h>

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
        qDebug() << (int)str.toStdString()[0] << "\n";
        if(str[0] == 0x08) {
            textCursor().deletePreviousChar();

        } else if(str[0] == 0x07) {
            // bell
        } else {
            insertPlainText(str);
        }
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
