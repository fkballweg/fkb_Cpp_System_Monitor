#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // this does somehow only work when I give the name of the class itself. I normally wanted to give an other name. Can you explain why I have to use the name of the class? I now just did it because I have seen in in an example that worked...
  int Pid() const;                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram() const;                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  
 private:
    int pid_;
};

#endif