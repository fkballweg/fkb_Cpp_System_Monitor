#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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
  string os, kernel, version;
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

float LinuxParser::MemoryUtilization() { 
  std::string line, key, value;
  float totalMem, freeMem; 

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){ 
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal:"){
          totalMem = std::stof(value);
        }
        if (key == "MemFree:"){
          freeMem = std::stof(value);
          break;
        }
      }
    }
  }
  return (totalMem - freeMem)/totalMem; 
}

long LinuxParser::UpTime() { 
  std::string line, upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return std::stol(upTime); 
}

long LinuxParser::Jiffies() { 
  long jiffies;
  std::vector<std::string> CpuUtilization = LinuxParser::CpuUtilization();
  vector<long> values(10, 0);
  vector<CPUStates> states = {kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_};
  for (int i : states) { 
    values[i] = stol(CpuUtilization[i]);
    jiffies += values[i];
  };
  return jiffies; 
  }

long LinuxParser::ActiveJiffies(int pid) { 
  std::vector<std::string> buffer; 
  std::string line, wert;
  long time; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> wert) {
      //std::cout << wert << std::endl;
      buffer.emplace_back(wert);
    }
    time = std::stol(buffer[13]) + //utime
           std::stol(buffer[14]) + //stime
           std::stol(buffer[15]) + //cutime
           std::stol(buffer[16]); //cstime
  }
  return time; 
}

long LinuxParser::ActiveJiffies() { 
  long active_jiffies;
  active_jiffies = LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
  return active_jiffies; 
  }

long LinuxParser::IdleJiffies() { 
  long idle_jiffies;
  std::vector<std::string> CpuUtilization = LinuxParser::CpuUtilization();
  idle_jiffies = std::stol(CpuUtilization[kIdle_]) + std::stol(CpuUtilization[kIOwait_]);
  
  return idle_jiffies; 
}

vector<string> LinuxParser::CpuUtilization() { 
  std::string line, key, value;
  std::vector <std::string> cpuUti;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value){
        cpuUti.emplace_back(value);
      }
    }
  return cpuUti; 
}

int LinuxParser::TotalProcesses() {
  std::string line, key, number; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> number){
        if (key == "processes")
          return std::stoi(number);
      }
    }
  }
  return 0; 
  }

int LinuxParser::RunningProcesses() { 
  std::string line, key, number;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> number){
        if (key == "procs_running"){
          return std::stoi(number);
        }
      }
    }
  }

  return 0; 
}

string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
     std::getline(filestream, line);
     if(line != "")
       return line;
  }
return line; 
}


string LinuxParser::Ram(int pid) { 
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmSize:") {
            return value;    
        }
      }
    }
  }
  return "0"; 
}

string LinuxParser::Uid(int pid) { 
  std::string line, key, value; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      while(linestream >> key >> value){
        if (key == "Uid")
          return value;
      }
    }
  }
  return string(); 
}

string LinuxParser::User(int pid) {
  std::string line, key, value, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream .is_open()) {
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value >> uid){
        if (uid == LinuxParser::Uid(pid)){
          return key;
          break;
        }
      }
    }
  }
  return "no_user"; 
}

long LinuxParser::UpTime(int pid) { 
  std::string line, wert; 
  std::vector<std::string> buffer;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> wert){
      buffer.emplace_back(wert);
    }
  }
  return UpTime() - std::stol(buffer[21])/ sysconf(_SC_CLK_TCK);
}