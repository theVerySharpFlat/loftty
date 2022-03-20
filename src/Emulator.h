//
// Created by aiden on 3/18/22.
//

#ifndef TERMINAL_EMULATOR_H
#define TERMINAL_EMULATOR_H

#include "Terminal.h"

class Emulator {
public:
    explicit Emulator(Terminal* terminal);
    void parse(const QString& str);

private:
    void parseCSI(const QStringRef& str);

    Terminal* m_terminal;
};


#endif //TERMINAL_EMULATOR_H
