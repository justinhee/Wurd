#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


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
        if(isalpha(line[i]))
        {
            while(i + 1 < line.size() && isalpha(line[i+1]))
            {
                end++;
                i++;
            }
            if(!isValidWord(line.substr(start, end+1)))
            {
                Position misspelledWord;
                misspelledWord.start = start;
                misspelledWord.end = end;
                problems.push_back(misspelledWord);
            }
        }
    }
    
}
