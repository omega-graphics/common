#include "omega-common/multithread.h"

namespace OmegaCommon {

    Semaphore::Semaphore(int initalValue){
        sem = CreateSemaphoreA(NULL,initalValue,initalValue,NULL);
    };
    void Semaphore::get(){
        BOOL block = TRUE;
        while(block) {
            DWORD signal = WaitForSingleObject(sem,0L);
            switch (signal) {
                case WAIT_OBJECT_0 : {
                    block = FALSE;
                    break;
                }
            }
        } 
    };
    void Semaphore::release(){
        assert(!ReleaseSemaphore(sem,1,NULL) && "Failed to Release Semaphore");
    }
    Semaphore::~Semaphore(){
        CloseHandle(sem);
    };
    
}