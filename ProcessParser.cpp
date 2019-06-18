#include"ProcessParser.h"

std::string ProcessParser::getCmd(std::string pid) {

    // read file in to string
    std::string file = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream = Util::getStream(file);
    std::string result;
    std::getline(stream, result);

    return result;
}

// std::vector<std::string> ProcessParser::getPidList(){
  
// }

std::string ProcessParser::getVmSize(std::string pid) {
    std::string line;
    std::string name = "VmData";
    std::string value;
    float result;

    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while(std::getline(stream, line)){
        if(line.compare(0,name.size(), name) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

            result = (stof(values[1])/float(1024^2));
            break;
        }
    }
    return std::to_string(result);
}

/* Questions for instructor:
* different to proposed solution. Why bother getting vector of all values 
* if just need first one?
* Done using pointers, is there any benefit to working this way especially with regards
* to memory management? */

long int ProcessParser::getSysUpTime() {
    Path* p = new Path;
    Util* u = new Util;

    // read file in to string
    std::string file = p->basePath() + p->upTimePath();
    std::ifstream stream = u->getStream(file);
    std::string result;
    std::getline(stream, result);

    delete p;
    delete u;

    return std::stoi(result);
}

std::string ProcessParser::getProcUpTime(std::string pid) {
    std::ifstream stream = Util::getStream((Path::basePath() + pid + "/" +  Path::statPath()));
    std::string line;
    std::getline(stream, line);
    std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

    return std::to_string(float(stof(values[13])/sysconf(_SC_CLK_TCK)));
}

std::string ProcessParser::getCpuPercent(std::string pid) {
    std::string line;
    std::ifstream stream = Util::getStream((Path::basePath() + pid + "/" + Path::statPath()));
    float result;

    float utime = stof(ProcessParser::getProcUpTime(pid));
    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);

    std::getline(stream, line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg,end);

    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);

    float totaltime = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    result = 100 * ((totaltime/freq)/seconds);

    return std::to_string(result);
}

// overloaded method to meet criteria - when nothing is passed "cpu" line is read
std::vector<std::string> ProcessParser::getSysCpuPercent() {
    return ProcessParser::getSysCpuPercent("");
}

std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber) {

    std::string line;
    std::ifstream stream = Util::getStream(Path::basePath() + Path::statPath());

    // while iterator less than number of cores
    std::string name = "cpu" + coreNumber;
    // get list

    while(std::getline(stream, line)){
        if(line.compare(0, name.size(), name)){
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

            return values;
        }
    }
}

std::string ProcessParser::getProcUser(std::string pid) {
    std::string line;
    std::string name = "Uid:";
    std::vector<std::string> linePart;
    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    std::string Uid;
    while(std::getline(stream, line)){
        if(line.compare(0,name.size(), name) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

            Uid = values[1];
            break;
        }
    }

    return Util::convertUidToUsername(Uid);
}

/* Solution differs from answer given. Is this more efficient? Bad practice? Why would lecturers 
*  method be superior?
*/
std::vector<std::string> ProcessParser::getPidList() {
    // Read directory
    std::vector<std::string> pidList;
    DIR *dir;
    struct dirent *ent;

    if ( (dir = opendir( Path::basePath().c_str() ) ) != NULL) {
        while ((ent = readdir(dir)) != NULL){
            std::string folder = ent->d_name;

            // make sure the each entry is a directory
            if(ent->d_type != DT_DIR) {
                continue;
            }
            // check if string can convert to int, if not suppress error
            try {
                // if yes add to array
                std::stoi(folder);
                pidList.push_back(folder);
            } catch ( std::invalid_argument const &e) {
                // suppress error
            } catch ( std::out_of_range const &e) {
                //supress error
            } 

        }
        if(closedir(dir)) {
            throw std::runtime_error(std::strerror(errno));
        };
    } else {
        throw std::runtime_error(std::strerror(errno));
    }

    return pidList;

}

