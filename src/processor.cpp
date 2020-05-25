#include "processor.h"
//#include <unistd.h>
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization

float Processor::Utilization() {
  float res = 0.0;
  long a1 = LinuxParser::ActiveJiffies();
  long i1 = LinuxParser::IdleJiffies();
  res = static_cast<float>(a1-a)/static_cast<float>(a1+i1-a-i);
  //res = static_cast<float>(a1)/static_cast<float>(a1+i1);
  a = a1;
  i = i1;
  return res;
}

//float Processor::Utilization() {return 0.0;}