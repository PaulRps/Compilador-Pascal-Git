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
    float time_begin_;
    static File *instance_;
    File();

public:
    static File* getInstance();
    string dataInput(const string fileName);
    void timeIn();
    void executionTime();
};

#endif
