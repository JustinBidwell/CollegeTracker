#include "../include/class.h"
#include "../include/include.h"
#include <cctype>
#include <cstring>
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
        int id = 0;

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
                                           "not_taken, scheduled, id) VALUES ($1, $2, $3, $4, $5, "
                                           "false, false, true, false, $6)",
                                           username,
                                           course,
                                           credits,
                                           year,
                                           semester,
                                           id);
                        worker.commit();
                        id += 1;
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

std::string toUpper(std::string str) {
    std::string new_str;
    char new_char;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 97 && str[i] <= 122) {
            new_char = str[i] - 32;
            new_str.push_back(new_char);
        } else {
            new_str.push_back(str[i]);
        }
    }
    return new_str;
}

bool checkEntry(std::string entry, int vect_len) {
    if (entry.length() == 1) {
        if (std::isdigit(entry[0])) {
            int index = stoi(entry);
            if (index >= 1 && index <= vect_len) {
                return true;
            }
        }
    }
    return false;
}
void displayClassesHelper(pqxx::result r, std::string header, std::string color) {
    Format menu;
    menu.displayBorder();
    menu.color = color;
    menu.header = toUpper(header);
    menu.displayHeaderString();
    menu.color = "0";
    menu.displayBorder();
    menu.class_str = "COLLEGE COURSES";
    menu.month_str = "MONTHS";
    menu.credit_str = "CREDITS";
    menu.status_str = "STATUS";
    menu.classView();
    menu.displayBorder();
    for (int i = 0; i < r.size(); i++) {
        menu.class_str = toUpper(r[i][0].c_str());
        menu.credit_str = toUpper(r[i][1].c_str());
        menu.month_str = toUpper(r[i][3].c_str());
        if (to_string(r[i][4]) == "t") {
            menu.status_str = "COMPLETED";
            menu.single_color = "32";
        } else if (to_string(r[i][5]) == "t") {
            menu.status_str = "IN PROGRESS";
            menu.single_color = "33";
        } else if (to_string(r[i][6]) == "t") {
            menu.status_str = "NOT TAKEN";
            menu.single_color = "31";
        } else if (to_string(r[i][7]) == "t") {
            menu.status_str = "SCHEDULED";
            menu.single_color = "34";
        }
        menu.classView();
        menu.single_color = "0";
    }
    menu.displayBorder();
    std::cout << "\n";
}
void displayClasses(std::string username,
                    std::string color,
                    std::string filter,
                    std::string display_type) {
    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    std::vector<std::string> year_vector = { "freshman", "sophmore", "junior", "senior" };
    try {
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        std::string bool_str;
        if (display_type == "all") {
            for (int i = 0; i < year_vector.size(); i++) {
                pqxx::result r = worker.exec_params(
                    "SELECT class, credits, year, semester, completed, in_progress, "
                    "not_taken, scheduled FROM classes WHERE username = "
                    "$1 AND year = $2 ORDER BY semester ASC, credits DESC",
                    username,
                    year_vector[i]);

                worker.commit();
                displayClassesHelper(r, year_vector[i], color);
            }
        } else if (display_type == "semester") {
            pqxx::result r =
                worker.exec_params("SELECT class, credits, year, semester, completed, in_progress, "
                                   "not_taken, scheduled FROM classes WHERE username = "
                                   "$1 AND year = $2 ORDER BY semester ASC, credits DESC",
                                   username,
                                   filter);

            worker.commit();
            displayClassesHelper(r, filter, color);
        } else if (display_type == "status") {
            pqxx::result r;
            if (filter == "completed") {
                r = worker.exec_params(
                    "SELECT class, credits, year, semester, completed, in_progress, "
                    "not_taken, scheduled FROM classes WHERE username = "
                    "$1 AND completed = 't' ORDER BY semester ASC, credits DESC",
                    username);
            } else if (filter == "in_progress") {
                r = worker.exec_params(
                    "SELECT class, credits, year, semester, completed, in_progress, "
                    "not_taken, scheduled FROM classes WHERE username = "
                    "$1 AND in_progress = 't' ORDER BY semester ASC, credits DESC",
                    username);
            } else if (filter == "not_taken") {
                r = worker.exec_params(
                    "SELECT class, credits, year, semester, completed, in_progress, "
                    "not_taken, scheduled FROM classes WHERE username = "
                    "$1 AND not_taken = 't' ORDER BY semester ASC, credits DESC",
                    username);
            } else if (filter == "scheduled") {
                r = worker.exec_params(
                    "SELECT class, credits, year, semester, completed, in_progress, "
                    "not_taken, scheduled FROM classes WHERE username = "
                    "$1 AND scheduled = 't' ORDER BY semester ASC, credits DESC",
                    username);
            }

            worker.commit();
            displayClassesHelper(r, filter, color);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void updateClass(std::string username) {
    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    Format menu;
    std::string class_picked;
    try {
        std::vector<std::string> class_list;
        std::string user_choice;
        pqxx::connection collegedb(connection_string);
        pqxx::work worker(collegedb);
        pqxx::result r = worker.exec_params("SELECT class FROM classes WHERE username = "
                                            "$1 ORDER BY id ASC",
                                            username);
        worker.commit();
        menu.header = "CLASSES";
        menu.displayBorder();
        menu.displayHeaderString();
        menu.displayBorder();
        for (int i = 0; i < r.size(); i++) {
            class_list.push_back(r[i][0].c_str());
            std::string class_str = "[" + std::to_string(i + 1) + "] " + r[i][0].c_str();
            menu.choice_str = class_str;
            menu.displayChoicesString();
        }
        menu.displayBorder();
        while (true) {
            bool valid_number = true;
            std::cout << "Select A Class You Want To Update:" << std::endl;
            std::cin.sync();
            std::getline(std::cin >> std::ws, user_choice);
            for (int i = 0; i < user_choice.length(); i++) {
                if (std::isdigit(user_choice[i])) {
                    continue;
                } else {
                    valid_number = false;
                    break;
                }
            }
            if (valid_number) {
                int index = stoi(user_choice) - 1;
                if (index >= 0 && index < class_list.size()) {
                    class_picked = class_list[index];
                    break;

                } else {
                    std::cout << "**INVALID ENTRY**" << std::endl;
                }
            } else {
                std::cout << "**INVALID ENTRY**" << std::endl;
            }
        }

        system("clear");
        r = worker.exec_params("SELECT class, credits, year, semester, completed, in_progress, "
                               "not_taken, scheduled FROM classes WHERE username = "
                               "$1 AND class = $2",
                               username,
                               class_picked);
        worker.commit();
        std::string year_update = r[0][2].c_str();
        std::string semester_update = r[0][3].c_str();
        std::string completed_update = r[0][4].c_str();
        std::string inprogress_update = r[0][5].c_str();
        std::string nottaken_update = r[0][6].c_str();
        std::string scheduled_update = r[0][7].c_str();
        while (true) {
            menu.displayBorder();
            menu.header = "CURRENT CLASS DISPLAY";
            menu.displayHeaderString();
            menu.displayBorder();
            menu.class_str = r[0][0].c_str();
            menu.credit_str = r[0][1].c_str();
            menu.month_str = r[0][3].c_str();
            if (to_string(r[0][4]) == "t") {
                menu.status_str = "COMPLETED";
                menu.single_color = "32";
            } else if (to_string(r[0][5]) == "t") {
                menu.status_str = "IN PROGRESS";
                menu.single_color = "33";
            } else if (to_string(r[0][6]) == "t") {
                menu.status_str = "NOT TAKEN";
                menu.single_color = "31";
            } else if (to_string(r[0][7]) == "t") {
                menu.status_str = "SCHEDULED";
                menu.single_color = "34";
            }
            menu.classView();
            menu.displayBorder();

            std::vector<std::string> update_options = {
                "[1] Year", "[2] Semester", "[3] Status", "[4] Complete"
            };
            std::vector<std::string> year_options = { "freshman", "sophmore", "junior", "senior" };
            std::vector<std::string> semester_options = { "fall", "spring" };
            std::vector<std::string> status_options = {
                "completed", "in progress", "not completed", "scheduled"
            };

            for (int i = 0; i < update_options.size(); i++) {
                menu.choice_str = update_options[i];
                menu.displayChoicesString();
            }

            std::string column_update_choice;
            menu.displayBorder();

            std::cout << "Select A Column To Update (Not Including Option 4):" << std::endl;
            std::cin.sync();
            std::getline(std::cin >> std::ws, column_update_choice);

            std::string option_choice;

            if (column_update_choice == "4") {
                system("clear");
                worker.exec_params("UPDATE classes SET year = $1, semester = $2, completed = $3, "
                                   "in_progress = $4, not_taken = $5, scheduled = $6 WHERE "
                                   "username = $7 AND class = $8 AND id = $9",
                                   year_update,
                                   semester_update,
                                   completed_update,
                                   inprogress_update,
                                   nottaken_update,
                                   scheduled_update,
                                   username,
                                   class_picked,
                                   stoi(user_choice) - 1);
                worker.commit();
                break;
            } else if (column_update_choice == "1") {
                system("clear");
                menu.header = "YEAR";
                menu.displayBorder();
                menu.displayHeaderString();
                menu.displayBorder();

                for (int i = 0; i < year_options.size(); i++) {
                    std::string year_str = "[" + std::to_string(i + 1) + "] " + year_options[i];
                    menu.choice_str = year_str;
                    menu.displayChoicesString();
                }
                menu.displayBorder();
                while (true) {
                    std::cout << "Select Which Year You Took The Class:" << std::endl;
                    std::cin.sync();
                    std::getline(std::cin >> std::ws, option_choice);

                    if (checkEntry(option_choice, year_options.size())) {
                        year_update = year_options[stoi(option_choice) - 1];
                        std::cout << year_update << std::endl;
                        system("clear");
                        break;
                    } else {
                        std::cout << "**INVALID ENTRY**" << std::endl;
                    }
                }

            } else if (column_update_choice == "2") {
                system("clear");
                menu.header = "SEMESTER";
                menu.displayBorder();
                menu.displayHeaderString();
                menu.displayBorder();

                for (int i = 0; i < semester_options.size(); i++) {
                    std::string semester_str =
                        "[" + std::to_string(i + 1) + "] " + semester_options[i];
                    menu.choice_str = semester_str;
                    menu.displayChoicesString();
                }
                menu.displayBorder();
                while (true) {
                    std::cout << "Select Which Semester You Took The Class:" << std::endl;
                    std::cin.sync();
                    std::getline(std::cin >> std::ws, option_choice);

                    if (checkEntry(option_choice, semester_options.size())) {
                        semester_update = semester_options[stoi(option_choice) - 1];
                        std::cout << semester_update << std::endl;
                        system("clear");
                        break;
                    } else {
                        std::cout << "**INVALID ENTRY**" << std::endl;
                    }
                }

            } else if (column_update_choice == "3") {
                system("clear");
                menu.header = "STATUS";
                menu.displayBorder();
                menu.displayHeaderString();
                menu.displayBorder();

                for (int i = 0; i < status_options.size(); i++) {
                    std::string status_str = "[" + std::to_string(i + 1) + "] " + status_options[i];
                    menu.choice_str = status_str;
                    menu.displayChoicesString();
                }
                menu.displayBorder();
                std::vector<std::string> status_options = {
                    "completed", "in progress", "not completed", "scheduled"
                };
                while (true) {
                    std::cout << "Select The Current Status Of Your Class:" << std::endl;
                    std::cin.sync();
                    std::getline(std::cin >> std::ws, option_choice);

                    if (checkEntry(option_choice, status_options.size())) {
                        system("clear");
                        if (option_choice == "1") {
                            completed_update = "true";
                            scheduled_update = "false";
                            inprogress_update = "false";
                            nottaken_update = "false";
                            break;
                        } else if (option_choice == "2") {
                            completed_update = "false";
                            scheduled_update = "false";
                            inprogress_update = "true";
                            nottaken_update = "false";
                            break;
                        } else if (option_choice == "3") {
                            completed_update = "false";
                            scheduled_update = "false";
                            inprogress_update = "false";
                            nottaken_update = "true";
                            break;
                        } else if (option_choice == "4") {
                            completed_update = "false";
                            scheduled_update = "true";
                            inprogress_update = "false";
                            nottaken_update = "false";
                            break;
                        }
                        semester_update = semester_options[stoi(option_choice) - 1];
                        std::cout << semester_update << std::endl;
                        break;
                    } else {
                        std::cout << "**INVALID ENTRY**" << std::endl;
                    }
                }
            } else {
                system("clear");
                std::cout << "**NOT A VALID ENTRY**" << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::string displayStatsHelper(pqxx::result r, std::string title) {
    int total1 = 0;
    int total2 = 0;
    std::string final_string;
    for (int i = 0; i < r.size(); i++) {
        std::string temp = r[i][0].c_str();
        if (temp.length() == 1) {
            total1 += stoi(temp);
            total2 += stoi(temp);
        } else if (temp.length() == 3) {
            int int_temp1 = temp[0] - 48;
            int int_temp2 = temp[2] - 48;
            total1 += int_temp1;
            total2 += int_temp2;
        }
    }

    std::string total1_str = std::to_string(total1);
    std::string total2_str = std::to_string(total2);

    if (total1 == total2) {
        final_string = title + ": " + total1_str;
    } else {

        final_string = title + ": " + total1_str + "/" + total2_str;
    }

    return final_string;
}

std::string displayStats(std::string username) {
    Format menu;
    std::string option_choice;

    std::string connection_string = "dbname = college user = postgres password = "
                                    "Learn8128 port = 5432";
    pqxx::connection collegedb(connection_string);
    pqxx::work worker(collegedb);
    pqxx::result r;
    menu.displayBorder();
    menu.header = "STATS";
    menu.displayHeaderString();
    menu.displayBorder();
    r = worker.exec_params("SELECT credits FROM classes WHERE username = "
                           "$1",
                           username);

    menu.choice_str = displayStatsHelper(r, "Total");
    menu.displayChoicesString();

    r = worker.exec_params("SELECT credits FROM classes WHERE username = "
                           "$1 AND completed = 't'",
                           username);

    menu.choice_str = displayStatsHelper(r, "Completed Total");
    menu.color = "32";
    menu.displayChoicesString();

    r = worker.exec_params("SELECT credits FROM classes WHERE username = "
                           "$1 AND in_progress = 't'",
                           username);

    menu.choice_str = displayStatsHelper(r, "In Progress Total");
    menu.color = "33";
    menu.displayChoicesString();

    r = worker.exec_params("SELECT credits FROM classes WHERE username = "
                           "$1 AND scheduled = 't'",
                           username);

    menu.choice_str = displayStatsHelper(r, "Scheduled Total");
    menu.color = "34";
    menu.displayChoicesString();

    r = worker.exec_params("SELECT credits FROM classes WHERE username = "
                           "$1 AND not_taken = 't'",
                           username);

    menu.choice_str = displayStatsHelper(r, "Not Taken Total");
    menu.color = "31";
    menu.displayChoicesString();

    menu.displayBorder();

    std::cout << "Enter Anything To Go Back:" << std::endl;
    std::cin.sync();
    std::getline(std::cin >> std::ws, option_choice);

    return option_choice;
}
