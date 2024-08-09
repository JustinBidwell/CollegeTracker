#include "../include/include.h"
#include "../include/menus.h"
#include <cctype>
int main(int argc, char* argv[]) {
    while (true) {
        std::string user_choice = mainMenu();
        if (user_choice == "1") {
            accountMenu();
        } else if (user_choice == "2") {
            std::vector<std::string> data = loginMenu();
            std::string username = data[0];
            std::string color = data[1];
            std::cout << "\nLogged In As ";
            std::cout << "\033[" + color + "m" + username + "\033[0m"
                      << std::endl;
            while (true) {
                std::string user_entry = collegeMenu();
                if (user_entry == "1") {
                    break;
                } else {
                    std::cout << "**INVALID ENTRY! PLEASE SELECT AN OPTION "
                                 "LISTED ON THE MENU!**"
                              << std::endl;
                }
            }
        } else if (user_choice == "3") {
            break;
        } else {
            std::cout << "**INVALID ENTRY! PLEASE SELECT AN OPTION LISTED ON "
                         "THE MENU!**"
                      << std::endl;
        }
    }
}
