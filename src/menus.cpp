#include "../include/accountquery.h"
#include "../include/include.h"
#include <vector>
std::string mainMenu() {
    std::string header =
        "\nWELCOME TO COLLEGE COURSE TRACKER! PLEASE SELECT AN OPTION BELOW!";
    std::cout << header << std::endl;
    std::cout << std::string(header.length(), '-') << std::endl;
    std::cout << "[1] Create An Account" << std::endl;
    std::cout << "[2] Login To Account" << std::endl;
    std::cout << "[3] Quit" << std::endl;
    std::cout << "Enter A Choice From The Menu:" << std::endl;
    std::string user_choice;
    std::cin.sync();
    std::getline(std::cin >> std::ws, user_choice);
    system("clear");
    return user_choice;
}

void accountMenu() {
    std::vector<std::string> colors { "0",  "30", "31", "32", "33",
                                      "34", "35", "36", "37" };
    while (true) {
        std::string header = "\nCREATE AN ACCOUNT";
        std::cout << header << std::endl;
        std::cout << std::string(header.length(), '-') << std::endl;
        std::string username = "";
        std::string password = "";
        std::string password_confirm = "";

        std::cout << "Enter Username:" << std::endl;
        std::cin.sync();
        std::getline(std::cin >> std::ws, username);
        if (!usernameCheck(username)) {
            continue;
        }

        std::cout << "Enter Password:" << std::endl;
        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin >> std::ws, password);
        if (!passwordCheck(password)) {
            continue;
        }
        std::cout << "Please Re-Enter Password:" << std::endl;
        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin >> std::ws, password_confirm);

        if (password == password_confirm) {
            std::cout << "\nUser Color List" << std::endl;
            std::cout << "\033[0m[1] White\033[0m" << std::endl;
            std::cout << "\033[30m[2] Black\033[0m" << std::endl;
            std::cout << "\033[31m[3] Red\033[0m" << std::endl;
            std::cout << "\033[32m[4] Green\033[0m" << std::endl;
            std::cout << "\033[33m[5] Yellow\033[0m" << std::endl;
            std::cout << "\033[34m[6] Blue\033[0m" << std::endl;
            std::cout << "\033[35m[7] Pink\033[0m" << std::endl;
            std::cout << "\033[36m[8] Cyan\033[0m" << std::endl;
            std::cout << "\033[37m[9] Gray\033[0m" << std::endl;
            std::string color;

            while (true) {
                std::cout << "Please Select A Color For Your User: "
                          << std::endl;
                std::cin.clear();
                std::cin.sync();
                std::getline(std::cin >> std::ws, color);

                if (color.length() == 1) {
                    if (std::isdigit(color[0]) && color != "0") {
                        break;
                    } else {
                        std::cout << "**INVALID ENTRY; PLEASE SELECT FROM THE "
                                     "COLOR MENU**"
                                  << std::endl;
                        continue;
                    }
                } else {
                    std::cout << "**INVALID ENTRY; PLEASE SELECT FROM THE "
                                 "COLOR MENU**"
                              << std::endl;
                    continue;
                }
            }

            int index = stoi(color);
            insertAccount(username, password, colors[index - 1]);
            system("clear");
            break;
        } else {
            std::cout << "**PASSWORDS DO NOT MATCH**" << std::endl;
        }
    }
}

std::vector<std::string> loginMenu() {
    while (true) {
        std::string header = "\nLOGIN TO YOUR ACCOUNT";
        std::cout << header << std::endl;
        std::cout << std::string(header.length(), '-') << std::endl;
        std::string username;
        std::string password;

        std::cin.clear();
        std::cin.sync();
        std::cout << "Username: " << std::endl;
        std::getline(std::cin >> std::ws, username);

        std::cin.clear();
        std::cin.sync();
        std::cout << "Password: " << std::endl;
        std::getline(std::cin >> std::ws, password);

        std::vector<std::string> data = loginCheck(username, password);
        if (data[0] != "") {
            system("clear");
            return data;
        } else {
            std::cout
                << "**USERNAME OR PASSWORD IS INCORRECT; PLEASE TRY AGAIN**"
                << std::endl;
        }
    }
}

std::string collegeMenu() {
    std::string header = "WELCOME TO COLLEGE COURSE TRACKER";
    std::cout << header << std::endl;
    std::cout << std::string(header.length(), '-') << std::endl;
    std::cout << "[1] Logout" << std::endl;
    std::string user_entry;

    std::cin.clear();
    std::cin.sync();
    std::cout << "Enter A Choice From The Menu: " << std::endl;
    std::getline(std::cin >> std::ws, user_entry);

    system("clear");
    return user_entry;
}
