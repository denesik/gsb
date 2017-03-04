#pragma once
#ifndef WORLD_LOG_H__
#define WORLD_LOG_H__

#include <boost/log/trivial.hpp>

#define __SHORT_FILE2__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __SHORT_FILE__ (strrchr(__SHORT_FILE2__, '\\') ? strrchr(__SHORT_FILE2__, '\\') + 1 : __SHORT_FILE2__)
#define LOG(level) BOOST_LOG_TRIVIAL(level) << __SHORT_FILE__ << ":" << __LINE__ << " "

void LogInit(bool consoleLogEnabled = true, bool fileLogEnabled = true);

#endif // WORLD_LOG_H__
