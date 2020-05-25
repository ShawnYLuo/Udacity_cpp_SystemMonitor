#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()const{
  string line, val;
  float tot = 0.0;
  float sec = LinuxParser::UpTime() -LinuxParser::UpTime(pid_) ;
  std::ifstream stream(LinuxParser::kProcDirectory +std::to_string(pid_)+ LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    for (int i=0; i<17; i++){
      linestream >> val;
      if (i>=13)
        tot += static_cast<float>(std::stol(val))/static_cast<float>(sysconf(_SC_CLK_TCK));
    }
  }
  float usage = tot/sec; 
  return usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const{
  float my_use = CpuUtilization();
  float a_use = a.CpuUtilization();
  return my_use > a_use;
}