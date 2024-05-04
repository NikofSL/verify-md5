#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> // for std::setw
#include <openssl/md5.h> // OpenSSL MD5 header

// Function to calculate the MD5 hash of a file
std::vector<unsigned char> calculateMD5(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fileName);
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    // Buffer to read the file
    std::vector<char> buffer(4096);
    while (file.good()) {
        file.read(buffer.data(), buffer.size());
        MD5_Update(&md5Context, buffer.data(), file.gcount());
    }

    std::vector<unsigned char> result(MD5_DIGEST_LENGTH);
    MD5_Final(result.data(), &md5Context);

    return result;
}

// Function to convert MD5 hash to hexadecimal string
std::string md5ToHex(const std::vector<unsigned char>& md5) {
    std::ostringstream oss;
    for (auto byte : md5) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
    }
    return oss.str();
}

int main(int argc, char* argv[]) {
    if (argc != 3) { // Expecting the file name and expected hash as arguments
        std::cerr << "Usage: " << argv[0] << " <file_name> <expected_md5_hash>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::string expectedHash = argv[2];

    try {
        // Calculate MD5 hash of the given file
        auto md5Hash = calculateMD5(fileName);
        std::string computedHash = md5ToHex(md5Hash);

        // Verify if the computed hash matches the expected hash
        if (computedHash == expectedHash) {
            std::cout << "MD5 verification successful." << std::endl;
        } else {
            std::cout << "MD5 verification failed." << std::endl;
            std::cout << "Expected: " << expectedHash << std::endl;
            std::cout << "Computed: " << computedHash << std::endl;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
