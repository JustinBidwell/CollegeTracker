#include "../include/class.h"
#include "../include/include.h"
#include <cctype>
#include <exception>
bool usernameCheck(std::string username) {
    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";

    if (username.length() < 5 || username.length() > 20) {
        std::cout << "**USERNAME NEEDS TO BE BETWEEN 5 AND 20 CHARACTERS LONG**" << std::endl;
        return false;
    }
    for (int i = 0; i < username.length(); i++) {
        if (ispunct(username[i])) {
            std::cout << "**USERNAME IS NOT ALLOWED TO HAVE PUNCTUATION**" << std::endl;
            return false;
        } else if (isspace(username[i])) {
            std::cout << "**USERNAME IS NOT ALLOWED TO HAVE SPACES**" << std::endl;
            return false;
        }
    }

    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result result =
            worker.exec_params("SELECT username FROM users WHERE username = $1", username);
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
        std::cout << "**PASSWORD NEEDS TO BE AT LEAST 6 CHARACTERS LONG" << std::endl;
        return false;
    }
    return true;
}

void insertAccount(std::string username,
                   std::string password,
                   std::string color,
                   std::string major) {
    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        worker.exec_params("INSERT INTO users (username, password, color, "
                           "major) VALUES ($1, $2, $3, $4)",
                           username,
                           password,
                           color,
                           major);
        worker.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<std::string> loginCheck(std::string username, std::string password) {
    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result result = worker.exec_params(
            "SELECT username,password,color FROM users WHERE username = $1", username);
        worker.commit();
        if (result.size() > 0) {
            if (result[0][1].c_str() == password) {
                return std::vector<std::string> { result[0][0].c_str(), result[0][2].c_str() };
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return std::vector<std::string> { "", "" };
}

std::string selectMajor(std::string username) {
    std::string major;
    Format menu;
    std::vector<std::string> major_degrees = {
        "applied-engineering-sciences",
        "biosystems-engineering",
        "chemical-engineering",
        "civil-engineering",
        "computational-data-science",
        "computer-engineering",
        "computer-science",
        "electrical-engineering",
        "environmental-engineering",
        "materials-science-engineering",
        "mechanical-engineering",
    };
    menu.displayBorder();
    menu.header = "Engineering Major List";
    for (int i = 0; i < major_degrees.size(); i++) {
        menu.choice_str = "[" + std::to_string(i + 1) + "] " + major_degrees[i];
        menu.displayChoicesString();
    }
    menu.displayBorder();

    std::string major_text;
    while (true) {
        std::cin.clear();
        std::cin.sync();
        std::cout << "Select An Engineering Major: " << std::endl;
        std::getline(std::cin >> std::ws, major);

        try {
            int index = stoi(major);
            if (index > 0 && index < 12) {
                major_text = major_degrees[index - 1];
                break;
            } else {
                std::cout << "**INVALID ENTRY, TRY AGAIN**" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "**INVALID ENTRY, TRY AGAIN**" << std::endl;
        }
    }

    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result r = worker.exec_params("SELECT data FROM scrapperdata WHERE degree = "
                                            "$1",
                                            major_text);
        worker.commit();

        std::string json_string = r[0][0].c_str();
        nlohmann::json json_object = nlohmann::json::parse(json_string);

        for (const auto& item : json_object.items()) {
            std::string year = item.key();
            for (const auto& val : item.value().items()) {
                std::string semester = val.key();
                for (const auto& val2 : val.value().items()) {
                    std::string course = val2.key();
                    std::string credits = val2.value();
                    if (course == credits) {
                        continue;
                    } else {
                        worker.exec_params("INSERT INTO classes (username, class, credits, "
                                           "year, semester, completed, in_progress, "
                                           "not_taken, scheduled) VALUES ($1, $2, $3, $4, $5, "
                                           "false, false, true, false)",
                                           username,
                                           course,
                                           credits,
                                           year,
                                           semester);
                        worker.commit();
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return major_text;
}

void printClasses(std::string username) { }
