#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct Trie
    {
    public:
        Trie(char c) : m_data(c)
        {
            for(int i = 0; i < 27; i++)
                m_children[i] = nullptr;
            m_isWord = false;
        }
        void clearTrie()
        {
            for(int i = 0; i < 27; i++)
            {
                if(m_children[i] != nullptr)
                {
                    m_children[i]->clearTrie();
                    delete m_children[i];
                }
            }
        }
        char m_data;
        bool m_isWord;
        Trie* m_children[];
    };
    Trie* m_root;
    void insert(std::string word)
    {
        Trie* node = m_root;
        int index;
        for(int i = 0; i != '\0'; i++)
        {
            index = word[i] - 'a';
            if(node->m_children[index] == nullptr)
            {
                node->m_children[index] = new Trie(word[i]);
            }
                node = node->m_children[index];
        }
        node->m_isWord = true;
    }
    
    
};

#endif  // STUDENTSPELLCHECK_H_
