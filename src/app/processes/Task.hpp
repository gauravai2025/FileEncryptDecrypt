// define custom data structure like struct

#ifndef TASK_HPP
#define TASK_HPP
#include<string>
#include<iostream>
#include<sstream>
#include "../fileHandling/IO.hpp"

enum class Action{
 ENCRYPT,
 DECRYPT
};

struct Task{
std::string filePath;
std::fstream f_stream;
Action action; 

// constructor for task class
Task(std::fstream &&stream,Action act, std::string filePath )
:f_stream(std::move(stream)),action(act),filePath(filePath)
{};

// Task(std::fstream &&stream,Action act, std::string filePath ){
// f_stream=std::move(stream);
// action=act;
// this->filePath=filePath;
// }

// serialise process
std:: string toString(){
std::ostringstream oss;
oss<<filePath<<","<<(action==Action::ENCRYPT?"ENCRYPT": "DECRYPT");
// text.txt ENCRYPT f_stream-> Task
// "text.txt,ENCRYPT"
return oss.str();
}

// deserialise process
static Task fromString(const std::string &taskData){
std::istringstream iss(taskData);
std::string filePath;
std:: string actionStr;

// "text.txt,ENCRYPT"
// filePath="text.txt", action="ENCRYPT"
if(std::getline(iss,filePath,',') && std::getline(iss,actionStr)){
Action action=(actionStr=="ENCRYPT"? Action::ENCRYPT:Action::DECRYPT);

IO io(filePath);
std::fstream f_stream=std::move(io.getFileStream());

if(f_stream.is_open()){
return Task(std::move(f_stream),action, filePath);
}
// exception handling
else{
throw std::runtime_error("Failed to open file: "+ filePath);
}

}

else{
throw std::runtime_error("Invalid task data format ");   
}
}

};
#endif