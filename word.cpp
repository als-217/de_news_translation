#include "word.hpp"

#include <iostream>

Word::Word(PoS pos, std::string word, std::string defn, std::string tags, std::string form_of, std::string special_char) : pos_(pos), word_str_(word), form_of_(form_of) {
    if (special_char.empty()) {
        spcl_char_ = false;
    }
    spcl_char_ = std::stoi(special_char);
    defn_ = ReadDefn(defn);
    tags_ = ReadTags(tags);
}

std::vector<std::string> Word::ReadDefn(const std::string& defn) {
    std::vector<std::string> defn_vector;
    unsigned int def_count = 0;
    for (unsigned int i = 0; i < defn.size(); ++i) {
        if (defn.at(i) == '~') { def_count++; }
    }
    size_t last = 0;
    size_t next = 0;
    if (def_count == 0) {
        return {defn};
    } else { defn_vector.reserve(def_count); }
    while ((next = defn.find('~', last)) != std::string::npos) {
        defn_vector.push_back(defn.substr(last, next-last));
        last = next + 1;
    }
    return defn_vector;
}

std::vector<std::string> Word::ReadTags(const std::string& tags) {
    unsigned int tag_count = 0;
    std::vector<std::string> tag_vector;
    for (unsigned int i = 0; i < tags.size(); ++i) {
        if (tags.at(i) == '~') { tag_count++; }
    }
    tag_vector.reserve(tag_count);
    size_t last = 0;
    size_t next = 0;
    while ((next = tags.find('~', last)) != std::string::npos) {
        std::string temp = tags.substr(last, next-last);
        if (temp != "0") {
            tag_vector.push_back(temp);
        } else { tag_vector.push_back(""); }
        last = next + 1;
    }
    return tag_vector;
}

const std::vector<std::string>& Word::Tags() const { return tags_; }
const std::vector<std::string>& Word::Defn() const { return defn_; }
const std::string& Word::GetWord() const { return word_str_; }
PoS Word::Pos() const { return pos_; }
std::string Word::AltForm() const { return form_of_; }

void Word::Print(int defn_idx) const {
    std::cout << "Word: " << word_str_ << std::endl;
    std::cout << "Definition: " << std::endl;
    if (defn_idx == -1) {
        for (unsigned int i = 0; i < defn_.size(); ++i) {
            std::cout << defn_.at(i) << std::endl;
        }
    } else { std::cout << defn_.at(defn_idx) << std::endl; }
}

std::vector<int> Word::Convert(const std::string& sub_str) {
    std::map<std::string, int> special_chars = {{"Ä", 128}, {"Ö", 129}, {"Ü", 130}, {"ä", 131}, {"ö", 132}, {"ü", 133}, {"ß", 134}};
    std::vector<int> str_as_int_vec;
    for(size_t i = 0; i < sub_str.length();) {
        int cplen = 1;
        if((sub_str[i] & 0xf8) == 0xf0) cplen = 4;
        else if((sub_str[i] & 0xf0) == 0xe0) cplen = 3;
        else if((sub_str[i] & 0xe0) == 0xc0) cplen = 2;
        if((i + cplen) > sub_str.length()) cplen = 1;
        std::string letter = sub_str.substr(i, cplen);
        try {
            str_as_int_vec.push_back(special_chars.at(letter));
        } catch (const std::out_of_range& e) {
            char letter = sub_str.at(i);
            str_as_int_vec.push_back(letter);
        } 
        i += cplen;
    }
    return str_as_int_vec;
}

int Word::CompareTo(const Word& word) const {
    int k = 0;
    std::vector<int> lhs = Convert(word_str_);
    std::vector<int> rhs = Convert(word.word_str_);
    int len1 = lhs.size();
    int len2 = rhs.size();
    int limit = 0;
    if (len1 < len2) { limit = len1; } else { limit = len2; }
    while (k < limit) {
        int c1 = lhs.at(k);
        int c2 = rhs.at(k);
        if (c1 != c2) {
            return c1 - c2;
        }
        k++;
    }
    return len1 - len2;
}

bool Word::operator==(const Word& word) {
    return ((word_str_.compare(word.word_str_) == 0) && (pos_ == word.pos_));
}
bool Word::operator<(const Word& word) {
    if (this->spcl_char_ == false && word.spcl_char_ == false) {
        return (word_str_.compare(word.word_str_) < 0);
    }
    return (this->CompareTo(word) < 0);
}
bool Word::operator<=(const Word& word) {
    if (this->spcl_char_ == false && word.spcl_char_ == false) {
        return (word_str_.compare(word.word_str_) <= 0);
    }
    return (this->CompareTo(word) <= 0);
}
bool Word::operator>=(const Word& word) {
    if (this->spcl_char_ == false && word.spcl_char_ == false) {
        return (word_str_.compare(word.word_str_) >= 0);
    }
    return (this->CompareTo(word) >= 0);
}
bool Word::operator>(const Word& word) {
    if (this->spcl_char_ == false && word.spcl_char_ == false) {
        return (word_str_.compare(word.word_str_) > 0);
    }
    return (this->CompareTo(word) > 0);
}
bool Word::operator!=(const Word& word) {
    return ((word_str_.compare(word.word_str_) != 0) || (pos_ == word.pos_));
}
