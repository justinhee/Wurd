#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>



StudentSpellCheck::Trie::Trie(char c)
: m_data(c)
{
   for(int i = 0; i < 27; i++)
       m_children[i] = nullptr;
   m_isWord = false;
}

void StudentSpellCheck::Trie::clearTrie()
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

void StudentSpellCheck::insert(std::string word)
{
    int n = word.size();
    Trie* node = m_root;
    int index;
    for(int i = 0; i < n; i++)
    {
        int index = word[i] == '\'' ? 26 : tolower(word[i]) - 'a';
        
        if(node->m_children[index] == nullptr)
        {
            node->m_children[index] = new Trie(tolower(word[i]));
        }
            node = node->m_children[index];
    }
    node->m_isWord = true;
}

bool StudentSpellCheck::isValidWord(std::string word)
{

    //check for apostrophe
    //TODO: add spelling suggestions
    Trie* node = m_root;
    for(int i = 0; i < word.size(); i++)
    {
        int index = word[i] == '\'' ? 26 : tolower(word[i]) - 'a';
        if(node->m_children[index] != nullptr)
            node = node->m_children[index];
        else
            return false;
    }
    return node->m_isWord;

}



SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}
StudentSpellCheck::StudentSpellCheck() : m_root(new Trie('\0'))
{}
StudentSpellCheck::~StudentSpellCheck() {
	// TODO
    m_root->clearTrie();
    delete m_root;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	// TODO
    std::ifstream infile(dictionaryFile);
    if(infile)
    {
        m_root->clearTrie();
        std::string s;
        while(getline(infile, s))
        {
            insert(s);
        }
        return true;
    }
    return false;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    //TODO: add spelling suggestions
	if(isValidWord(word))
        return true;
    else
    {
        suggestions.push_back(word);
        return false;
    }
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
    problems.clear();
    for(int i = 0; i < line.size(); i ++)
    {
        int start = i;
        int end = i;
        //if it's a valid character
        if(isalpha(line[i]) || line[i] == '\'')
        {
            std::string temp;
            temp += line[i];
            //check if the next character is also valid and not out of bounds
            while(i + 1 < line.size() && (isalpha(line[i+1]) || line[i+1] == '\''))
            {
                end++;
                i++;
                temp += line[i];
            }
            if(!isValidWord(temp))
            {
                Position misspelledWord;
                misspelledWord.start = start;
                misspelledWord.end = end;
                problems.push_back(misspelledWord);
            }
        }
        
    }
    
}
