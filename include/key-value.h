#include <stdio.h>
#include <cassert>
#include <map>
#include <string>

using namespace std;

class KeyValueStore
{
public:
  KeyValueStore();
  void set();
private:
  map<string, string> m;
};
