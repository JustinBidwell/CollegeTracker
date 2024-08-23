#include "../include/include.h"

bool usernameCheck(std::string username);
bool passwordCheck(std::string password);
void insertAccount(std::string username,
                   std::string password,
                   std::string color,
                   std::string major);
std::vector<std::string> loginCheck(std::string username, std::string password);
std::string selectMajor(std::string username);
void displayClasses(std::string username,
                    std::string color,
                    std::string filter,
                    std::string display_type);
void updateClass(std::string username);
std::string displayStats(std::string username);
