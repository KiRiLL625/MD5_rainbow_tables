#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>

void strip(std::string& string) {
    size_t start = string.find_first_not_of(" \t\n\r\f\v");
    size_t end = string.find_last_not_of(" \t\n\r\f\v");
    if (start != std::string::npos && end != std::string::npos) {
        string = string.substr(start, end - start + 1);
    } else {
        string = "";
    }
}

std::string binarySearch(const std::string& target) {
    //std::filesystem::path path = std::filesystem::current_path() / "rainbow_tables";
    std::filesystem::path path = "C:\\Users\\Admin\\CLionProjects\\Task11_cpp\\rainbow-tables";
    int files_left = 1;
    int files_right = 12;
    while (files_left <= files_right) {
        int files_mid = (files_left + files_right) / 2;
        std::string filename = "md5_rainbow_table_part_";
        filename.append(std::to_string(files_mid)).append(".csv");
        std::filesystem::path file_path = path / filename;
        if(!std::filesystem::exists(file_path)) {
            std::cout << "File not found" << std::endl;
            return "";
        }
        std::ifstream stream(file_path, std::ios::in | std::ios::binary);
        if (!stream.is_open()) {
            std::cout << "Error opening file" << std::endl;
            return "";
        }
        std::string string_left;
        stream.seekg({}, std::ios_base::beg);
        std::getline(stream, string_left);
        strip(string_left);
        std::string hashLeft = string_left.substr(1, 32);
        std::string valueLeft = string_left.substr(36, string_left.size() - 37);
        stream.seekg({}, std::ios_base::end);
        auto length = stream.tellg();
        std::string string_right;
        auto string_right_pos = length;
        string_right_pos -= 1;

        std::streampos left = {};
        std::streampos right = length;

        stream.seekg(string_right_pos, std::ios_base::beg);
        while ((string_right_pos > 0) && (std::count_if(string_right.begin(), string_right.end(), [](char & c ){return c == '"';}) != 4)) {
            string_right_pos -= 1;
            stream.seekg(string_right_pos, std::ios_base::beg);
            if (string_right_pos > 0) {
                std::getline(stream, string_right);
            }
        }
        strip(string_right);

        std::string hashRight = string_right.substr(1, 32);
        std::string valueRight = string_right.substr(36, string_right.size() - 37);
        if (target > hashRight) {
            files_left = files_mid + 1;
        } else if (target < hashLeft) {
            files_right = files_mid - 1;
        } else {
            while (left <= right) {
                std::streampos mid = (left + right) / 2;
                stream.seekg(mid, std::ios_base::beg);
                auto midPos = stream.tellg();
                char c;
                stream.get(c);
                while ((midPos > 0) && (c != '\n')) {
                    midPos -= 1;
                    stream.seekg(midPos, std::ios_base::beg);
                    if (midPos > 0) {
                        stream.get(c);
                    }
                }

                std::string str;
                std::getline(stream, str);
                strip(str);
                if (str.size() < 42) {
                    right = midPos;
                    continue;
                }
                std::string hashMid = str.substr(1, 32);
                std::string valueMid = str.substr(36, str.size() - 37);
                if (target > hashMid) {
                    left = stream.tellg();
                } else if (target < hashMid) {
                    right = midPos;
                } else {
                    return valueMid;
                }
            }
        }
    }
    return "";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string res = binarySearch("cf6a143b017924acd1c1b176f44b3564");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << res << std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
    return 0;
}
