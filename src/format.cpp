#include <string>

#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) { 
    long minutes = seconds / 60;
    long hour = seconds / 3600;
    std::string HH = std::to_string(hour);
    std::string MM = std::to_string(minutes%60);
    std::string SS = std::to_string(seconds%60);

    return HH + ":" + MM + ":" + SS; 
}