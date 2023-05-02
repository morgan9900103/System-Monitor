#include "processor.h"

#include <cassert>
#include <string>
#include <vector>

Processor::Processor() {
  state_prev_.resize(10);
  state_curr_.resize(10);
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long long prev_idle = state_prev_.at(3) + state_prev_.at(4);
  long long idle = state_curr_.at(3) + state_curr_.at(4);

  long long prev_non_idle = state_prev_.at(0) + state_prev_.at(1) +
                            state_prev_.at(2) + state_prev_.at(5) +
                            state_prev_.at(6) + state_prev_.at(7);
  long long non_idle = state_curr_.at(0) + state_curr_.at(1) +
                       state_curr_.at(2) + state_curr_.at(5) +
                       state_curr_.at(6) + state_curr_.at(7);

  long long prev_total = prev_idle + prev_non_idle;
  long long total = idle + non_idle;

  // Differentiate
  long long totald = total - prev_total;
  long long idled = idle - prev_idle;

  return (float)(totald - idled) / (float)totald;
}

void Processor::UpdateCpuState(std::vector<std::string> state) {
  // Copy current state to previous state
  state_prev_ = state_curr_;

  // Update current state
  for (size_t i = 0; i < state.size(); i++) {
    state_curr_.at(i) = std::stol(state.at(i));
  }
}
