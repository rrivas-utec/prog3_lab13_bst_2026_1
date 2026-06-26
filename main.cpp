#include <iostream>
#include <string>
#include "map_utec.h"

void test_1() {
    utec::map<std::string, int> mp1;

    mp1.insert("A", 10);
    mp1.insert("B", 30);
    mp1.insert("C", 40);
    mp1.insert("D", 40);

    for (auto it = mp1.begin(); it != mp1.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << "-----" << std::endl;
    std::cout << mp1.size() << std::endl;
    std::cout << "-----" << std::endl;
    for (const auto& [key, value] : mp1) {
        std::cout << key << " " << value << std::endl;
    }

}

int main() {
    test_1();
}