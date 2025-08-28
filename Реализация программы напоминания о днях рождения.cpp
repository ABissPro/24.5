#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <vector>

//преобразованиe строки в tm{}
tm parseDate(const std::string& dateStr) {
    tm date = {};
    size_t pos1 = dateStr.find('/');
    size_t pos2 = dateStr.find('/', pos1 + 1);

    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        date.tm_year = std::stoi(dateStr.substr(0, pos1)) - 1900;
        date.tm_mon = std::stoi(dateStr.substr(pos1 + 1, pos2 - pos1 - 1)) - 1;
        date.tm_mday = std::stoi(dateStr.substr(pos2 + 1));
    }

    return date;
}

int getYearDay(const tm& date) {
    tm temp = date;
    std::time_t t = std::mktime(&temp);

    //кроссплатформенность (localtime)*
    tm localTime = {};
#if defined(_WIN32)
    localtime_s(&localTime, &t);
#else
    tm* tempTime = std::localtime(&t);
    localTime = *tempTime;
#endif

    return localTime.tm_yday;
}

int main() {
    std::map<std::string, tm> birthdays;
    std::string name;

    while (true) {
        std::cout << "Enter name (or 'end' for stop): ";
        std::getline(std::cin, name);
        if (name == "end") break;

        std::cout << "Enter birthday (year/month/day): ";
        std::string dateStr;
        std::getline(std::cin, dateStr);

        birthdays[name] = parseDate(dateStr);
    }

    std::time_t now = std::time(nullptr);
    tm currentDate = {};

#if defined(_WIN32)
    localtime_s(&currentDate, &now);
#else
    tm* temp = std::localtime(&now);
    currentDate = *temp;
#endif

    int currentYearDay = getYearDay(currentDate);


    std::vector<std::string> todayBirthdays;
    std::multimap<int, std::string> upcomingBirthdays; //ключ - день года, значение - имя

    for (std::map<std::string, tm>::const_iterator it = birthdays.begin(); it != birthdays.end(); ++it) {
        const std::string& friendName = it->first;
        tm birthDate = it->second;

        birthDate.tm_year = currentDate.tm_year;
        int birthYearDay = getYearDay(birthDate);

        if (birthYearDay == currentYearDay) {
            todayBirthdays.push_back(friendName);
        }
        else if (birthYearDay > currentYearDay) {
            upcomingBirthdays.insert(std::make_pair(birthYearDay, friendName));
        }
    }

    if (!todayBirthdays.empty()) {
        std::cout << "Today is birthday of: " << std::endl;
        for (std::vector<std::string>::const_iterator it = todayBirthdays.begin(); it != todayBirthdays.end(); ++it) {
            std::cout << " - " << *it << std::endl;
        }
    }

    if (!upcomingBirthdays.empty()) {
        std::multimap<int, std::string>::const_iterator nextBirthday = upcomingBirthdays.begin();
        int closestDay = nextBirthday->first;

        std::cout << "The nearest birthday is: " << std::endl;
        std::pair<std::multimap<int, std::string>::const_iterator, std::multimap<int, std::string>::const_iterator> range =
            upcomingBirthdays.equal_range(closestDay);

        for (std::multimap<int, std::string>::const_iterator it = range.first; it != range.second; ++it) {
            tm birthDate = birthdays.find(it->second)->second;
            std::cout << " - " << it->second << ": "
                << (birthDate.tm_mon + 1) << "/" << birthDate.tm_mday << std::endl;
        }
    }
    else if (todayBirthdays.empty()) {
        std::cout << "There're no comming birthdays" << std::endl;
    }

    return 0;
}