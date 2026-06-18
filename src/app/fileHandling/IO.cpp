#include<iostream>
#include "IO.hpp"  // including own header file 
using namespace std;
// calling constructor of IO class 
IO::IO(const std:: string &file_path){
   file_stream.open(file_path,std::ios::in | std::ios::out | std::ios::binary);

   if(!file_stream.is_open()){
    std::cout<<"unable to open file: "<<file_path<<std::endl;
   } 
}   

std::fstream IO::getFileStream(){
    return std::move(file_stream);
}

IO::~IO(){
 if(file_stream.is_open()){
    file_stream.close(); // when object is out of bound free memory and close the file
 }   
}