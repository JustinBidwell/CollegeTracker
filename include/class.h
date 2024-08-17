#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
struct Format {
public:
    std::string header = "NULL";
    std::string choice_str = "NULL";
    std::string class_str = "NULL";
    std::string month_str = "NULL";
    std::string credit_str = "NULL";
    std::string status_str = "NULL";
    std::string symbol = "*";
    std::string accent_symbol = "|";
    std::string color = "0";
    bool border = true;
    int width;
    Format() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if (w.ws_col % 2 != 0) {
            width = w.ws_col - 1;
        } else {
            width = w.ws_col;
        }
        class_len = width - (MONTH_LEN + CREDIT_LEN + STATUS_LEN);
    }
    void classView() {
        if (!border) {
            accent_symbol = " ";
        }
        std::cout << buildLeftString(class_len, class_str, true, true, color);
        std::cout << buildLeftString(MONTH_LEN, month_str, false, false, color);
        std::cout << buildLeftString(CREDIT_LEN, credit_str, true, true, color);
        std::cout << buildLeftString(STATUS_LEN, status_str, false, true, color) << std::endl;
    }
    void displayBorder() {
        for (int i = 0; i < width; i++) {
            std::cout << symbol;
        }
        std::cout << "\n";
    }
    void displayHeaderString() {
        if (!border) {
            accent_symbol = " ";
        }
        std::cout << buildCenterString(width, header, true, true, color) << std::endl;
    }
    void displayChoicesString() {
        if (!border) {
            accent_symbol = " ";
        }
        std::cout << buildLeftString(width, choice_str, true, true, color) << std::endl;
    }

private:
    int MONTH_LEN = 10;
    int CREDIT_LEN = 12;
    int STATUS_LEN = 16;
    int class_len;
    std::string
    buildCenterString(int len, std::string str, bool inside, bool outside, std::string color) {
        std::string final_str = "";
        int max_len = len - 2;
        while (true) {
            if (str.length() > max_len) {
                std::string resized_str = str.substr(0, max_len);
                str = str.substr(max_len, std::string::npos);
                int new_len = (len - resized_str.length()) / 2;
                resized_str.insert(resized_str.length(), new_len, ' ');
                resized_str.insert(0, new_len, ' ');
                if (inside) {
                    resized_str.replace(0, 1, accent_symbol);
                }
                if (outside) {
                    resized_str.replace(resized_str.length() - 1, 0, accent_symbol);
                }
                if (resized_str.length() < len) {
                    resized_str.push_back(' ');
                } else if (resized_str.length() > len) {
                    resized_str.pop_back();
                }
                final_str = final_str + resized_str + "\n";

            } else {
                int new_len = (len - str.length()) / 2;
                str.insert(str.length(), new_len, ' ');
                str.insert(0, new_len, ' ');
                if (inside) {
                    str.replace(0, 1, accent_symbol);
                }
                if (outside) {
                    str.replace(len - 1, 0, accent_symbol);
                }
                if (str.length() < len) {
                    str.push_back(' ');
                } else if (str.length() > len) {
                    str.pop_back();
                }

                std::string color_str = "\033[" + color + "m";
                final_str = final_str + str;
                if (inside) {
                    final_str.insert(1, color_str);
                } else {
                    final_str.insert(0, color_str);
                }
                if (outside) {
                    final_str.insert(final_str.length() - 1, "\033[0m");
                } else {
                    final_str.insert(final_str.length() - 1, "\033[0m");
                }
                return final_str;
            }
        }
    }
    std::string
    buildLeftString(int len, std::string str, bool inside, bool outside, std::string color) {
        int max_len = len - 4;
        int indent_size = 2;
        std::string final_str = "";
        while (true) {
            if (str.length() > max_len) {
                std::string resized_str = str.substr(0, max_len);
                str = str.substr(max_len, std::string::npos);
                resized_str = std::string(indent_size, ' ') + resized_str + " ";
                int new_len = (len - resized_str.length());
                resized_str.insert(resized_str.length(), new_len, ' ');
                if (inside) {
                    resized_str.replace(0, 1, accent_symbol);
                }
                if (outside) {
                    resized_str.replace(resized_str.length() - 1, 0, accent_symbol);
                }
                final_str = final_str + resized_str + "\n";
            } else {
                str = std::string(indent_size, ' ') + str;
                int new_len = (len - str.length());
                str.insert(str.length(), new_len, ' ');
                if (inside) {
                    str.replace(0, 1, accent_symbol);
                }
                if (outside) {
                    str.replace(len - 1, 1, accent_symbol);
                }
                std::string color_str = "\033[" + color + "m";
                final_str = final_str + str;
                if (inside) {
                    final_str.insert(1, color_str);
                } else {
                    final_str.insert(0, color_str);
                }
                if (outside) {
                    final_str.insert(final_str.length() - 1, "\033[0m");
                } else {
                    final_str.insert(final_str.length() - 1, "\033[0m");
                }
                return final_str;
                break;
            }
        }
    }
};

#endif
