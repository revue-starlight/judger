#pragma once
#include <iostream>
#include <cstring>
#include <string>
class Result{
  std::string time,mem;
  const uint SUCCESS = 0;
  const uint TLE = 1;
  const uint MLE = 2;
  const uint OLE = 4;
  const uint RE = 8;
};
