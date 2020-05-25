#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hour = seconds / 3600;
  seconds = seconds % 3600;
  int minute = seconds / 60;
  seconds = seconds % 60;
  string h = std::to_string(hour);
  if (h.length()<2) h = '0'+h;
  string m = std::to_string(minute);
  if (h.length()<2) m = '0'+m;
  string s = std::to_string(seconds);
  if (h.length()<2) s = '0'+s;
  string res = h+":"+m+":"+s;
  
  return res; 
}