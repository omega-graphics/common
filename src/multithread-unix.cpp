
#include "omega-common/multithread.h"

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

#endif
}