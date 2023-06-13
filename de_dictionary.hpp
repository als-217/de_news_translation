#ifndef DE_DICTIONARY_HPP
#define DE_DICTIONARY_HPP

#include <cstddef>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <set>

#include "word.hpp"
#include "flashcard.hpp"

class DeDictionary {
    public:
        DeDictionary(const std::string& path, PoS pos = PoS::kMultiple);
        DeDictionary(std::vector<Word> words, PoS pos = PoS::kMultiple);
        DeDictionary(const DeDictionary& dictionary, int start = -1, int end = 0);
        DeDictionary& operator=(const DeDictionary& dictionary);
        ~DeDictionary() = default;
        Word BinarySearch(Word to_find);
        Word BinarySearch(std::string to_find, PoS pos = PoS::kMultiple);
        std::vector<Word> Dict() const;
        const Word& DictAt(size_t index) const;
        const size_t& WordCount() const;
        PoS Pos() const;
        void Sort(int start = -1, int end = 0);
        Flashcard SearchDictionary(std::pair<std::string, std::string> word, std::string pos);
        Flashcard SearchAdjectives(std::pair<std::string, std::string> word);
        Flashcard SearchAdverbs(std::pair<std::string, std::string> word);
        Flashcard SearchPrepositions(std::pair<std::string, std::string> word);
        Flashcard SearchNouns(std::pair<std::string, std::string> word);
        Flashcard SearchVerbs(std::pair<std::string, std::string> word);
        Flashcard SearchPhraseVerbs(const std::vector<std::pair<std::string, std::string>>& phrase);
    private:
        int Partition(int start, int end);
        std::vector<std::vector<std::string>> ReadData(const std::string& path, PoS pos = PoS::kMultiple);
        std::vector<Word> dict_;
        PoS pos_ = PoS::kMultiple;
        size_t word_count_ = 0;
};

#endif