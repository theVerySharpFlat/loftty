//
// Created by aiden on 3/18/22.
//

#include "Emulator.h"
#include <QDebug>
#include <cctype>

Emulator::Emulator(Terminal *terminal) : m_terminal(terminal) {

}

void Emulator::parse(const QString &str) {
    for (int i = 0; i < str.length(); i++) {
        QChar ch = str[i];
        switch (ch.toLatin1()) {
            case '\033': {// ESC
                qDebug() << "escape character!";
                if (str[i + 1] == '[') { // find the start and end of the csi sequence
                    int start = i + 2;
                    int end;
                    for(end = i + 2; end < str.length(); end++) {
                        if(std::isalpha(static_cast<unsigned char>(str[end].toLatin1()))) {
                            break;
                        }
                    }
                    QStringRef ref(&str, start, end - start + 1);
                    parseCSI(ref);
                    i = end;
                }
                break;
            }

            case '\b':
                m_terminal->moveCursorLeft();
                break;

            case '\a': // TODO: BEL
                break;

            case '\n':
                m_terminal->moveCursorDown();
                break;

            case '\r':
                m_terminal->moveCursorToBeginningOfLine();
                break;

            default: {
                if (m_terminal->textCursor().columnNumber() >= m_terminal->getWidth() - 1) {
                    m_terminal->replaceCharWith('\n');
                }
                m_terminal->replaceCharWith(ch);
            }

        }

    }

}

void Emulator::parseCSI(const QStringRef& str) {
    char csiCode = str[str.length() - 1].toLatin1();

    switch(csiCode) {
        case 'J': {
            bool success = true;
            int op = 0;
            if(str.length() != 1)
                op = str.chopped(1).toInt(&success);

            if(!success || !(op >= 0 && op <= 3)) {
                qDebug() << "invalid CSI n J opcode!";
                return;
            }

            switch(op) {
                case 0: // clear from cursor to end of screen
                    qDebug() << "case 0";
                    m_terminal->clearFromCursorToEnd();
                    break;
                case 1:
                    qDebug() << "case 1";
                    m_terminal->clearFromCursorToBeginning(false);
                    break;
                case 2:
                    qDebug() << "case 2";
                    m_terminal->moveCursorToEnd();
                    m_terminal->clearFromCursorToBeginning(true);
                    break;
                case 3:
                    qDebug() << "case 3";
                    m_terminal->clearAll();
                    break;


            }
        }

        default:
            qDebug() << "CSI " << str;
            break;
    }
}
