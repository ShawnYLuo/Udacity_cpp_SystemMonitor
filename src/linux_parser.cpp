#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel,v;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> v >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  long totalMem, freeMem, value;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    int count = 0;
    while (count < 2) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") 
        totalMem = value;
      if (key == "MemFree:")
        freeMem = value;
      count += 1;
    }
  }
 return static_cast<float>(totalMem-freeMem)/static_cast<float>(totalMem);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  long res = std::stol(uptime);
  return res;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string jiffies;
  string line;
  std::ifstream stream(kProcDirectory +std::to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    for (int i=0; i<22; i++){
      linestream >> jiffies;
    }
  }
  return std::stol(jiffies);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string key, user, nice, sys, idle;
  string line;
  long res;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> sys >> idle) {
        if (key == "cpu") {
          res = std::stol(user)+std::stol(nice)+std::stol(sys);
          return res;}
      }
    }
  }
  return res;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string key, user, nice, sys, idle;
  string line;
  long res;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> sys >> idle) {
        if (key == "cpu") {
          res = std::stol(idle);
          return res;}
      }
    }
  }
  return res;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string value;
  long up = 0;
  long idle = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      for (int i=0; i<5; i++){
        linestream>>value;
        if (i>0 && i<4)
          up += std::stol(value);
        if (i==4)
          idle += std::stol(value);
      }
  }
  vector<string> res = {std::to_string(up),std::to_string(idle)};
  return res;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int res = 0;
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes"){
          res = std::stoi(value);
          return res;
        }
      }
    }
  }
  return res;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value;
  int res = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          res = std::stoi(value);
          return res;
        }
      }
    }
  }
  return res;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory +std::to_string(pid)+ "/cmdline");
  if (stream.is_open())
    std::getline(stream, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  string res;
  long mbUsage = 0;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "VmSize:") {
          mbUsage = std::stol(value)/1024;
          res = std::to_string(mbUsage);
          return res;
        }
    }
  }
  return res;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid){
  string line;
  string key;
  string id;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> id;
      if (key == "Uid:")
        return id;
    }
  }
  return id;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string id = LinuxParser::Uid(pid);
  string line;
  string user,letter,uid;
  std::ifstream stream("/etc/passwd");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> letter >> uid;
      if (uid == id)
        return user;
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long int jiff = LinuxParser::ActiveJiffies(pid);
  return jiff/static_cast<long>(sysconf(_SC_CLK_TCK));
}