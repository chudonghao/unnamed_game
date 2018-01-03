//
// Created by chudonghao on 17-11-28.
//
#include "config.h"
#include "main_widget.h"
#include "ui_main_widget.h"

namespace unnamed_game {

    main_widget_t::main_widget_t(QWidget *parent) : QWidget(parent),ui(new Ui::main_widget) {
        ui->setupUi(this);
    }

    main_widget_t::~main_widget_t() {

    }
}
