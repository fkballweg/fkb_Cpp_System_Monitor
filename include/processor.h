#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  
  void GetCurrentUsage();
  
 private:
    long idle_;
    long nonIdle_;
};

#endif