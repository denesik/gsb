#include "Log.h"

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/format.hpp>

using boost::shared_ptr;

using namespace std;

void LogInit(bool consoleLogEnabled, bool fileLogEnabled)
{
  boost::log::add_console_log(
    std::cout,
    boost::log::keywords::format = (
      boost::log::expressions::stream
      << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
      << " "
      << boost::log::trivial::severity
      << " "
      << boost::log::expressions::message
      )
  );

  boost::log::add_file_log
  (
    boost::log::keywords::file_name = "world_%5N.log",
    boost::log::keywords::rotation_size = 10 * 1024 * 1024,
    boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    boost::log::keywords::format = (
      boost::log::expressions::stream
      << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
      << " "
      << boost::log::trivial::severity
      << " "
      << boost::log::expressions::message
      )
  );

  boost::log::core::get()->set_filter
  (
    boost::log::trivial::severity >= boost::log::trivial::trace
  );

  boost::log::add_common_attributes();
}