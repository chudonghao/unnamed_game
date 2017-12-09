#include <QDebug>
#include <QtWidgets/QApplication>
#include "main_widget.h"
#include "log.h"
using namespace untitled_game;

int main(int argc, char *argv[]) {
    init_logging();
    QApplication application(argc, argv);
    main_widget_t main_widget;
    main_widget.show();
    return application.exec();
}