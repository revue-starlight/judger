#pragma once
#include <unistd.h>
#include <chrono>
#include "now.hpp"
typedef std::chrono::time_point<std::chrono::system_clock> stamp;
const stamp DEBUG_START_TIME = NOW::now();
#define TIMESTAMP  1.0*(NOW::now() - DEBUG_START_TIME).count() / 1000000
#define PRINT_TIMESTAMP { \
    printf("[%8.3f ms] [pid = %lu]",TIMESTAMP,(unsigned long)getpid()); \
}

#define INFO(...) {\
    PRINT_TIMESTAMP;    \
    printf("\033[0;37m INFO: \033[m");     \
    printf(__VA_ARGS__);   \
    printf("\n");   \
}


#define ERROR(...) {\
    PRINT_TIMESTAMP;    \
    printf("\033[0;32;31m ERROR: \033[m");     \
    printf(__VA_ARGS__);    \
    printf("\n");   \
}

#define SUCCESS(...){\
    PRINT_TIMESTAMP;\
    printf("\033[32m SUCCESS: \033[m");\
    printf(__VA_ARGS__);\
    printf("\n");\
}


#define WARNING(...){\
    PRINT_TIMESTAMP;\
    printf("\033[33m WARNING: \033[m");\
    printf(__VA_ARGS__);\
    printf("\n");\
}
