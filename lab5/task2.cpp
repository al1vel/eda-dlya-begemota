#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

enum ret_type_t {
    SUCCESS,
    FILE_OPENING_ERROR,
    ERROR_SAME_FILES,
    ERROR_EMPTY_KEY
};

void HandlingError(int code) {
    switch (code) {
        case FILE_OPENING_ERROR:
            printf("File opening error.\n");
            break;
        case ERROR_SAME_FILES:
            printf("Input and output files are same.\n");
            break;
        case ERROR_EMPTY_KEY:
            printf("Key is empty.\n");
            break;
        default:
            break;
    }
}

bool isSameFiles(const std::string& file1, const std::string& file2) {
    try {
        return std::filesystem::equivalent(file1, file2);
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }
}

class encoder {
    std::vector<std::byte> key;

public:
    explicit encoder(const std::vector<std::byte>& k) : key(k) {}

    int encode(const std::string& inputFile, const std::string& outputFile, bool opType) const {
        if (isSameFiles(inputFile, outputFile)) {
            return ERROR_SAME_FILES;
        }

        std::ifstream in(inputFile, std::ios::binary);
        in.open(inputFile, std::ios::binary);
        if (!in.is_open()) {
            std::cout << inputFile << '\n';
            std::cerr << "File openning error: " << inputFile << ", code: " << errno << '\n';
            return FILE_OPENING_ERROR;
        }

        std::ofstream out(outputFile, std::ios::binary);
        out.open(outputFile, std::ios::binary);
        if (!out) {
            // std::cout << outputFile << '\n';
            return FILE_OPENING_ERROR;
        }

        std::vector<std::byte> S(256);
        for (int i = 0; i < 256; ++i) {
            S[i] = static_cast<std::byte>(i);
        }

        if (key.empty()) {
            return ERROR_EMPTY_KEY;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + std::to_integer<int>(S[i]) + std::to_integer<int>(key[i % key.size()])) % 256;
            swap(S[i], S[j]);
        }

        int i = 0, k = 0;
        char byte;
        while (in.get(byte)) {
            i = (i + 1) % 256;
            j = (j + std::to_integer<int>(S[i])) % 256;
            swap(S[i], S[j]);
            k = std::to_integer<int>(S[(std::to_integer<int>(S[i]) + std::to_integer<int>(S[j])) % 256]);

            byte ^= static_cast<char>(k);

            out.put(byte);
        }

        in.close();
        out.close();
        return SUCCESS;
    }
};

int main() {
    std::vector<std::byte> key = {std::byte(0x51), std::byte(0x23), std::byte(0x45), std::byte(0x6A), std::byte(0x89), std::byte(0xAB), std::byte(0x5D), std::byte(0xEF)};
    encoder encoder(key);

    int code = encoder.encode("in.txt", "encoded.txt", true);
    if (code) {
        HandlingError(code);
        return code;
    }

    code = encoder.encode("encoded.txt", "decoded.txt", true);
    if (code) {
        HandlingError(code);
        return code;
    }
    std::cout << "SUCCESS";
}