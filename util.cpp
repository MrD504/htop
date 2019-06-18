#include "util.h"

std::string Util::convertToTime(long int input_seconds) {
    int minutes = input_seconds / 60;
    int hours = minutes / 60;
    int seconds = int(input_seconds % 60);

    minutes = int(minutes % 60);

    std::string result = convertNumToString(hours) + ":" + convertNumToString(minutes) + ":" + convertNumToString(seconds);

    return result;
}

std::string Util::getProgressBar( std::string percent ) {
    // Create progress bar. Each bar will represent 2%.
    // There will need to be a space inbetween in bar
    std::string result = "0% ";
    int max_length = 50;
    int used_percent = (stof(percent) / 100) * max_length;;
    for (int count = 0; count < max_length; count += 2) {
        if(count < used_percent) {
            result += "| ";
        } else {
            result += "  ";
        }
    }

    result += " " + percent.substr(0,5) + "%/100%";

    return result;
}

std::ifstream Util::getStream(std::string path) {
    std::ifstream stream(path);

    if(!stream) {
        // handle error
        throw std::runtime_error("Non-existing PID" + path);
    }

    return stream;
}

std::string Util::convertNumToString(int num) {
    std::string result = "";
    if (num < 10) {
        result += "0" + std::to_string(num);
    } else {
        result += std::to_string(num);
    }

    return result;
}

std::string Util::convertUidToUsername(std::string uid){
    std::ifstream stream = Util::getStream("/etc/passwd");
    std::string line;
    std::string result;
    std::string name = ("x:" + uid);
    while(std::getline(stream, line)){
        if (line.find(name) != std::string::npos) {
            result = line.substr(0, line.find(":"));
            return result;
        }
    }

    return "";
}