#include "../include/include.h"
#include <cctype>
bool usernameCheck(std::string username) {
    std::string connection_string =
        "dbname = college user = postgres password = "
        "Learn8128 port = 5432";

    if (username.length() < 5 || username.length() > 20) {
        std::cout << "**USERNAME NEEDS TO BE BETWEEN 5 AND 20 CHARACTERS LONG**"
                  << std::endl;
        return false;
    }
    for (int i = 0; i < username.length(); i++) {
        if (ispunct(username[i])) {
            std::cout << "**USERNAME IS NOT ALLOWED TO HAVE PUNCTUATION**"
                      << std::endl;
            return false;
        } else if (isspace(username[i])) {
            std::cout << "**USERNAME IS NOT ALLOWED TO HAVE SPACES**"
                      << std::endl;
            return false;
        }
    }

    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result result = worker.exec_params(
            "SELECT username FROM users WHERE username = $1", username);
        worker.commit();

        if (result.size() > 0) {
            if (result[0][0].c_str()) {
                std::cout << "**USERNAME ALREADY EXISTS**" << std::endl;
                return false;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return true;
}

bool passwordCheck(std::string password) {
    if (password.length() < 6) {
        std::cout << "**PASSWORD NEEDS TO BE AT LEAST 6 CHARACTERS LONG"
                  << std::endl;
        return false;
    }
    return true;
}

void insertAccount(std::string username,
                   std::string password,
                   std::string color) {
    std::string connection_string =
        "dbname = college user = postgres password = "
        "Learn8128 port = 5432";
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        worker.exec_params(
            "INSERT INTO users (username, password, color) VALUES ($1, $2, $3)",
            username,
            password,
            color);
        worker.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<std::string> loginCheck(std::string username,
                                    std::string password) {
    std::string connection_string =
        "dbname = college user = postgres password = "
        "Learn8128 port = 5432";
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result result = worker.exec_params(
            "SELECT username,password,color FROM users WHERE username = $1",
            username);
        worker.commit();
        if (result.size() > 0) {
            if (result[0][1].c_str() == password) {
                return std::vector<std::string> { result[0][0].c_str(),
                                                  result[0][2].c_str() };
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return std::vector<std::string> { "", "" };
}
