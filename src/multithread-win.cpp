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
        assert(!ReleaseSemaphore(sem,1,nullptr) && "Failed to Release Semaphore");
    }
    Semaphore::~Semaphore(){
        CloseHandle(sem);
    };

    ChildProcess ChildProcess::Open(const OmegaCommon::String &cmd, const OmegaCommon::Vector<const char *> &args) {
        STARTUPINFO startupinfo;
        ZeroMemory(&startupinfo,sizeof(STARTUPINFO));
        startupinfo.cb = sizeof(STARTUPINFO);

        ChildProcess process{};
        ZeroMemory(&process.processInformation,sizeof(process.processInformation));

        CreateProcessA(NULL,(LPSTR)cmd.data(),NULL,NULL,FALSE,0,NULL,NULL,&startupinfo,&process.processInformation);

        return process;
    }

    ChildProcess ChildProcess::OpenWithStdoutPipe(const OmegaCommon::String &cmd, const char *args) {
        STARTUPINFO startupinfo;
        ZeroMemory(&startupinfo,sizeof(STARTUPINFO));
        startupinfo.cb = sizeof(STARTUPINFO);

        ChildProcess process{};
        ZeroMemory(&process.processInformation,sizeof(process.processInformation));

        CreateProcessA(NULL,(LPSTR)(OmegaCommon::String(cmd) + args).c_str(),NULL,NULL,FALSE,0,NULL,NULL,&startupinfo,&process.processInformation);

        return process;
    }

    int ChildProcess::wait() {
        WaitForSingleObject(processInformation.hProcess,INFINITE);
        CloseHandle(processInformation.hThread);
        CloseHandle(processInformation.hProcess);
    }
    
}