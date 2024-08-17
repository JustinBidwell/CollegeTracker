#include "../include/include.h"

bool usernameCheck(std::string username);
bool passwordCheck(std::string password);
void insertAccount(std::string username,
                   std::string password,
                   std::string color,
                   std::string major);
std::vector<std::string> loginCheck(std::string username, std::string password);
std::string selectMajor(std::string username);
void printClasses(std::string username);
