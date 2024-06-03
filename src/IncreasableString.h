#ifndef INCREASABLESTRING_H
#define INCREASABLESTRING_H

#include <array>
#include <string>

class IncreasableString {
    public:
        IncreasableString(int len);
        
        void set_position(const unsigned int pos, const char value);
        std::string get_string();
        friend void operator+=(IncreasableString&, int);

        bool endofword = false;

        ~IncreasableString();
    private:
        int len;
        char *stringData;
        
        void set_endofword();

        
        constexpr static std::array<char, 72> letters = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
            'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 
            'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
            'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
            'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
            'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
            'W', 'X', 'Y', 'Z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', ' ', '!',
            '+', '-', '*', '/', '#', '~', '?', '%',
            // '', '', '', '', '', '', '', '',
        };
        
        /*
        constexpr static std::array<char, 14> letters = {
            'H', 'a', 'l', 'o', 'g', 'h', 'W', 'e', 
            'l', 't', 'f', ' ', 'i', 'j', 
        };
        */
};

#endif