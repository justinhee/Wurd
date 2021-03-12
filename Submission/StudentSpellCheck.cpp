#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>



//creates a trie node with the given data, initializes its children to nullptr
StudentSpellCheck::Trie::Trie(char c)
: m_data(c)
{
   for(int i = 0; i < 27; i++)
       m_children[i] = nullptr;
   m_isWord = false;
}

//empties the trie
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

//inset a new word into the trie
void StudentSpellCheck::insert(std::string word)
{
    int n = word.size();
    Trie* node = m_root;
    for(int i = 0; i < n; i++)
    {
        //map apostrophe to index 26
        int index = word[i] == '\'' ? 26 : tolower(word[i]) - 'a';
        //if the next letter doesn't exist, add it
        if(node->m_children[index] == nullptr)
        {
            node->m_children[index] = new Trie(tolower(word[i]));
        }
            node = node->m_children[index];
    }
    //the last node of a word should be marked
    node->m_isWord = true;
}

bool StudentSpellCheck::isValidWord(std::string word)
{

    //check for apostrophe
    Trie* node = m_root;
    //for each letter in word
    for(int i = 0; i < word.size(); i++)
    {
        //map apostrophe to index 26
        int index = word[i] == '\'' ? 26 : tolower(word[i]) - 'a';
        //if the letter is in the trie, go to the next layer
        if(node->m_children[index] != nullptr)
            node = node->m_children[index];
        else
            return false;
    }
    //return if it's a word, because you don't want to misidentify substrings
    return node->m_isWord;

}



SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}
//when you create a spellcheck, initialize the trie
StudentSpellCheck::StudentSpellCheck() : m_root(new Trie('\0'))
{}

//when you delete a spellcheck, delete it's data
StudentSpellCheck::~StudentSpellCheck() {
	// TODO
    m_root->clearTrie();
    delete m_root;
}
//add a dictionary file
bool StudentSpellCheck::load(std::string dictionaryFile) {
	// TODO
    std::ifstream infile(dictionaryFile);
    if(infile)
    {
        //empty the trie
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
    //if it's a real word return true
	if(isValidWord(word))
        return true;
    //if it's a fake word search for suggestions
    else
    {
        suggestions.clear();
        int count = 0;
        //go through each index and allow that index to be any letter, and check if it's a word
        //stop when you get to max_suggestions or the last index
        for(int i = 0; i < word.size() && count < max_suggestions; i++)
        {
            std::string potentialWord = word;
            for(int j = 0; j < 27; j++)
            {
                //at index 26 treat it as an apostrophe
                potentialWord[i] = j == 26 ? '\'' : 'a' + j;
                if(isValidWord(potentialWord))
                {
                    suggestions.push_back(potentialWord);
                    count++;
                }
            }
        }
        return false;
    }
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	//clear problems
    problems.clear();
    
    //for each char in the line
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
            //get each chunk of valid characters
            while(i + 1 < line.size() && (isalpha(line[i+1]) || line[i+1] == '\''))
            {
                end++;
                i++;
                temp += line[i];
            }
            //if the chunk isn't a word, then add it's beginning and end to the problems vector
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
