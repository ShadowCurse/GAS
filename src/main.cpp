#include <QApplication>

#include "gas_core.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GasCore core;
    core.show_gui();
    return a.exec();
}
