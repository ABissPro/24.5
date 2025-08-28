#include <iostream>
#include <iomanip> //для форматирования вывода
#include <ctime>
#include <chrono> //std::chrono::seconds(1) - задает продолжительность паузы (1 секунда)
#include <thread> //для sleep_for; приостанавливает выполнение текущего потока

int main() {
    std::tm timer_input = {};
    int total_seconds = 0;

    std::cout << "Enter time in format mm:ss: ";
    std::cin >> std::get_time(&timer_input, "%M:%S");

    if (std::cin.fail()) {
        std::cout << "Error, use format mm:ss" << std::endl;
        return 1;
    }

    total_seconds = timer_input.tm_min * 60 + timer_input.tm_sec;

    if (total_seconds <= 0) {
        std::cout << "Time needs to be > 0" << std::endl;
        return 1;
    }

    std::time_t start_time = std::time(nullptr);
    std::time_t end_time = start_time + total_seconds;

    std::cout << "Timer on "
        << std::setfill('0') << std::setw(2) << timer_input.tm_min << ":"
        << std::setfill('0') << std::setw(2) << timer_input.tm_sec << std::endl;

    while (true) {
        std::time_t current_time = std::time(nullptr);
        std::time_t remaining = end_time - current_time;

        if (remaining <= 0) {
            break;
        }

        int minutes_remaining = static_cast<int>(remaining) / 60;
        int seconds_remaining = static_cast<int>(remaining) % 60;

        std::cout << "\n Time left: "
            << std::setfill('0') << std::setw(2) << minutes_remaining << ":"
            << std::setfill('0') << std::setw(2) << seconds_remaining << std::flush;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "\nDING! DING! DING!" << std::endl;

    return 0;
}