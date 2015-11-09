#pragma once

#include <sstream>
#include <mutex>

namespace constrained_based_networks
{
#ifdef DEBUG
#define DEBUG_CLASS_SOLUTION std::cerr
#define DEBUG_XML std::cerr
#else
#define DEBUG_CLASS_SOLUTION if(false) constrained_based_networks::Logger::getInstance()->get()
#define DEBUG_XML if(false) constrained_based_networks::Logger::getInstance()->get()
#endif
class Logger
{

   public:
    Logger() {};
    static Logger* getInstance()
    {
        pthread_mutex_lock(&mutex);
        if (!Logger::logger) {
            Logger::logger = new Logger();
        }
        pthread_mutex_lock(&mutex);
        return Logger::logger;
    }

    std::ostringstream& get()
    {
        os.clear();  // Make sure we not waisting memory
        return os;
    }

    std::ostringstream os;
    static Logger* logger;
    static pthread_mutex_t mutex;
};
}
