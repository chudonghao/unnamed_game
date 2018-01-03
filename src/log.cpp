//
// Created by chudonghao on 17-11-29.
//
#include "config.h"
#include <iostream>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include "log.h"

using namespace std;
using namespace unnamed_game;
namespace logging = boost::log;

namespace unnamed_game {
    logging::sources::severity_logger <severity_level> severity_logger;
    // The formatting logic for the severity level
    template<typename CharT, typename TraitsT>
    static inline
    basic_ostream<CharT, TraitsT> &
    operator<<(
            basic_ostream<CharT, TraitsT> &strm,
            severity_level lvl) {
        static const char *const str[] =
                {
                        "normal",
                        "notification",
                        "warning",
                        "error",
                        "critical"
                };
        if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
            strm << str[lvl];
        else
            strm << static_cast< int >(lvl);
        return strm;
    }

    void init_logging() {
        namespace expr = logging::expressions;
        namespace attrs = logging::attributes;
        namespace keywords = logging::keywords;
        logging::add_console_log(
                std::cout,
                keywords::format = expr::stream
                        //<< expr::format_named_scope("Scope", keywords::format = "%f:%l") << " "
                        << "<" << expr::attr< severity_level >("Severity") << ">"
                        << expr::message
        );
        logging::add_common_attributes();
        //logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());

        string path = getenv("HOME");
        if (path.size() == 0) {
            path = getenv("USERPROFILE");
        }
        if (path.size() == 0) {
            char const *hdrive = getenv("HOMEDRIVE"),
                    *hpath = getenv("HOMEPATH");
            path = string(hdrive) + hpath;
        }
        if (path.size() == 0) {
            LOG_E << "unknown system,can NOT get user path.";
            throw runtime_error(nullptr);
        }
    }
}