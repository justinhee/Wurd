#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <cctype>



class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    //private trie implementation
    struct Trie
    {
    public:
        Trie(char c);
        void clearTrie();
        char m_data;
        bool m_isWord;
        Trie* m_children[27];
    };
    Trie* m_root;
    void insert(std::string word);
    bool isValidWord(std::string word);
    
    
    
};

#endif  // STUDENTSPELLCHECK_H_
