#ifndef MULTITHREADPROXY_LOGGER_H
#define MULTITHREADPROXY_LOGGER_H

#include <iostream>

class Logger {
private:
    const bool is_debug;
    std::ostream &ostream;
public:
    explicit Logger(bool isDebug, std::ostream &ostream = std::cerr);

    ~Logger() = default;

    void info(const std::string& tag, const std::string& message);

    void debug(const std::string &tag, const std::string &message);

    bool isDebug() const;
};


#endif //MULTITHREADPROXY_LOGGER_H
