#include "config.h"
#include <QDebug>
#include <QtWidgets/QApplication>

#include <boost/program_options.hpp>

#include "main_widget.h"
#include "log.h"

using namespace unnamed_game;
using namespace boost;
using namespace std;

namespace unnamed_game {

bool show_basic_floor = false;
string show_dae_file;
}

int main(int argc, char *argv[]) {
    //
    QApplication application(argc, argv);
    main_widget_t main_widget;
    //
    init_logging();
    //
    program_options::options_description od("Allowed options");
    od.add_options()
            ("help", "show help")
            ("show_basic_floor", program_options::value<bool>(&show_basic_floor)->default_value(false),
             "show basic floor")
            ("show_dae_file", program_options::value<string>(&show_dae_file)->default_value(""), "");
    program_options::variables_map vm;
    program_options::store(program_options::parse_command_line(argc, reinterpret_cast<const char *const *>(argv), od),
                           vm);
    program_options::notify(vm);
    if (vm.count("help")) {
        LOG_N << od;
    }

    main_widget.show();
    return application.exec();
}
