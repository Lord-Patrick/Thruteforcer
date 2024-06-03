#include "IncreasableString.h"
#include <atomic>
#include <iostream>
#include <list>
#include <ostream>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <string>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iomanip> // for std::setw and std::setfill
#include <sstream> // for std::stringstream

void force(struct forcer_param);

std::atomic<bool> stop = false;
std::atomic<bool> found = false;

std::string target_hash = "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824";
unsigned char target_hash_chars[SHA256_DIGEST_LENGTH];

struct forcer_param {
    std::string prefix;
    int length;
};

bool sha256(const char *str, unsigned char buffer[SHA256_DIGEST_LENGTH], unsigned int strlen) {
    if (str[strlen] != '\0') {
        return false;
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        return false;
    }

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr)) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    if (1 != EVP_DigestUpdate(mdctx, str, strlen)) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    unsigned int length = 0;
    if (1 != EVP_DigestFinal_ex(mdctx, buffer, &length)) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    EVP_MD_CTX_free(mdctx);
    return length == SHA256_DIGEST_LENGTH;
}

void string_to_uchar_vector(const std::string& str, unsigned char hash[SHA256_DIGEST_LENGTH]) {
    for (size_t i = 0; i < str.length(); i += 2) {
        std::string byteString = str.substr(i, 2);
        unsigned char byte = (unsigned char)strtol(byteString.c_str(), nullptr, 16);
        hash[i / 2] = byte;
    }
}

void listen_for_quit() {
    std::cin.ignore();
    stop.store(true);
}

int main() {
    string_to_uchar_vector(target_hash, target_hash_chars);

    std::list<std::thread> threads;
    IncreasableString prefix(1);
    struct forcer_param param;

    std::thread listener(listen_for_quit);

    for (int i = 1; i <= 7; i += 1) {
        std::cout << "Forcing with length = " << i + 1 << "\n";
        param.length = i;
        do {
            param.prefix = prefix.get_string();
            threads.emplace_back(force, param);
            prefix += 1;
        } while (!prefix.endofword);

        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        if (found.load() || stop.load()) {
            break;
        }
    }

    listener.join();
    return 0;
}

void force(struct forcer_param params) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    IncreasableString ic(params.length);

    do {
        if (found.load()) break;

        std::string tb_hashed = params.prefix + ic.get_string();
        bool success = sha256(tb_hashed.c_str(), hash, tb_hashed.length());

        if (success) {
            bool equal = std::equal(std::begin(hash), std::end(hash), std::begin(target_hash_chars));
            if (equal) {
                    found.store(true);

                std::stringstream ss;
                for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
                    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                }
                std::string hash_str = ss.str();

                std::cout << "SHA-256 hash of \"" << tb_hashed << "\" is: " << hash_str << std::endl;
            }
        } else {
            std::cout << "Fehler. " << std::endl;
        }
        ic += 1;
    } while (!ic.endofword && !stop.load());
}