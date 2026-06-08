#include<iostream>
#include<filesystem>
#include "./src/app/processes/processManagement.hpp"
#include "./src/app/processes/Task.hpp"

namespace fs=std::filesystem;   

int main(int argc,char *argv[]){
 std::string directory;
 std::string action;
 
  std::cout<<"enter file directory path: "<<std::endl;
  std::getline(std::cin,directory);

 std::cout<<"enter file action(encrypt/decrypt): "<<std::endl;
 std::getline(std::cin,action);

try{

if(fs::exists(directory) && fs::is_directory(directory)){
    ProcessManagement ProcessManagement;    

    for(const auto &entry: fs::recursive_directory_iterator(directory)){
        if(entry.is_regular_file()){
           std::string filePath=entry.path().string(); 
            IO io(filePath);
            std::fstream f_stream=std::move(io.getFileStream());

            if(f_stream.is_open()){
             Action taskAction=(action=="encrypt"?Action::ENCRYPT:Action::DECRYPT);  
             auto task=std::make_unique<Task>(std::move(f_stream),taskAction,filePath);
             ProcessManagement.submitToQueue(std::move(task)); 
            }

            else{
             std::cout<<"unable to open the file: "<<filePath<<std::endl;   
            }

            }
        }

        ProcessManagement.executeTasks();
    }
    else{
    std::cout<<"directory does not exist/ invalid directory"<<std::endl;   
    }
}
// exception handling
catch(fs::filesystem_error &ex){ // accessing exception by refrence
std::cout<<"filesyatem error"<<ex.what()<<std::endl;
}
return 0;
}