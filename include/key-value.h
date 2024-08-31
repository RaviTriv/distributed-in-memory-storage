#include <stdio.h>
#include <cassert>
#include <unordered_map>
#include <string>

using namespace std;

class KeyValueStore
{
public:
  KeyValueStore();
  void set(string key, string val);
  string get(string key);
  void update(string key, string val);
  void deletePair(string key);

private:
  unordered_map<string, string> m;
};
