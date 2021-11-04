#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

Processor::Processor(){ GetCurrentUsage();}

void Processor::GetCurrentUsage(){
   std::vector<std::string> cpu_uti = LinuxParser::CpuUtilization();
   idle_ = std::stof(cpu_uti[3]) + std::stof(cpu_uti[4]);
   nonIdle_ = std::stof(cpu_uti[0]) + std::stof(cpu_uti[1]) + std::stof(cpu_uti[2]) 
            + std::stof(cpu_uti[5]) + std::stof(cpu_uti[6]) + std::stof(cpu_uti[7]);
   
}
float Processor::Utilization() {  
   long diff_total, diff_idle;
   long prevIdle, prevNonIdle, prevTotal, total;
   float cpuPercantage;
   
   prevIdle = idle_;
   prevNonIdle = nonIdle_;
   
   GetCurrentUsage();
   
   prevTotal = prevIdle + prevNonIdle;
   total = idle_ + nonIdle_;

   diff_total = total - prevTotal;
   diff_idle = idle_ - prevIdle;
   
   if ((float)diff_total > 0){
      cpuPercantage = ((float)diff_total - (float)diff_idle ) / (float)diff_total;
   }
   else{
      cpuPercantage = 0.0f;
   }

   return cpuPercantage;
   
   
   
   
}