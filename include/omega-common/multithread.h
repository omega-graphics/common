

#ifndef OMEGA_COMMON_MULTITHREAD_H
#define OMEGA_COMMON_MULTITHREAD_H

#include "utils.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#include <mutex>
#include <thread>

#include <cassert>

namespace OmegaCommon {
    typedef std::thread Thread;

    class Semaphore {
        #ifdef _WIN32
        HANDLE sem;
        #else 
        semaphore_t *sem;
        #endif
    public:
        Semaphore(int initialValue);
        void release();
        void get();
        ~Semaphore();
    };
};

#endif


