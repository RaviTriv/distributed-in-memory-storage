#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

#include "../include/persistence.h"

DataPersistence::DataPersistence()
{
}

void DataPersistence::write(std::string key, std::string val)
{
  databin = fopen("../data/data.txt", "w");
  if (databin != NULL)
  {
    char line[1024];
    sprintf(line, "{%s : %s}\n", key.c_str(), val.c_str());
    fputs(line, databin);
    fclose(databin);
  }
  else
  {
    printf("ERROR \n");
  }
}

string DataPersistence::read()
{
  char res[1024];
  databin = fopen("../data/data.txt", "r");
  fclose(databin);
  return res;
}