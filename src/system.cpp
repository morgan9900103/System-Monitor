#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

// Debug
#include <fstream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() {
  cpu_.UpdateCpuState(LinuxParser::CpuUtilization());
  return cpu_;
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();

  // Get Pids
  vector<int> pids_input = LinuxParser::Pids();
  std::set<int> pids_set;
  for (auto it = pids_input.begin(); it != pids_input.end(); it++) {
    pids_set.insert(*it);
  }

  vector<int> pids;
  for (auto it = pids_set.begin(); it != pids_set.end(); it++) {
    pids.emplace_back(*it);
  }

  // Set Process
  for (size_t i = 0; i < pids.size(); i++) {
    Process proc;
    // Set pid
    proc.Pid(pids.at(i));

    // Set user
    std::string uid = LinuxParser::Uid(pids.at(i));
    std::string user = LinuxParser::User(uid);
    proc.User(user);

    // Set Command
    std::string command = LinuxParser::Command(pids.at(i));
    proc.Command(command);

    // Set Uptime
    long int process_time = LinuxParser::UpTime() - LinuxParser::UpTime(pids.at(i));
    proc.UpTime(process_time);

    // Set Process Cpu Utilization
    long total_time = LinuxParser::ActiveJiffies(pids.at(i));
    long seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pids.at(i));
    float cpu_usage = 0;
    cpu_usage = static_cast<float>(total_time) / static_cast<float>(seconds) / 100;

    proc.CpuUtilization(cpu_usage);

    // Set Ram
    long ram_mb = std::stol(LinuxParser::Ram(pids.at(i))) / 1000;
    proc.Ram(std::to_string(ram_mb));

    processes_.emplace_back(proc);
  }

  sort(processes_.rbegin(), processes_.rend());

  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
