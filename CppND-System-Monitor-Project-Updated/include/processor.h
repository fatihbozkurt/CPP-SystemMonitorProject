#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <chrono>
#include <iostream>

class Processor {
 public:
  float Utilization();                 

 private:
  long PrevIdleTime {0};
  long PrevActiveTime {0};
  long ActiveTime {0};
  long IdleTime {0};
};

#endif