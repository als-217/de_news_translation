#include "de_dictionary.hpp"

#include <iostream>

std::vector<std::vector<std::string>> DeDictionary::ReadData(const std::string& path, PoS pos) {
    std::ifstream file (path);
    std::vector<std::vector<std::string>> dict;
    std::string line;
    int current;
    size_t line_num = 0;
    std::vector<std::string> word(6);
    while(std::getline(file, line)) {
        if (line_num % 6 == 0) {
            current = std::stoi(line);
            word.clear();
            if (current == static_cast<int>(pos) || pos == PoS::kMultiple) {
                word_count_++;
            }
        }
        if (current == static_cast<int>(pos) || pos == PoS::kMultiple) {
            word.push_back(line);
        }
        line_num++;
        if (line_num % 6 == 0 && (current == static_cast<int>(pos) || pos == PoS::kMultiple)) { dict.push_back(word); }
    }
    return dict;
}

DeDictionary::DeDictionary(const std::string& path, PoS pos) : pos_(pos) {
    std::vector<std::vector<std::string>> str_dict = ReadData(path, pos);
    dict_.reserve(word_count_);
    for (unsigned int word = 0; word < str_dict.size(); ++word) {
        Word new_word(pos, str_dict.at(word).at(1), 
                        str_dict.at(word).at(4), str_dict.at(word).at(5), str_dict.at(word).at(3), str_dict.at(word).at(2));
        dict_.push_back(new_word);
    }
}

DeDictionary::DeDictionary(std::vector<Word> words, PoS pos) : dict_(words), word_count_(words.size()) {
    pos_ = pos;
    for (auto const &word : words) {
        if (word.Pos() != pos) {
            pos_ = PoS::kMultiple;
            break;
        }
    }
}

DeDictionary::DeDictionary(const DeDictionary& dictionary, int start, int end) {
    if ((start == -1 && end != 0) || (start != -1 && end == 0)) { throw std::invalid_argument("Both start and end must be passed or neither."); }
    if (start == -1 && end == 0) { end = dict_.size(); }
    pos_ = dictionary.pos_;
    dict_.reserve(end - start);
    for (int i = start; i < end; ++i) {
        dict_.push_back(dictionary.dict_.at(i));
    }
    word_count_ = dict_.size();
}

DeDictionary& DeDictionary::operator=(const DeDictionary& dictionary) {
    if (this == &dictionary) { return *this; }
    dict_.clear();
    pos_ = dictionary.pos_;
    dict_.reserve(dictionary.dict_.size());
    for (unsigned int i = 0; i < dict_.capacity(); ++i) {
        dict_.push_back(dictionary.dict_.at(i));
    }
    word_count_ = dict_.size();
    return *this;
}

PoS DeDictionary::Pos() const { return pos_; }

std::vector<Word> DeDictionary::Dict() const { return dict_; }

const Word& DeDictionary::DictAt(size_t index) const { return dict_.at(index); }

const size_t& DeDictionary::WordCount() const { return word_count_; }

Word DeDictionary::BinarySearch(Word to_find) {
    size_t first = 0;
    size_t last = dict_.size() - 1;
    size_t mid = dict_.size() / 2;
    while(first <= last) {
        mid = static_cast<size_t>((first + last) / 2);
        if (dict_.at(mid).GetWord().compare(to_find.GetWord()) == 0) {
            return dict_.at(mid);
        }
        Word test = dict_.at(mid);
        if (to_find < dict_.at(mid)) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }
    Word not_found;
    return not_found;
}

Word DeDictionary::BinarySearch(std::string to_find, PoS pos) {
    size_t first = 0;
    size_t last = dict_.size() - 1;
    size_t mid = dict_.size() / 2;
    while(first <= last) {
        mid = static_cast<size_t>((first + last) / 2);
        std::string test = dict_.at(mid).GetWord();
        if ((dict_.at(mid).GetWord().compare(to_find) == 0) && ((dict_.at(mid).Pos() == pos) || (pos == PoS::kMultiple))) {
            return dict_.at(mid);
        }
        std::vector<int> to_find_vals = Word::Convert(to_find);
        std::vector<int> current_vals = Word::Convert(dict_.at(mid).GetWord());
        int dif = 0;
        int idx = 0;
        int limit = std::min(to_find_vals.size(), current_vals.size());
        while (idx < limit) {
            if (to_find_vals.at(idx) != current_vals.at(idx)) {
                dif = to_find_vals.at(idx) - current_vals.at(idx);
                break;
            }
            idx++;
        }
        if (idx == limit) { dif = to_find_vals.size() - current_vals.size(); }
        if (dif < 0) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }
    Word not_found;
    return not_found;
}

int DeDictionary::Partition(int start, int end) {
    Word pivot = dict_.at(start);
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (dict_.at(i) <= pivot) { count++; }
    }
    int pivotIndex = start + count;
    std::swap(dict_.at(pivotIndex), dict_.at(start));
    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
        while (dict_.at(i) <= pivot) {
            i++;
        }
        while (dict_.at(j) > pivot) {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(dict_.at(i++), dict_.at(j--));
        }
    }
    return pivotIndex;
}

void DeDictionary::Sort(int start, int end) {
    if (end >= static_cast<int>(dict_.size())) { throw std::invalid_argument("End must be lower than the dictionary's size."); }
    if (start == -1 && end == 0) {
        start = 0;
        end = dict_.size() - 1;
    }
    if (start >= end) { return; }
    if ((start == -1 && end != 0) || (start != -1 && end == 0)) { throw std::invalid_argument("Both start and end must be passed or neither."); }
    int p = Partition(start, end);
    Sort(start, p - 1);
    Sort(p + 1, end);
}

