#include "../include/accountquery.h"
#include "../include/class.h"
#include "../include/include.h"
#include <vector>

std::string mainMenu() {
    Format menu;
    menu.displayBorder();
    menu.header = "WELCOME TO COLLEGE COURSE TRACKER! PLEASE SELECT AN OPTION BELOW";
    menu.displayHeaderString();
    menu.displayBorder();
    std::vector<std::string> choices = {
        "[1] Create An Account", "[2] Login To Account", "[3] Run NeoFetch", "[4] Quit"
    };

    for (int i = 0; i < choices.size(); i++) {
        menu.choice_str = choices[i];
        menu.displayChoicesString();
    }

    menu.displayBorder();
    std::cout << "Enter A Choice From The Menu:" << std::endl;
    std::string user_choice;
    std::cin.sync();
    std::getline(std::cin >> std::ws, user_choice);
    system("clear");
    return user_choice;
}

void accountMenu() {
    std::vector<std::string> colors { "0", "30", "31", "32", "33", "34", "35", "36", "37" };
    std::vector<std::string> colors_string { "[1] White", "[2] Black",  "[3] Red",
                                             "[4] Green", "[5] Yellow", "[6] Blue",
                                             "[7] Pink",  "[8] Cyan",   "[9] Gray" };
    Format menu;
    menu.header = "CREATE AN ACCOUNT";
    while (true) {
        menu.displayBorder();
        menu.displayHeaderString();
        menu.displayBorder();
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
            menu.displayBorder();
            menu.choice_str = "Color Menu";
            menu.displayChoicesString();
            for (int i = 0; i < colors_string.size(); i++) {
                menu.choice_str = colors_string[i];
                menu.color = colors[i];
                menu.displayChoicesString();
            }
            menu.color = "0";
            menu.displayBorder();

            std::string color;

            while (true) {
                std::cout << "Please Select A Color For Your User: " << std::endl;
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
            std::string major = selectMajor(username);
            insertAccount(username, password, colors[index - 1], major);
            system("clear");
            break;
        } else {
            std::cout << "**PASSWORDS DO NOT MATCH**" << std::endl;
        }
    }
}

std::vector<std::string> loginMenu() {
    while (true) {
        Format menu;
        menu.displayBorder();
        menu.header = "LOGIN TO YOUR ACCOUNT";
        menu.displayHeaderString();
        menu.displayBorder();
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
            std::cout << "**USERNAME OR PASSWORD IS INCORRECT; PLEASE TRY AGAIN**" << std::endl;
        }
    }
}

std::string collegeMenu() {
    std::vector<std::string> options = { "[1] Logout" };
    Format menu;
    menu.header = "WELCOME TO COLLEGE COURSE TRACKER";
    menu.displayBorder();
    menu.displayHeaderString();
    menu.displayBorder();

    for (int i = 0; i < options.size(); i++) {
        menu.choice_str = options[i];
        menu.displayChoicesString();
    }
    menu.displayBorder();

    std::string user_entry;
    std::cin.clear();
    std::cin.sync();
    std::cout << "Enter A Choice From The Menu: " << std::endl;
    std::getline(std::cin >> std::ws, user_entry);

    system("clear");
    return user_entry;
}
