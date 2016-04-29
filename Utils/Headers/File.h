#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>//time
#include <unistd.h>//time
//#include <windows.h>//time

using namespace std;

class File{

private:
  static float time_begin;

public:
  static std::string dataInput(string fileName);
  static void timeIn();
  static void executionTime();

};

#endif
