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
#include "Emulator.h"

Terminal::Terminal() : m_pty(nullptr), m_emulator(nullptr){
    resize(QSize(800, 600));

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


    m_pty = new Pty(
            getWidth(),
            getHeight(),
            "/bin/bash"
            );

    m_emulator = new Emulator(this);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTerminal()));
    timer->start(30);
}

Terminal::~Terminal() {
    delete m_pty;
}

void Terminal::updateTerminal() {
    QString str = "";
    if(m_pty->read(str) && str.length() > 0) {
        qDebug() << str;
        m_emulator->parse(str);
    }
}

void Terminal::keyPressEvent(QKeyEvent *e) {
    if(textCursor().columnNumber() + 1 >= getWidth()) {
        replaceCharWith('\n');
    }
    if(e->key() >= 0x20 && e->key() <= 0x0de) {
        m_pty->write(e->text());
    }

    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        m_pty->write("\n");
    }

    if(e->key() == Qt::Key_Backspace) {
        m_pty->write("\b");
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
    m_pty->resize(
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

void Terminal::moveCursorToBeginningOfLine() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::StartOfLine);
    setTextCursor(tc);
}

void Terminal::moveCursorToEndOfLine() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::EndOfLine);
    setTextCursor(tc);
}

void Terminal::moveCursorToEnd() {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::End);
    setTextCursor(tc);
}

void Terminal::replaceCharWith(QChar ch) {
    auto tc = textCursor();
    if(!textCursor().atEnd()) {
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

void Terminal::clearFromCursorToEnd() {
    int startPosition = textCursor().position();

    moveCursorToEnd();

    while(textCursor().position() != startPosition) {
        auto tc = textCursor();
        tc.deletePreviousChar();
        setTextCursor(tc);
    }

}

void Terminal::moveToLine(int line, bool moveAnchor) {
    auto tc = textCursor();
    tc.movePosition(QTextCursor::Start);
    tc.movePosition(QTextCursor::Down, moveAnchor ? QTextCursor::MoveAnchor : QTextCursor::KeepAnchor, line - 1);
    setTextCursor(tc);
}

void Terminal::deleteLine(int line) {
    moveToLine(line, true);
    auto tc = textCursor();
    tc.select(QTextCursor::SelectionType::LineUnderCursor);
    tc.removeSelectedText();
    tc.deleteChar();
    tc.deletePreviousChar();
    setTextCursor(tc);
}

void Terminal::clearFromCursorToBeginning(bool deleteNewlines) {
    int startCol = textCursor().columnNumber();
    int startRow = textCursor().blockNumber();

    int numLinesThatFillScreen = startRow < getHeight() ? startRow : getHeight();
    int targetRow = textCursor().blockNumber() - numLinesThatFillScreen;

    if(startRow < targetRow) {
        qDebug() << "wait a moment! Start row is smaller than target row! I\'m pretty sure that\'s a bug!";
    }


    auto tc = textCursor();
    qDebug() << "move down " << startRow - targetRow - 1<< " lines";
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    tc.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    setTextCursor(tc);
    tc.movePosition(QTextCursor::Up,QTextCursor::KeepAnchor, startRow - targetRow);
    tc.removeSelectedText();
    setTextCursor(tc);


    if(!deleteNewlines) {
        for(int i = 0; i < (startRow - targetRow); i++) {
            insertPlainText("\n");
        }
    }
}

void Terminal::clearAll() {
    clear();
}

unsigned short Terminal::getHeight() {
    return (int)viewport() -> height() / (int)fontMetrics().height();
}

unsigned short Terminal::getWidth() {
    return (window()->size().width()) / fontMetrics().averageCharWidth();
}