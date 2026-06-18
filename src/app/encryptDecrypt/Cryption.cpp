#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"
#include<ctime>
#include<iomanip>

int executeCryption(const std::string taskData){
Task task=Task::fromString(taskData);
 ReadEnv env;
 std:: string envkey=env.getenv();
 int key=std::stoi(envkey);

 if(task.action==Action::ENCRYPT){
    char ch;

    while(task.f_stream.get(ch)){  
     ch=(ch+key)%256;  // a=(69+1234567)%256-> [0,255]->convert to charector based on ascii code/Table
     task.f_stream.seekp(-1,std::ios::cur);// go back 1 index back make it current index hello
                                           // after processing h-> idx==1 now bo back 1 step idx=0;   
     task.f_stream.put(ch);
    }

    task.f_stream.close();

 }

 else{
  char ch;
   while(task.f_stream.get(ch)){  
    ch=(ch-key+256)%256;
    task.f_stream.seekp(-1,std::ios::cur);
    task.f_stream.put(ch);

   }

   task.f_stream.close();
 }
std::time_t t=std::time(nullptr);
std::tm* now= std::localtime(&t);
std::cout<<"Exiting the encryption/decryption at: "<<std::put_time(now, "%y-%m-%d %H:%M:%S")<<std::endl;
return 0;
}