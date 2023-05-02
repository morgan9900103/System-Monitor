#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

  // Mutator
  void Pid(int);
  void User(std::string);
  void Command(std::string);
  void CpuUtilization(float);
  void Ram(std::string);
  void UpTime(long int);

  // Declare any necessary private members
 private:
  int pid_ = 0;
  std::string user_ = std::string();
  std::string command_ = std::string();
  float cpu_util_ = 0.0;
  std::string ram_ = std::string();
  long int uptime_ = 0;
};

#endif