Flashcard DeDictionary::SearchNouns(std::pair<std::string, std::string> word) {
    std::string lemma = word.first;
    Word noun = BinarySearch(lemma, PoS::kNoun);
    if (noun.GetWord().empty()) { throw std::runtime_error("Noun not found"); }
    if (noun.AltForm() != "0") { noun = BinarySearch(noun.AltForm(), PoS::kNoun); }
    std::string gender = "das";
    for (auto const& tag : noun.Tags()) {
        if (tag == "masculine") {
            gender = "der";
        } else if (tag == "feminine") {
            gender = "die";
        }
    }
    return Flashcard(PoS::kNoun, gender + noun.GetWord(), word.second, noun.Defn());
}

Flashcard DeDictionary::SearchAdjectives(std::pair<std::string, std::string> word) {
    std::string lemma = word.first;
    Word adj = BinarySearch(lemma, PoS::kAdj);
    if (adj.GetWord().empty()) { throw std::runtime_error("Adjective not found"); }
    if (adj.AltForm() != "0") { adj = BinarySearch(adj.AltForm(), PoS::kAdj); }
    return Flashcard(adj, word.second);
}

Flashcard DeDictionary::SearchAdverbs(std::pair<std::string, std::string> word) {
    std::string lemma = word.first;
    Word adv = BinarySearch(lemma, PoS::kAdv);
    if (adv.GetWord().empty()) { throw std::runtime_error("Adverb not found"); }
    if (adv.AltForm() != "0") { adv = BinarySearch(adv.AltForm(), PoS::kAdv); }
    return Flashcard(adv, word.second);
}

Flashcard DeDictionary::SearchPrepositions(std::pair<std::string, std::string> word) {
    std::string lemma = word.first;
    Word prep = BinarySearch(lemma, PoS::kPrep);
    if (prep.GetWord().empty()) { throw std::runtime_error("Preposition not found"); }
    if (prep.AltForm() != "0") { prep = BinarySearch(prep.AltForm(), PoS::kPrep); }
    return Flashcard(prep, word.second);
}

Flashcard DeDictionary::SearchVerbs(std::pair<std::string, std::string> word) {
    std::string lemma = word.first;
    Word verb = BinarySearch(lemma, PoS::kVerb);
    if (verb.GetWord().empty()) { throw std::runtime_error("Verb not found"); }
    if (verb.AltForm() != "0") { verb = BinarySearch(verb.AltForm(), PoS::kVerb); }
    return Flashcard(verb, word.second);
}

Flashcard DeDictionary::SearchPhraseVerbs(const std::vector<std::pair<std::string, std::string>>& phrase) {
    std::pair<std::string, std::string> word = phrase.at(1);
    Flashcard verb = SearchVerbs(word);
    std::pair<std::string, std::string> prf = phrase.at(0);
    std::pair<std::string, std::string> prep = phrase.at(2);
    if (!prf.first.empty() && !prep.first.empty()) {
        std::vector<std::string> meanings = {}; 
        for (unsigned int i = 0; i < verb.GetDefn().size(); ++i) {
            if (verb.GetDefn().at(i).find("reflexive") != std::string::npos 
                && verb.GetDefn().at(i).find(prep.first) != std::string::npos) {
                    meanings.push_back(verb.GetDefn().at(i));
                } 
        }
        return Flashcard(PoS::kVerb, prf.first + word.first + prep.first, prf.second + word.second + prep.second, meanings);
    }
    if (!prf.first.empty() && prep.first.empty()) {
        std::vector<std::string> meanings = {};
        for (unsigned int i = 0; i < verb.GetDefn().size(); ++i) {
            if (verb.GetDefn().at(i).find("reflexive") != std::string::npos) {
                meanings.push_back(verb.GetDefn().at(i));
            } 
        }
        return Flashcard(PoS::kVerb, prf.first + word.first, prf.second + word.second, meanings);
    }
    if (prf.first.empty() && !prep.first.empty()) {
        std::vector<std::string> meanings = {};
        for (unsigned int i = 0; i < verb.GetDefn().size(); ++i) {
            if (verb.GetDefn().at(i).find(prep.first) != std::string::npos) {
                meanings.push_back(verb.GetDefn().at(i));
            } 
        }
        return Flashcard(PoS::kVerb, word.first + prep.first, word.second + prep.second, meanings);
    }
    return verb;
}

Flashcard DeDictionary::SearchDictionary(std::pair<std::string, std::string> word, std::string pos) {
    std::set<std::string> pos_set = {"AUX", "VERB", "ADJ", "NOUN", "ADV", "ADP"};
    if (pos_set.find(pos) == pos_set.end()) { throw std::invalid_argument("POS not supported."); }
    if (pos == "AUX") { 
        Word found = Word(PoS::kVerb, "sein", "to be", "", "0", "0");
        return Flashcard(found, word.second);
    }
    if (pos == "VERB") {
        return SearchVerbs(word);
    }
    if (pos == "NOUN") {
        return SearchNouns(word);
    }
    if (pos == "ADJ") {
        return SearchAdjectives(word);
    }
    if (pos == "ADV") {
        return SearchAdverbs(word);
    }
    return SearchPrepositions(word);
}

