#include "processor.h"
#include "linux_parser.h"


float Processor::Utilization() {
 
  long TotalTime, PrevTotalTime, TotalTimeDiff, IdleDiff;
  float CPU_Utilization;
  
  ActiveTime = LinuxParser::ActiveJiffies();
  IdleTime = LinuxParser::IdleJiffies();

  PrevTotalTime = PrevActiveTime + PrevIdleTime;
  TotalTime = ActiveTime + IdleTime;

  TotalTimeDiff = TotalTime - PrevTotalTime;
  IdleDiff = IdleTime - PrevIdleTime;

  CPU_Utilization = (float)(TotalTimeDiff - IdleDiff)/TotalTimeDiff;
  
  PrevIdleTime = IdleTime;
  PrevActiveTime = ActiveTime;
  
  return CPU_Utilization;
}