#include "../include/accountquery.h"
#include "../include/class.h"
#include "../include/include.h"
#include "../include/menus.h"
#include <cctype>
#include <cstdlib>

int main(int argc, char* argv[]) {
    Format menu;
    if (menu.width < 70) {
        std::cout << "**INCREASE TERMINAL WIDTH TO AT LEAST 70**" << std::endl;
        std::cout << "CURRENT TERMINAL WIDTH: " << menu.width << std::endl;
        return 0;
    }

    while (true) {
        std::string user_choice = mainMenu();
        if (user_choice == "1") {
            accountMenu();
        } else if (user_choice == "2") {
            std::vector<std::string> data = loginMenu();
            std::string username = data[0];
            std::string color = data[1];
            std::cout << "\nLogged In As ";
            std::cout << "\033[" + color + "m" + username + "\033[0m" << std::endl;
            while (true) {
                std::string user_entry = collegeMenu();
                if (user_entry == "1") {
                    displayClasses(username, color, "null", "all");
                    std::string usersub_entry = displayClassesMenu();
                    while (true) {
                        if (usersub_entry == "1") {
                            system("clear");
                            displayClasses(username, color, "null", "all");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "2") {
                            system("clear");
                            displayClasses(username, color, "freshman", "semester");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "3") {
                            system("clear");
                            displayClasses(username, color, "sophmore", "semester");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "4") {
                            system("clear");
                            displayClasses(username, color, "junior", "semester");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "5") {
                            system("clear");
                            displayClasses(username, color, "senior", "semester");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "6") {
                            system("clear");
                            displayClasses(username, color, "completed", "status");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "7") {
                            system("clear");
                            displayClasses(username, color, "in_progress", "status");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "8") {
                            system("clear");
                            displayClasses(username, color, "scheduled", "status");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "9") {
                            system("clear");
                            displayClasses(username, color, "not_taken", "status");
                            usersub_entry = displayClassesMenu();
                        } else if (usersub_entry == "10") {
                            break;
                        } else {
                            std::cout << "**INVALID ENTRY! PLEASE SELECT AN OPTION "
                                         "LISTED ON THE MENU!**"
                                      << std::endl;
                            usersub_entry = displayClassesMenu();
                        }
                    }
                } else if (user_entry == "2") {
                    updateClass(username);
                } else if (user_entry == "3") {
                    std::string display_entry = displayStats(username);
                    system("clear");
                } else if (user_entry == "4") {
                    break;
                } else {
                    std::cout << "**INVALID ENTRY! PLEASE SELECT AN OPTION "
                                 "LISTED ON THE MENU!**"
                              << std::endl;
                }
            }
        } else if (user_choice == "3") {
            system("clear");
            system("neofetch");
        } else if (user_choice == "4") {
            break;
        } else {
            std::cout << "**INVALID ENTRY! PLEASE SELECT AN OPTION LISTED ON "
                         "THE MENU!**"
                      << std::endl;
        }
    }
}
