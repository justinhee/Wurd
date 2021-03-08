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
            //convert s to lowercase
            size_t len = s.size();
            for(int i = 0; i < len; i++)
            {
                s[i] = toupper(s[i]);
            }
            insert(s);

        }
        return true;
    }
    return false;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}
