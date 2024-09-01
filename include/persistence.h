#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

class DataPersistence{
  public:
    DataPersistence();
    void write(string key, string val);
  private:
    //FILE *databin;
};