//
// Created by aiden on 3/18/22.
//

#include "Emulator.h"
#include <QDebug>
#include <cctype>

static const unsigned int colors[] = {
        0x000000,
        0x800000,
        0x008000,
        0x808000,
        0x000080,
        0x800080,
        0x008080,
        0xc0c0c0,
        0x808080,
        0xff0000,
        0x00ff00,
        0xffff00,
        0x0000ff,
        0xff00ff,
        0x00ffff,
        0xffffff,
        0x000000,
        0x00005f,
        0x000087,
        0x0000af,
        0x0000d7,
        0x0000ff,
        0x005f00,
        0x005f5f,
        0x005f87,
        0x005faf,
        0x005fd7,
        0x005fff,
        0x008700,
        0x00875f,
        0x008787,
        0x0087af,
        0x0087d7,
        0x0087ff,
        0x00af00,
        0x00af5f,
        0x00af87,
        0x00afaf,
        0x00afd7,
        0x00afff,
        0x00d700,
        0x00d75f,
        0x00d787,
        0x00d7af,
        0x00d7d7,
        0x00d7ff,
        0x00ff00,
        0x00ff5f,
        0x00ff87,
        0x00ffaf,
        0x00ffd7,
        0x00ffff,
        0x5f0000,
        0x5f005f,
        0x5f0087,
        0x5f00af,
        0x5f00d7,
        0x5f00ff,
        0x5f5f00,
        0x5f5f5f,
        0x5f5f87,
        0x5f5faf,
        0x5f5fd7,
        0x5f5fff,
        0x5f8700,
        0x5f875f,
        0x5f8787,
        0x5f87af,
        0x5f87d7,
        0x5f87ff,
        0x5faf00,
        0x5faf5f,
        0x5faf87,
        0x5fafaf,
        0x5fafd7,
        0x5fafff,
        0x5fd700,
        0x5fd75f,
        0x5fd787,
        0x5fd7af,
        0x5fd7d7,
        0x5fd7ff,
        0x5fff00,
        0x5fff5f,
        0x5fff87,
        0x5fffaf,
        0x5fffd7,
        0x5fffff,
        0x870000,
        0x87005f,
        0x870087,
        0x8700af,
        0x8700d7,
        0x8700ff,
        0x875f00,
        0x875f5f,
        0x875f87,
        0x875faf,
        0x875fd7,
        0x875fff,
        0x878700,
        0x87875f,
        0x878787,
        0x8787af,
        0x8787d7,
        0x8787ff,
        0x87af00,
        0x87af5f,
        0x87af87,
        0x87afaf,
        0x87afd7,
        0x87afff,
        0x87d700,
        0x87d75f,
        0x87d787,
        0x87d7af,
        0x87d7d7,
        0x87d7ff,
        0x87ff00,
        0x87ff5f,
        0x87ff87,
        0x87ffaf,
        0x87ffd7,
        0x87ffff,
        0xaf0000,
        0xaf005f,
        0xaf0087,
        0xaf00af,
        0xaf00d7,
        0xaf00ff,
        0xaf5f00,
        0xaf5f5f,
        0xaf5f87,
        0xaf5faf,
        0xaf5fd7,
        0xaf5fff,
        0xaf8700,
        0xaf875f,
        0xaf8787,
        0xaf87af,
        0xaf87d7,
        0xaf87ff,
        0xafaf00,
        0xafaf5f,
        0xafaf87,
        0xafafaf,
        0xafafd7,
        0xafafff,
        0xafd700,
        0xafd75f,
        0xafd787,
        0xafd7af,
        0xafd7d7,
        0xafd7ff,
        0xafff00,
        0xafff5f,
        0xafff87,
        0xafffaf,
        0xafffd7,
        0xafffff,
        0xd70000,
        0xd7005f,
        0xd70087,
        0xd700af,
        0xd700d7,
        0xd700ff,
        0xd75f00,
        0xd75f5f,
        0xd75f87,
        0xd75faf,
        0xd75fd7,
        0xd75fff,
        0xd78700,
        0xd7875f,
        0xd78787,
        0xd787af,
        0xd787d7,
        0xd787ff,
        0xd7af00,
        0xd7af5f,
        0xd7af87,
        0xd7afaf,
        0xd7afd7,
        0xd7afff,
        0xd7d700,
        0xd7d75f,
        0xd7d787,
        0xd7d7af,
        0xd7d7d7,
        0xd7d7ff,
        0xd7ff00,
        0xd7ff5f,
        0xd7ff87,
        0xd7ffaf,
        0xd7ffd7,
        0xd7ffff,
        0xff0000,
        0xff005f,
        0xff0087,
        0xff00af,
        0xff00d7,
        0xff00ff,
        0xff5f00,
        0xff5f5f,
        0xff5f87,
        0xff5faf,
        0xff5fd7,
        0xff5fff,
        0xff8700,
        0xff875f,
        0xff8787,
        0xff87af,
        0xff87d7,
        0xff87ff,
        0xffaf00,
        0xffaf5f,
        0xffaf87,
        0xffafaf,
        0xffafd7,
        0xffafff,
        0xffd700,
        0xffd75f,
        0xffd787,
        0xffd7af,
        0xffd7d7,
        0xffd7ff,
        0xffff00,
        0xffff5f,
        0xffff87,
        0xffffaf,
        0xffffd7,
        0xffffff,
        0x080808,
        0x121212,
        0x1c1c1c,
        0x262626,
        0x303030,
        0x3a3a3a,
        0x444444,
        0x4e4e4e,
        0x585858,
        0x606060,
        0x666666,
        0x767676,
        0x808080,
        0x8a8a8a,
        0x949494,
        0x9e9e9e,
        0xa8a8a8,
        0xb2b2b2,
        0xbcbcbc,
        0xc6c6c6,
        0xd0d0d0,
        0xdadada,
        0xe4e4e4,
        0xeeeeee
};

Emulator::Emulator(Terminal *terminal) : m_terminal(terminal) {}

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

        case 'm': // SGR (COLORS!!! and text formatting)
            parseSGR(str);
            break;

        default:
            qDebug() << "CSI " << str;
            break;
    }
}

void Emulator::parseSGR(const QStringRef &str) {
    qDebug() << "SGR " << str;
    int start = 0;
    if(!std::isdigit(str[start].toLatin1())) {
        qDebug() << "First digit of SGR opcode is not a number!";
        return;
    }

    // find opcode
    int end;
    for(end = 0; end < str.length(); end++) {
        if(!std::isdigit(str[end].toLatin1()))
            break;
    }
    bool success;
    int op = str.left(end).toInt(&success);
    if(!success) {
        qDebug() << "failed to parse opcode for some reason. Bug???";
        return;
    }

    if((op >= 30 && op <= 39) || (op >= 40 && op <= 49)) {
        parseColorOp(str.right(str.size() - end), op);
    }
}

void Emulator::parseColorOp(const QStringRef &str, int op) {
    unsigned int& targetColorField =
            (op >= 30 && op <= 39) ? m_terminal->getTextFormat().fg : m_terminal->getTextFormat().bg;

    int colorCode = op % 10;
    qDebug() << "ColorCode: " << colorCode;
    if(colorCode < 8) {
        targetColorField = colors[colorCode];
    } else if(colorCode == 9) {
        targetColorField =
                (op >= 30 && op <= 39) ? Terminal::defaultTextFormat.fg : Terminal::defaultTextFormat.bg;
    } else if(colorCode == 8) {
        qDebug() << "We currently do not support non 3 bit colors!!!";
    }
}
