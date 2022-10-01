#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


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


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    
    if (file->d_type == DT_DIR) {
      
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


float LinuxParser::MemoryUtilization() { 
  
  float MemoryUtilization, TotalMemory, FreeMemory;
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) 
    {
      std::istringstream stream(line);
      while(stream >> key >> value) 
      {
        if(key == "MemTotal:") 
          TotalMemory = stol(value);
        else if(key == "MemFree:")
          FreeMemory = stol(value);
       }
    }
  }

  MemoryUtilization = (TotalMemory - FreeMemory) / TotalMemory;
  return MemoryUtilization;
}


long LinuxParser::UpTime() { 
  
  long UpTime;
  string line, value;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    UpTime = stol(value);  
  }
  
  return UpTime; 
}


long LinuxParser::Jiffies() { 
  
  long SystemTime UpTime();
  long SystemFrequency sysconf(_SC_CLK_TCK);
  
  return  (SystemTime * SystemFrequency); 
  
}


long LinuxParser::ActiveJiffies(int pid) { 
  
  string line, value, jiffies_toString;
  long UserTime, KernelTime, C_UserTime, C_KernelTime, TotalTime;
  vector<string> TimeValues;
  int counter {0};
  
  jiffies_toString = to_string(pid);
  std::ifstream filestream(kProcDirectory + jiffies_toString + kStatFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      TimeValues.push_back(value);
      counter++;
      if(counter == TIMEVALUES_MAX_INDEX)
        break;
    }
  }
  
  if (counter == TIMEVALUES_MAX_INDEX) {
    UserTime = stol(TimeValues[USER_TIME_INDEX]);
    KernelTime = stol(TimeValues[KERNEL_TIME_INDEX]);
    C_UserTime = stol(TimeValues[C_USER_TIME_INDEX]);
    C_KernelTime = stol(TimeValues[C_KERNEL_TIME_INDEX]);
    TotalTime = UserTime + KernelTime + C_UserTime + C_KernelTime;
  }
  
  return TotalTime; 
}


long LinuxParser::ActiveJiffies() { 
  
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_])); 
}


long LinuxParser::IdleJiffies() { 
  
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));

}


vector<string> LinuxParser::CpuUtilization() { 

  vector<string> utilization;
  string line, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    if (stream >> value) {
      if (value == "cpu") {
        while (stream >> value)
          utilization.push_back(value);
      }
    }
  }
  
  return utilization; 
}


int LinuxParser::TotalProcesses() { 
  
  int numProcesses;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if(key == "processes") {
          numProcesses = stoi(value);
        }
      }
    }
  }
  return numProcesses;
  
}


int LinuxParser::RunningProcesses() { 
  
  int runProcesses;
  string line, key, value;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if(key == "procs_running") {
          runProcesses = stoi(value);
        }
      }
    }
  }
  return runProcesses;

}


string LinuxParser::Command(int pid) { 
  
  string line, pid_toString;

  pid_toString = to_string(pid);
  std::ifstream filestream(kProcDirectory + pid_toString + kCmdlineFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream,line);
    return line;
  }
  else
    return string("NULL"); 
  
}


string LinuxParser::Ram(int pid) { 

  string line, key, value, pid_toString, value_toString;
  pid_toString = to_string(pid);
  
  std::ifstream filestream(kProcDirectory + pid_toString + kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "VmSize:") {
          value_toString = to_string(stol(value) / KB_DIVIDER);
          return value_toString;
        }
      }
    }
  }
  
  return string("NULL"); 
  
}


string LinuxParser::Uid(int pid) {
  string line, key, UidValue, uid_toString;

  uid_toString = to_string(pid);
  std::ifstream filestream(kProcDirectory + uid_toString + kStatusFilename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> UidValue) {
        if (key == "Uid:") {
          return UidValue;
        }
      }
    }
  }
  return UidValue;
}


string LinuxParser::User(int pid) {
  string UserValue, password, UidValue, line;
  
  std::ifstream filestream(kPasswordPath);
  
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> UserValue >> password >> UidValue) {
        if (password == "x" && (UidValue == Uid(pid))) {
          return UserValue;
        }
      }
    }
  }
  
  return string("NULL"); 
}


long LinuxParser::UpTime(int pid) {
  string line, UpTimeValue, UpTime_toString;
  int counter = 0;
  long UpTime;
  
  UpTime_toString = to_string(pid);
  std::ifstream filestream(kProcDirectory + UpTime_toString + kStatFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> UpTimeValue){
      counter++;
      if (counter == UPTIME_INDEX) {
        UpTime = stol(UpTimeValue) / sysconf(_SC_CLK_TCK);
        return UpTime;
      }
    }
  }
  return 0;
}
