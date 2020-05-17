#ifndef UTILITY_H
#define UTILITY_H

#define LOG(msg) \
    std::cout << "\033[33m"; \
    std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl; \
    std::cout << "\033[0m"

#endif // UTILITY_H
