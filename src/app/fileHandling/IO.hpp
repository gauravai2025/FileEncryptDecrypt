#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>
#include<iostream>

class IO{
 public:
// defining constructor
// const- file path/directory remain same,  &file_path- taking file path by reference/address 
IO(const std::string &file_path); // call when new object is created memory is allocated 
// defining deconstructor
~ IO(); // call when object is deleted free space/memory

std::fstream getFileStream(); // getter 
private:
std::fstream file_stream;


};

#endif
