#include "gui_core.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GuiCore core;
    core.show_gui();
    return a.exec();
}
