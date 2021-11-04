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

Process::Process(int pid){
    pid_ = pid;
}

int Process::Pid() const {
    return pid_; 
}

float Process::CpuUtilization() { 
    float pid_uti;
    float pid_uptime = (float)LinuxParser::UpTime(Pid());
    float uptime = (float)LinuxParser::UpTime();
    float pid_act_jiffies = (float)LinuxParser::ActiveJiffies(Pid())/sysconf(_SC_CLK_TCK);

    pid_uti = pid_act_jiffies / (uptime - pid_uptime);
    
    return pid_uti;
}

string Process::Command() { 
    std::string command_ = LinuxParser::Command(Pid());
    return command_; 
}

string Process::Ram() const { 
    std::string ram_ = LinuxParser::Ram(Pid());
    return std::to_string(std::stoi(ram_)/1024);
}

string Process::User() { 
   return LinuxParser::User(Pid());
}

long int Process::UpTime() {
    return LinuxParser::UpTime(Pid());
     
}

bool Process::operator<(Process const& a) const { 
    long aRam = std::stol(a.Ram());
    long thisRam = std::stol(Ram());
    return thisRam < aRam; 
}