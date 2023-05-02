#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();
  Processor();
  void UpdateCpuState(std::vector<std::string>);

 private:
  std::vector<long long> state_curr_ = {};
  std::vector<long long> state_prev_ = {};
};

#endif
