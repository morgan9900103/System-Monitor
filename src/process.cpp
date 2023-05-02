#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_util_; }

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

void Process::Pid(int pid) { pid_ = pid; }

void Process::User(std::string user) { user_ = user; }

void Process::Command(std::string cmd) { command_ = cmd; }

void Process::UpTime(long int uptime) { uptime_ = uptime; }

void Process::CpuUtilization(float util) { cpu_util_ = util; }

void Process::Ram(std::string ram) { ram_ = ram; }
