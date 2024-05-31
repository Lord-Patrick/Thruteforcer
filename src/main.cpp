#include "IncreasableString.h"
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <pthread.h>
#include <string>
#include <iomanip> // for std::setw and std::setfill
#include <sstream> // for std::stringstream
#include <openssl/sha.h> // for SHA-256 functions
#include <array>
#include <thread>
#include <mutex>
#include <vector>

void force(struct forcer_param);

std::mutex m_found = std::mutex();
bool found = false;

std::string target = std::string("334d5df7e9a67ea3370664ba4f50de08fbe1bb1f0b2c3d0bbd1a72ea5bbd4e91");

struct forcer_param {
    std::string prefix;
    int length;
};

bool sha256(const char *str, unsigned char buffer[SHA256_DIGEST_LENGTH], unsigned int strlen) {
    if (str[strlen] != 0) {
        return false;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen);
    SHA256_Final(buffer, &sha256);

    return true;
}


int main() {
    /*
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string hallo = "Hallo! ";
    sha256(hallo.c_str(), hash, hallo.size() * sizeof(char));

    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    std::string hashValue = ss.str();
    
    std::cout << hashValue << std::endl;
    
    return 0;
    */


    std::list<std::thread> threads = std::list<std::thread>();
    IncreasableString prefix = IncreasableString(1);
    struct forcer_param param;

    for (int i = 1; i <= 7; i += 1) {
        std::cout << "Forcing with length = " << i + 1 << "\n";
        param.length = i;
        do {
            param.prefix =  prefix.get_string();
            threads.push_back(std::thread(force, param));
            prefix += 1;
        } while (!prefix.endofword);

        while (threads.size() > 0) {
            threads.back().join();
            threads.pop_back();
        }
        if (found) {
            break;
        }
    }

    return 0;
}


void force(struct forcer_param params) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    IncreasableString ic = IncreasableString(params.length);

    do {
        m_found.lock();
        if (found) {
            m_found.unlock();
            break;
        } else {
            m_found.unlock();
        }
        std::string tbHashed = params.prefix + ic.get_string();
        bool success = sha256(tbHashed.c_str(), hash, tbHashed.length());
        
        std::stringstream ss;
        
        if (success) {
            for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
            }
            std::string hashValue = ss.str();
            if (hashValue.compare(target) == 0) {
                m_found.lock();
                found = true;
                m_found.unlock();
                
                std::cout << "SHA-256 hash of \"" << tbHashed << "\" is: " << hashValue << std::endl;
            }
        } else {
            std::cout << "Fehler. " << std::endl;
        }
        ic += 1;
    } while (!ic.endofword);
}