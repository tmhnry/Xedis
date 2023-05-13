#pragma once
#include <iostream>

enum {
  SER_NIL = 0,
  SER_ERR = 1,
  SER_STR = 2,
  SER_INT = 3,
  SER_ARR = 4,
};

enum {
  ERR_UNKNOWN = 1,
  ERR_2BIG = 2,
};

static void out_nil(std::string &out) { out.push_back(SER_NIL); };