int ProcessParser::getNumberOfCores() {
    std::string line;
    std::string name = "cpu cores";
    std::ifstream stream = Util::getStream(Path::basePath() + Path::cpuInfoPath());

    while(std::getline(stream, line)) {
        if(line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg,end);
            return std::stoi(values[3]);
        }
    }
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2) {
    float activeTime = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
    float idleTime = get_sys_idle_time(values2) - get_sys_idle_time(values1);
    float totalTime = activeTime + idleTime;
    float result = 100 * (activeTime / idleTime);
    return (std::to_string(result));
}

float ProcessParser::getSysRamPercent() {
    
    std::string file = Path::basePath() + Path::memInfoPath();
    std::ifstream stream = Util::getStream(file);
    std::string line;

    std::string sMemAvail = "MemAvailable:";
    std::string sMemFree = "MemFree";
    std::string sBuffers = "Buffers:";

    float totalMem = 0;
    float freeMem = 0;
    float buffers = 0;

    while(std::getline(stream, line)) {
        if(line.compare(0, sMemAvail.size(), sMemAvail) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);
            totalMem = std::stof(values[1]);
        }

        if(line.compare(0, sMemFree.size(), sMemFree) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);
            freeMem = std::stof(values[1]);
        }

        if(line.compare(0, sBuffers.size(), sBuffers) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);
            buffers = std::stof(values[1]);
        }
    }
    return float(100.0*(1-(freeMem/(totalMem-buffers))));

}

std::string ProcessParser::getSysKernelVersion() {
    std::string file = Path::basePath() + Path::versionPath();
    std::ifstream stream = Util::getStream(file);

    std::string line;

    std::string name = "Linux version ";

    while(std::getline(stream, line)) {
        if(line.compare(0, name.size(), name) == 0 ) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);
            return values[2];
        }

    }
}

float ProcessParser::get_sys_active_cpu_time(std::vector<std::string> values) {
    return (
        stof(values[S_USER]) +
        stof(values[S_NICE]) +
        stof(values[S_SYSTEM]) +
        stof(values[S_IRQ]) +
        stof(values[S_SOFTIRQ]) +
        stof(values[S_STEAL]) +
        stof(values[S_GUEST]) +
        stof(values[S_GUEST_NICE])    
    );
}

float ProcessParser::get_sys_idle_time(std::vector<std::string> values) {
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

std::vector<std::string> ProcessParser::getValuesFromBuffer(std::string line) {
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg,end);

    return values;
}

std::string ProcessParser::getOSName() {
    std::string name = "PRETTY_NAME";
    std::ifstream stream = Util::getStream("/etc/os-release");
    std::string line;
    
    while(std::getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0 ) {
            int sIdx = line.find("\"") + 1;
            int eIdx = line.find("\"", sIdx + 1) -1;
            return line.substr(sIdx, eIdx - sIdx);;
        }
    }

    return "nothing found";
}

int ProcessParser:: getTotalThreads() {
    std::string line;
    int result = 0;
    std::string name = "Threads:";

    std::vector<std::string> list = ProcessParser::getPidList();

    for(int count = 0; count < list.size(); count++) {
        std::string pid = list[count];

        std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));

        while(std::getline(stream, line)) {
            if(line.compare(0, name.size(), name) == 0) {
                std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

                result += std::stoi(values[1]);
                break;
            }
        }
    }
    return result;
}


// getTotalNumberOfProcesses and getNumberOfRunningProcesses are so similar refactored to on fn
int ProcessParser::getNumberOfProcesses(std::string name) {
    std::string line;
    int result = 0;

    std::vector<std::string> list = ProcessParser::getPidList();
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));

    while(std::getline(stream, line)) {
        if(line.compare(0, name.size(), name) == 0) {
            std::vector<std::string> values = ProcessParser::getValuesFromBuffer(line);

            result += std::stoi(values[1]);
            break;
        }
    }
    return result;
}