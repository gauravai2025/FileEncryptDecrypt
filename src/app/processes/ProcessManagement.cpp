#include<string>
#include<iostream>
#include "processManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"


ProcessManagement:: ProcessManagement(){}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task>task){
     taskQueue.push(std::move(task));
     return true;

}

void ProcessManagement::executeTasks(){
   while(!taskQueue.empty()){
    std::unique_ptr<Task> taskToExecute=std::move(taskQueue.front());
    taskQueue.pop();
    std::cout<<"Executing task "<<taskToExecute->toString()<<std::endl;
    // executeCryption take serialise string and convert into deserialise string
    executeCryption(taskToExecute->toString());
   }
}




