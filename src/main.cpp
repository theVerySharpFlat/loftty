#include "Terminal.h"
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Terminal terminal;
    terminal.show();
    return QApplication::exec();
}
