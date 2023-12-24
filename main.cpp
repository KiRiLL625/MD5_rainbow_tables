#include <iostream>
#include <filesystem>
#include <fstream>

std::string get_value_from_rainbow_table(std::string& hash) {
    std::filesystem::path path = std::filesystem::current_path() / "rainbow-tables";
    //открыть файл с таблицей
    for(auto& it: std::filesystem::directory_iterator(path)){
        std::ifstream file(it.path());
        std::string line;
        while (std::getline(file, line)) {
            if(line.find(hash) != std::string::npos){
                return line.substr(line.find('\t') + 2, line.size() - line.find('\t') - 3);
            }
        }
    }
    return {};
}

int main() {
    std::string hash;
    std::cin >> hash;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::string value = get_value_from_rainbow_table(hash);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << value << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
    << "ms" << std::endl;
    return 0;
}
