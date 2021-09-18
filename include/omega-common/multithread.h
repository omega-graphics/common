

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
        sem_t sem;
        #endif
    public:
        Semaphore(int initialValue);
        void release();
        void get();
        ~Semaphore();
    };

    class ChildProcess {
#ifdef _WIN32
        HANDLE handle;
#else
        FILE *p_file = nullptr;
        bool use_pipe;
        pid_t pid;
#endif
    public:
        static ChildProcess OpenWithStdoutPipe(const OmegaCommon::String & cmd,const char * args);
        static ChildProcess Open(const OmegaCommon::String & cmd,const OmegaCommon::Vector<const char *> & args);
        int wait();
        ~ChildProcess();
    };

    /// @brief A unidirectional transport bridge.
    /// @paragraph
    /// The pipe in this implementation is represented a one-way bridge between Point A and Point B.
    class Pipe {
        bool sideA;
#ifdef _WIN32
#else
        int pipe_fd[2];
#endif
        Pipe();
        /// @brief Sets the Current Process As Point A
        void setCurrentProcessAsA();
        /// @brief Sets the Current Process As Point B
        void setCurrentProcessAsB();

        size_t readA(char *buffer,size_t n_read);
        void writeA(char *buffer,size_t n_write);

        size_t readB(char *buffer,size_t n_read);
        void writeB(char *buffer,size_t n_write);

        ~Pipe();
    };
};

#endif


