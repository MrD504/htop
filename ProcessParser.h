#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <dirent.h>
#include "constants.h"
#include "util.h"

class ProcessParser {
    public:
        static std::string getCmd(std::string pid);
        static std::vector<std::string> getPidList();
        static std::string getVmSize(std::string pid);
        static std::string getCpuPercent(std:: string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(std::string pid);
        static std::string getProcUser(std::string pid);
        static std::vector<std::string> getSysCpuPercent();
        static std::vector<std::string> getSysCpuPercent(std::string coreNumber);
        static float getSysRamPercent();
        static std::string getSysKernelVersion();
        static int getTotalThreads();
        static int getNumberOfProcesses(std::string);
        static int getNumberOfCores();
        static std::string getOSName();
        static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
        static float get_sys_active_cpu_time(std::vector<std::string> values);
        static float get_sys_idle_time(std::vector<std::string> values);
    private:
        static std::vector<std::string> getValuesFromBuffer(std::string line);
};