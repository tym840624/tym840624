
#include <iostream>
#include <string>
#include <vector>

std::vector<int> splitStringToInt(const std::string& str, char delimiter) {
        std::vector<int> result;
        std::string item;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == delimiter) {
                result.push_back(std::stoi(item));
                item.clear();
            } else {
                item += str[i];
            }
        }
        result.push_back(std::stoi(item));
        return result;
    }

int main() {
    std::string str = "20:23:05:30:04:::";
    std::vector<int> fields = splitStringToInt(str, ':');
    for (size_t i = 0; i < fields.size(); ++i) {
        std::cout << fields[i] << std::endl;
    }
    return 0;
}