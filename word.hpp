#ifndef WORD_HPP
#define WORD_HPP

#include <utility>
#include <map>
#include <string>
#include <vector>
#include <sstream>

enum class PoS { kMultiple, kVerb, kNoun, kAdv, kAdj, kPrep };

class Word {
    public:
        Word() = default;
        Word(PoS pos, std::string word, std::string defn, std::string tags, std::string form_of, std::string special_char = "");
        PoS Pos() const;
        const std::string& GetWord() const;
        const std::vector<std::string>& Tags() const;
        const std::vector<std::string>& Defn() const;
        std::string AltForm() const;
        void Print(int defn_idxs = -1) const;
        bool operator==(const Word& word);
        bool operator<(const Word& word);
        bool operator<=(const Word& word);
        bool operator>=(const Word& word);
        bool operator>(const Word& word);
        bool operator!=(const Word& word);
        static std::vector<int> Convert(const std::string& sub_str);
    private:
        std::vector<std::string> ReadTags(const std::string& tags);
        std::vector<std::string> ReadDefn(const std::string& defn);
        int CompareTo(const Word& word) const;
        PoS pos_ = PoS::kMultiple;
        std::string word_str_;
        std::string form_of_ = "0";
        bool spcl_char_ = false;
        std::vector<std::string> defn_;
        std::vector<std::string> tags_;
};

#endif
