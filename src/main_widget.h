//
// Created by chudonghao on 17-11-28.
//

#ifndef UNNAMED_GAME_MAIN_WIDGET_H
#define UNNAMED_GAME_MAIN_WIDGET_H

#include <QtWidgets/QWidget>

namespace Ui{
    class main_widget;
}

namespace unnamed_game {
    class main_widget_t : public QWidget {
        Q_OBJECT
    public:
        explicit main_widget_t(QWidget *parent = nullptr);
        ~main_widget_t() override;
    private:
        Ui::main_widget *ui;
    };
}


#endif //UNNAMED_GAME_MAIN_WIDGET_H
