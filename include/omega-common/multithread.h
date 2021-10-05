

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
#include <future>

#include <cassert>

namespace OmegaCommon {
    typedef std::thread Thread;
    typedef std::mutex Mutex;

    template<class T>
    using Async = std::future<T>;

    template<class T>
    class Promise {
        std::promise<T> _prom;
    public:
        Async<T> async(){
            return _prom.get_future();
        };
        void set(const T & val){
            _prom.set_value(val);
        }
        void set(T && val){
            _prom.set_value(val);
        }
    };

    class OMEGACOMMON_EXPORT Semaphore {
        #ifdef _WIN32
        HANDLE sem;
        #elif defined(__APPLE__)
        dispatch_semaphore_t sem;
        #else
        sem_t sem;
        #endif
    public:
        explicit Semaphore(int initialValue);
        void release();
        void get();
        ~Semaphore();
    };

    /// @brief A unidirectional transport bridge.
    /// @paragraph
    /// The pipe in this implementation is represented a one-way bridge between Point A and Point B.
    class OMEGACOMMON_EXPORT Pipe {
        bool sideA;
        friend class ChildProcess;
#ifdef _WIN32
        HANDLE h;
        HANDLE file_a,file_b;
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

    /// @brief A Subprocess of the current process.
    class OMEGACOMMON_EXPORT ChildProcess {
#ifdef _WIN32
        bool off = false;
        PROCESS_INFORMATION processInformation;
        STARTUPINFO startupinfo;
        Pipe pipe;
        bool use_pipe;
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

};

#endif


