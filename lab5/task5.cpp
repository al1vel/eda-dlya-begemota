#include <iostream>
#include <string>
#include <regex>
#include <fstream>

bool isVariable(const std::string& variable) {
    std::regex mask("^[a-zA-Z_][a-zA-Z0-9_]*$");
    return std::regex_match(variable, mask);
}

std::string cdecl_translate(const std::string& declaration) {
    try {
        std::smatch match;
        std::regex trash(R"(\s*;\s*)");
        std::regex base_mask(
                R"(^\s*(int|char|float|double|void)\s+((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*(\s*;\s*)?$)"
        );
        std::regex func_mask(
                R"(^\s*(int|char|float|double|void)\s+\(((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*\)(\(.*\))?(\s*;\s*)?$)"
        );

        if (!std::regex_match(declaration, match, base_mask) && !std::regex_match(declaration, match, func_mask)) {
            return "Syntax error in declaration: " + declaration;
        }

        std::string base_type = match[1].str();
        std::string pointer = match[3].str();
        std::string identifier = match[4].str();
        std::string array_part = match[5].str();
        std::string func_part = match[6].str();

        if (!isVariable(identifier)) {
            return "Syntax error at position " + std::to_string(declaration.find(identifier));
        }

        std::string description = "declare " + identifier + " as ";

        size_t pointerCount = std::count(pointer.begin(), pointer.end(), '*');
        for (size_t i = 0; i < pointerCount; ++i) {
            description += "pointer to ";
        }

        if (!array_part.empty()) {
            std::regex arrayRegex(R"(\[([0-9]*)\])");
            std::sregex_iterator begin(array_part.begin(), array_part.end(), arrayRegex);
            std::sregex_iterator end;

            for (auto i = begin; i != end; ++i) {
                std::string size = (*i)[1].str();
                if (size.empty()) {
                    description += "array of ";
                } else {
                    description += "array of " + size + " elements of ";
                }
            }
        }

        description += base_type;
        if (!func_part.empty() && !(std::regex_match(func_part, trash))) {
            description += " getting as args: " + func_part;
        }

        return description;

    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Provide one input file" << std::endl;
        return -1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << cdecl_translate(line) << std::endl;
    }
    file.close();
    return 0;
}