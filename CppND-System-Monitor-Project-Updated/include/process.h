#ifndef PROCESS_H
#define PROCESS_H

#include <string>


class Process {
 public:
  Process(int pid) : pid_(pid) {}                                 
  int Pid();
  std::string User();                                             
  std::string Command();                                          
  float CpuUtilization() const;                                        
  std::string Ram();                                              
  long int UpTime();                                              
  bool operator<(Process const& a) const;                         
  bool operator>(Process const& a) const;                         

 private:
  int pid_;
  float CPU_Utilization_ {0};                                      
                                   
};

#endif