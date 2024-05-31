#include "IncreasableString.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>

IncreasableString::IncreasableString(int len) {
    this->len = len;
    this->stringData = (char*) calloc(len + 1, sizeof(char));
}


void IncreasableString::set_position(const unsigned int pos, const char value) {
    if (pos >= this->len) {
        return;
    }
    this->stringData[pos] = value;

    set_endofword();
}

void operator+=(IncreasableString& that, int amount) {
    that.endofword = false;
    int uebertrag = amount;
    for (int i = that.len-1; i >= 0; i -= 1) {
        that.stringData[i] += uebertrag;
        uebertrag = that.stringData[i] / that.letters.size();
        that.stringData[i] = that.stringData[i] % that.letters.size();
    }
    that.set_endofword();
}
void IncreasableString::set_endofword() {
    this->endofword = true;
    for (int i = 0; i < this->len; i += 1) { 
        if (this->stringData[i] != letters.size() -1) {
            this->endofword = false;
            break;
        }
    }
}

std::string IncreasableString::get_string() {
    char string_values[len+1];
    string_values[len] = '\0';
    for (int i = 0; i < this->len; i += 1) {
        string_values[i] = this->letters[this->stringData[i]];
    }

    return std::string(string_values);
}

IncreasableString::~IncreasableString() {
    free(this->stringData);
}