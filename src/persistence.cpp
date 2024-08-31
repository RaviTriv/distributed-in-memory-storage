#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

#include "../include/persistence.h"

DataPersistence::DataPersistence()
{
}
void DataPersistence::write(string key, string val)
{
  databin = fopen("src/data.txt", "a+");
  printf("HERE\n");
  fprintf(databin, "hello world");
}