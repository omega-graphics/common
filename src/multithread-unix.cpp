
#include "omega-common/multithread.h"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

namespace OmegaCommon {

#if defined(__APPLE__)

    Semaphore::Semaphore(int initialValue):sem(dispatch_semaphore_create(initialValue)) {

    }

    void Semaphore::get() {
        dispatch_semaphore_wait(sem,DISPATCH_TIME_FOREVER);
    }

    void Semaphore::release() {
        dispatch_semaphore_signal(sem);
    }

    Semaphore::~Semaphore() {
        dispatch_release(sem);
    }
#else

    Semaphore::Semaphore(int initialValue){
        sem_init(&sem,0,0);
    };

    void Semaphore::get(){
        sem_wait(&sem);
    }

    void Semaphore::release(){
        sem_post(&sem);
    }

    Semaphore::~Semaphore(){
        sem_close(&sem);
    }

#endif
}