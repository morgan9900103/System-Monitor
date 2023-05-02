#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  long long mem_total = 0;
  long long mem_free = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "MemTotal") {
        mem_total = stol(value);
      } else if (key == "MemFree") {
        mem_free = stol(value);
      }
    }
  }
  return (float)(mem_total - mem_free) / (float)mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string time_sys;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time_sys;
  }
  return std::stol(time_sys);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.emplace_back(value);
    }
  }

  return std::stol(values.at(13)) + std::stol(values.at(14)) +
         std::stol(values.at(15)) + std::stol(values.at(16));
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = LinuxParser::CpuUtilization();

  return std::stol(jiffies.at(CPUStates::kUser_)) +
         std::stol(jiffies.at(CPUStates::kNice_)) +
         std::stol(jiffies.at(CPUStates::kSystem_)) +
         std::stol(jiffies.at(CPUStates::kIRQ_)) +
         std::stol(jiffies.at(CPUStates::kUser_)) +
         std::stol(jiffies.at(CPUStates::kUser_));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jiffies = LinuxParser::CpuUtilization();

  return std::stol(jiffies.at(CPUStates::kIdle_)) +
         std::stol(jiffies.at(CPUStates::kIOwait_));
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> jiffies;
  string line, cpu, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> value) {
      jiffies.emplace_back(value);
    }
  }

  return jiffies;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  int total_processes = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        total_processes = std::stoi(value);
      }
    }
  }
  return total_processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  int running_processes = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        running_processes = std::stoi(value);
      }
    }
  }
  return running_processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize") {
        break;
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string uid;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid") {
        uid = value;
        break;
      }
    }
  }

  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(string num) {
  string line, user, x, uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if (uid == num) {
        break;
      }
    }
  }

  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::vector<std::string> times;
  string line, time;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> time) {
      times.emplace_back(time);
    }
  }
  return std::stol(times.at(21)) / sysconf(_SC_CLK_TCK);
}
