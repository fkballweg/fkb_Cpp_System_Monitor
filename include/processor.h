#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  
 private:
 long jiffies;
 long activeJiffies;
};

#endif