#pragma once

#include <sstream>

namespace constrained_based_networks
{
#ifdef DEBUG
#define DEBUG_CLASS_SOLUTION std::cerr
#else
#define DEBUG_CLASS_SOLUTION constrained_based_networks::Logger::getInstance()->get()
#endif
class Logger
{
   public:
    Logger() {};
    static Logger* getInstance()
    {
        if (!Logger::logger) {
            Logger::logger = new Logger();
        }
        return Logger::logger;
    }

    std::ostringstream& get()
    {
        os.clear();  // Make sure we not waisting memory
        return os;
    }

    std::ostringstream os;
    static Logger* logger;
};
}
