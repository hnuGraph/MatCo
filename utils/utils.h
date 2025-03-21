#ifndef UTILS_UTILS
#define UTILS_UTILS

#include <algorithm>
#include <future>
#include <iostream>
#include <vector>
#include <string.h>
#include <sys/stat.h> /* For stat() */
#include "utils/types.h"

namespace io {
    inline size_t file_exists(const char *path) {
        struct stat st;
        return stat(path, &st) == 0;
    }
}

inline void execute_with_time_limit(std::function<void()> fun, uint time_limit, std::atomic<bool>& reach_time_limit)
{
    reach_time_limit = false;
    std::future<void> future = std::async(std::launch::async, fun);
    std::future_status status;
    do {
        status = future.wait_for(std::chrono::seconds(time_limit));
        if (status == std::future_status::deferred)
        {
            std::cout << "Deferred" << std::endl;
            exit(-1);
        }
        else if (status == std::future_status::timeout)
        {
            reach_time_limit = true;
            std::cout << "Timeout " << time_limit << "s\n";
        }
    } while (status != std::future_status::ready);
}

namespace mem {
    inline int parseLine(char* line){
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char* p = line;
        while (*p <'0' || *p > '9') p++;
        line[i-3] = '\0';
        i = atoi(p);
        return i;
    }

    inline int getValue(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmPeak:", 7) == 0){
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    inline int getMemUsage(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmHWM:", 6) == 0) { 
                result = parseLine(line);  // RSS
            }
        }
        fclose(file);
        return result;
    }
}

#endif //UTILS_UTILS
