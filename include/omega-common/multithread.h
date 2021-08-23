

#ifndef OMEGA_COMMON_MULTITHREAD_H
#define OMEGA_COMMON_MULTITHREAD_H

#include "utils.h"
#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)

#include <dispatch/dispatch.h>

#else
#include <semaphore.h>
#endif
#include <mutex>
#include <thread>

#include <cassert>

namespace OmegaCommon {
    typedef std::thread Thread;

    class OMEGACOMMON_EXPORT Semaphore {
        #ifdef _WIN32
        HANDLE sem;
        #elif defined(__APPLE__)
        dispatch_semaphore_t sem;
        #else
        sem_t * sem;
        #endif
    public:
        Semaphore(int initialValue);
        void release();
        void get();
        ~Semaphore();
    };
};

#endif


