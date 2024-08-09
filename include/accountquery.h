#include "../include/include.h"

bool usernameCheck(std::string username);
bool passwordCheck(std::string password);
void insertAccount(std::string username,
                   std::string password,
                   std::string color);
std::vector<std::string> loginCheck(std::string username, std::string password);
