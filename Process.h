#pragma once
#include <string>
#include "ProcessParser.h"

class Process {
    private:
        std::string pid;
        std::string User;
        std::string CMD;
        std::string CPU;
        std::string memory;
        std::string UpTime; // Todo: make a date class
    public:
        Process(std::string pid) {
            this->pid = pid;
            this->User = ProcessParser::getProcUser(pid);
            this->CMD = ProcessParser::getCmd(pid);
            this->memory = ProcessParser::getVmSize(pid);
            this->UpTime = ProcessParser::getProcUpTime(pid);
            this->CPU = ProcessParser::getCpuPercent(pid);
        }
        
        std::string getPid(void);
        void setPid(std::string pid);
        
        std::string getUser(void);
        
        std::string getCmd(void);

        std::string getCPU(void);

        std::string getMemory(void);

        std::string getUpTime(void);

        std::string getProcess();
};