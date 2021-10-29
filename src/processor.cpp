#include "processor.h"
#include "linux_parser.h"

float jiffies_n0 = (float)LinuxParser::Jiffies();
float act_jiffies_n0 = (float)LinuxParser::ActiveJiffies(); 


float Processor::Utilization() { 
    float jiffies_n1 = (float)LinuxParser::Jiffies();
    float act_jiffies_n1 = (float)LinuxParser::ActiveJiffies(); 

    float diff_j = jiffies_n1 - jiffies_n0;
    float diff_act_j = act_jiffies_n1 - act_jiffies_n0;

    jiffies_n0 = jiffies_n1; 
    act_jiffies_n0 = act_jiffies_n1;

    if (diff_j != 0){
        return (diff_act_j/diff_j);
    }
    else {
        return 0.0f; 
    }

}