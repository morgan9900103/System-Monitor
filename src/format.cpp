#include "format.h"

#include <string>
#include <iomanip>

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = 0;
  long minutes = 0;
  hours = seconds / (60 * 60);
  seconds %= 60 * 60;
  minutes = seconds / 60;
  seconds %= 60;

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ":"
         << std::setw(2) << std::setfill('0') << minutes << ":"
         << std::setw(2) << std::setfill('0') << seconds;
  
  return stream.str();
}
