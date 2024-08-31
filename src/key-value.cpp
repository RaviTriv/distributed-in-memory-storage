#include <stdio.h>
#include <cassert>
#include <unordered_map>
#include <string>
#include "../include/key-value.h"
#include <iostream>

using namespace std;

KeyValueStore::KeyValueStore()
{
}

void KeyValueStore::set(string key, string value)
{
  m.insert(make_pair(key, value));
}

string KeyValueStore::get(string key)
{
  return m.at(key);
}
