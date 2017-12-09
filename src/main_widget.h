//
// Created by chudonghao on 17-11-28.
//

#ifndef UNTITLED_GAME_MAIN_WIDGET_H
#define UNTITLED_GAME_MAIN_WIDGET_H

#include "config.h"
#include <QtWidgets/QWidget>

namespace Ui{
    class main_widget;
}

namespace untitled_game{
    class main_widget_t : public QWidget {
        Q_OBJECT
    public:
        explicit main_widget_t(QWidget *parent = nullptr);
        ~main_widget_t() override;
    private:
        Ui::main_widget *ui;
    };
}


#endif //UNTITLED_GAME_MAIN_WIDGET_H
