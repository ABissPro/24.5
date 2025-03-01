#include <iostream>
#include <ctime>
#include <map>
#include <string>

void addTaskToList(std::map<std::string, std::time_t>& taskManager,
    const std::string& task,
    std::time_t taskTime) {
    if (taskManager.find(task) != taskManager.end()) {
        taskManager[task] += taskTime;
    }
    else {
        taskManager[task] = taskTime;
    }
}

int main() {
    std::map<std::string, std::time_t> taskManager;
    std::string currentTask;
    std::time_t startTime = 0;
    std::string command;

    while (true) {
        std::cout << "\nEnter command (begin/end/status/exit): ";
        std::cin >> command;

        if (command == "begin") {
            if (!currentTask.empty()) {
                std::time_t duration = std::time(nullptr) - startTime;
                addTaskToList(taskManager, currentTask, duration);
            }
            std::cout << "Enter task name: ";
            std::cin >> currentTask;
            startTime = std::time(nullptr);
        }
        else if (command == "end") {
            if (!currentTask.empty()) {
                std::time_t duration = std::time(nullptr) - startTime;
                addTaskToList(taskManager, currentTask, duration);
                currentTask.clear();
            }
        }
        else if (command == "status") {
            std::cout << "\nFinished tasks:\n";
            for (std::map<std::string, std::time_t>::const_iterator it = taskManager.begin();
                it != taskManager.end(); ++it) {
                std::cout << "• " << it->first << ": " << it->second << " sec\n";
            }
            if (!currentTask.empty()) {
                std::time_t currentDuration = std::time(nullptr) - startTime;
                std::cout << "\nCurrent task: " << currentTask
                    << " (" << currentDuration << " sec elapsed)\n";
            }
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Unknown command!\n";
        }
    }
}