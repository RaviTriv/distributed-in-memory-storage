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
  databin = fopen("../data/data.txt", "r+");
  if (databin != NULL)
  {
    char line[1024];
    sprintf(line, "\n{%s : %s}", key.c_str(), val.c_str());
    fseek(databin, 0, SEEK_END);
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
  //TODO: read
  char res[1024];
  databin = fopen("../data/data.txt", "r");
  fgets(res, 1024, databin);
  fclose(databin);
  return res;
}