#include"Process.h"


std::string Process::getPid() { return this->pid; }
void Process::setPid(std::string pid) { this->pid = pid; }
std::string Process::getUser() { return this->User; }
std::string Process::getCmd(void) { return this->CMD; }
std::string Process::getCPU(void) { return this->CPU; }
std::string Process::getMemory(void) { return this->memory; }
std::string Process::getUpTime(void){ return this->UpTime; }

std::string Process::getProcess(){
    this->memory = ProcessParser::getVmSize(this->pid);
    this->UpTime = ProcessParser::getProcUpTime(this->pid);
    this->CPU = ProcessParser::getCpuPercent(this->pid);

    return (this -> pid + "   "
                        + this->User
                        + "   "
                        + this->memory.substr(0,5)
                        + "   "
                        +this->CPU.substr(0,5)
                        + "   "
                        +this->UpTime.substr(0,5)
                        + "   "
                        +this->CMD.substr(0,30)
                        + "...");
}