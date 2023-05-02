#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = 0;
  long minutes = 0;
  hours = seconds / (60 * 60);
  seconds %= 60 * 60;
  minutes = seconds / 60;
  seconds %= 60;

  string HH, MM, SS;
  HH = std::to_string(hours);
  MM = std::to_string(minutes);
  SS = std::to_string(seconds);

  if (hours < 9) {
    HH = "0" + HH;
  }
  if (minutes < 9) {
    MM = "0" + MM;
  }
  if (seconds < 9) {
    SS = "0" + SS;
  }
  return string(HH + ":" + MM + ":" + SS);
}
