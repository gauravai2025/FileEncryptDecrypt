#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include<memory>
#include<queue>
#include "Task.hpp"
#include<semaphore.h>
#include<atomic>
#include <mutex>


class ProcessManagement{
    sem_t* itemSemaphore;
    sem_t* emptySlotsSemaphore;
    public:
    // defining constructor
    ProcessManagement();
     // defining destructor
    ~ProcessManagement(); // free shared memory
    bool submitToQueue(std::unique_ptr<Task>task);
    void executeTasks();

    private:
    // std::queue<std::unique_ptr<Task>>taskQueue; // for single process/ sequential execution
    struct SharedMemory{
    std::atomic<int>size; // at a time one process can access size variable in shared memory
    char tasks[1000][256];// task will store 1000 files at a time and with each file can have 256 characters
   // 1 character size=1 byte
   int front;
   int rear;

   void printSharedMemory(){
    std::cout<<size<<std::endl;
   }
};

 SharedMemory* SharedMem;
    int shFd;
    const char* SHM_NAME= "/my_queue";
    std::mutex queueLock; 
};

#endif