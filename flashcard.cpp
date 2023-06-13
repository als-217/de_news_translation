#include "flashcard.hpp"
#include "word.hpp"

Flashcard::Flashcard(PoS pos, std::string word, std::string original, std::vector<std::string> defn) {
    pos_ = pos;
    word_ = word;
    original_ = original;
    defn_ = defn;
}

Flashcard::Flashcard(Word word, std::string original) : original_(original) {
    pos_ = word.Pos();
    word_ = word.GetWord();
    defn_ = word.Defn();
}

const std::string& Flashcard::GetWord() {
    return word_;
}
const std::string& Flashcard::GetOriginal() {
    return original_;
}
const std::vector<std::string>& Flashcard::GetDefn() {
    return defn_;
}
PoS Flashcard::GetPos() {
    return pos_;
}
