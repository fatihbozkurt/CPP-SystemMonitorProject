#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
  std::string ElapsedTime(long seconds);  
  std::string LeadingZero(int time_unit);
};                                   // namespace Format

#endif