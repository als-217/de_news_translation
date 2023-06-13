#ifndef DE_FLASHCARD_HPP
#define DE_FLASHCARD_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "word.hpp"

class Flashcard {
    public:
        Flashcard(PoS pos, std::string word, std::string original, std::vector<std::string> defn);
        Flashcard(Word word, std::string original);
        const std::string& GetWord();
        const std::string& GetOriginal();
        const std::vector<std::string>& GetDefn();
        PoS GetPos();
    private:
        std::string word_;
        std::string original_;
        std::vector<std::string> defn_ = {};
        PoS pos_;
};

#endif