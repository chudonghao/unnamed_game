//
// Created by chudonghao on 17-11-29.
//

#ifndef UNNAMED_GAME_LOG_H
#define UNNAMED_GAME_LOG_H

#include <boost/log/common.hpp>

namespace unnamed_game {
enum severity_level {
    normal,
    notification,
    warning,
    error,
    critical
};
extern boost::log::sources::severity_logger<severity_level> severity_logger;

void init_logging();
}

//#define LOG_FUNC BOOST_LOG_FUNCTION()
#ifdef LOG_FILE_LINE
#define LOG_N BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::normal) << __FILE__":" << __LINE__ << " "
#define LOG_W BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::warning) << __FILE__":" << __LINE__ << " "
#define LOG_E BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::error) << __FILE__":" << __LINE__ << " "
#else
#define LOG_N BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::normal)
#define LOG_W BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::warning)
#define LOG_E BOOST_LOG_SEV(unnamed_game::severity_logger,unnamed_game::error)
#endif
#endif //UNNAMED_GAME_LOG_H
