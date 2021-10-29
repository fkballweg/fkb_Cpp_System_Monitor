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
 //adapted from: https://stackoverflow.com/questions/1420426/how-to-calculate-the-cpu-usage-of-a-process-by-pid-in-linux-from-c

    float pid_utime = (float)LinuxParser::UpTime(Pid());
    float utime = (float)LinuxParser::UpTime();
    float utime_diff = utime - pid_utime;
    float act_jiffies = (float)LinuxParser::ActiveJiffies();

    return ((act_jiffies-utime_diff)/(float)sysconf(_SC_CLK_TCK));
     
    }

string Process::Command() { 
    std::string command_ = LinuxParser::Command(Pid());
    return command_; 
}

string Process::Ram() const { 
    std::string ram_ = LinuxParser::Ram(Pid());
    return ram_; 
}

string Process::User() { 
    std::string user_ = LinuxParser::User(Pid());
    return user_; 
}

long int Process::UpTime() {
    long int utime_ = LinuxParser::UpTime(Pid());
     return utime_;
}

bool Process::operator<(Process const& a) const { 
    long aRam = std::stol(a.Ram());
    long thisRam = std::stol(Ram());
    return thisRam < aRam; 
}