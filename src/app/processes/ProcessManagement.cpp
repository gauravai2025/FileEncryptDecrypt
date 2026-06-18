#include<string>
#include<iostream>
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait(), waitpid()
#include <unistd.h>     // fork(), getpid(), getppid()
#include "processManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"
#include<atomic>
#include<sys/mman.h>
#include<sys/fcntl.h>
#include<semaphore.h>
#include <cstring>


// defining constructor
ProcessManagement:: ProcessManagement(){
  
    itemSemaphore =sem_open("/items_semaPhore",O_CREAT, 0666,0); // starting zero task
    emptySlotsSemaphore=sem_open("/empty_slots_semaPhore",O_CREAT, 0666,1000);// starting 1000 empty slots available


if (itemSemaphore == SEM_FAILED) {
    perror("sem_open itemSemaphore");
    exit(EXIT_FAILURE);
}

if (emptySlotsSemaphore == SEM_FAILED) {
    perror("sem_open emptySlotsSemaphore");
    exit(EXIT_FAILURE);
}


  shFd=shm_open(SHM_NAME,O_CREAT | O_RDWR,0666);// giving permission tp admin , user, user_group to read write
  ftruncate(shFd,sizeof(SharedMemory));
  SharedMem =static_cast<SharedMemory*>(mmap(nullptr,sizeof(SharedMemory),PROT_READ | PROT_WRITE, MAP_SHARED,shFd,0));
  SharedMem->front=0;
  SharedMem->rear=0;
  SharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task>task){
    //  taskQueue.push(std::move(task));
   
    sem_wait(emptySlotsSemaphore); // sem_wait() decreases (decrements) the semaphore value by 1.

//   If emptySlotsSemaphore > 0
// Decrement it by 1.
// Continue execution.
// If emptySlotsSemaphore == 0
// It waits until another thread/process increases the semaphore using sem_post().

    std::unique_lock<std::mutex>lock(queueLock);

    if(SharedMem->size.load()>=1000){ // queue is full
    return false;
    }

    strcpy(SharedMem->tasks[SharedMem->rear],task->toString().c_str()); // serailising the task
    SharedMem->rear=(SharedMem->rear+1)%1000; // cyclic queue
    SharedMem->size.fetch_add(1);

    lock.unlock();
    //lock.unlock(); is executed:
// The mutex becomes available.
// Other threads waiting for the mutex can acquire it.
// The current thread leaves the critical section.

    sem_post(itemSemaphore); // // signal slot became empty item increase by 1

     int pid=fork();

     if(pid<0) {// child process not created
     return false;
     }

     else if(pid>0){ // parent process
    // waitpid(); pid process will wait for finishing of child process
     std::cout<<"Entering to parent process: "<<std::endl;
     }
     
   else{ //pid==0-> child process
   std::cout<<"Entering the child process: "<<std::endl;
   executeTasks();
   std::cout<<"Exiting the child process: "<<std::endl;
   exit(0);
    }
     return true;
}

void ProcessManagement::executeTasks(){

   sem_wait(itemSemaphore); // item decrease by one
   std::unique_lock<std::mutex>lock(queueLock);

  char taskStr[256];
  strcpy(taskStr, SharedMem->tasks[SharedMem->front]);
  SharedMem->front=(SharedMem->front+1)%1000;
  SharedMem->size.fetch_sub(1);

      lock.unlock();
      sem_post(emptySlotsSemaphore); // emptyslot increase by one

  std::cout<<"Executing child process: "<<std::endl; 
   executeCryption(taskStr);


  //  while(!taskQueue.empty()){
    // std::unique_ptr<Task> taskToExecute=std::move(taskQueue.front());
    // taskQueue.pop();
    // std::cout<<"Executing task "<<taskToExecute->toString()<<std::endl;
    // executeCryption take serialise string and convert into deserialise string
    // executeCryption(taskToExecute->toString());
}

ProcessManagement:: ~ProcessManagement(){
  munmap(SharedMem,sizeof(SharedMemory));
  shm_unlink(SHM_NAME);

}

