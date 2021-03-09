#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_col(0), m_row(0) {
	// TODO
     m_lines.push_back("");
     m_currentRow = m_lines.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
    clearLines();
    //gotta clear undo
}

void StudentTextEditor::clearLines()
{
    for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end();)
    {
        p = m_lines.erase(p);
    }
}

bool StudentTextEditor::load(std::string file) {
	// TODO
    std::ifstream infile(file);
    if(infile)
    {
        clearLines();
        m_currentRow = m_lines.begin();
        std::string nextLine;
        while(getline(infile, nextLine))
        {
            if(nextLine[nextLine.size()-1] == '\r')
                nextLine = nextLine.substr(0,nextLine.size()-1);
            m_lines.insert(m_currentRow, nextLine);
        }
        m_currentRow = m_lines.begin();
        m_row = 0;
        m_col = 0;
        return true;
    }
    return false;
}

bool StudentTextEditor::save(std::string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	// TODO
}

void StudentTextEditor::move(Dir dir) {
	// TODO
    switch(dir)
    {
        case Dir::UP:
            if(m_row > 0)
            {
                m_row--;
                m_currentRow--;
                int n = m_currentRow->size();
                m_col = m_col > n ? n : m_col;
            }
            break;
        case Dir::DOWN:
            //TODO: is size O(1)?
            if(m_row+1 < m_lines.size())
            {
                m_row++;
                m_currentRow++;
                int n = m_currentRow->size();
                m_col = m_col > n ? n : m_col;
            }
            break;
        case Dir::LEFT:
            if(m_col > 0)
            {
                m_col--;
            }
            else if(m_col == 0 && m_row > 0)
            {
                m_row--;
                m_currentRow--;
                m_col = m_currentRow->size();
            }
            break;
        case Dir::RIGHT:
            if(m_col < m_currentRow->size())
            {
                m_col++;
            }
            else if(m_col == m_currentRow->size() && m_row+1 < m_lines.size())
            {
                m_col = 0;
                m_row++;
                m_currentRow++;
            }
            break;
        case Dir::HOME:
            m_col = 0;
            break;
        case Dir::END:
            m_col = m_currentRow->size();
            break;
        default:
            break;
            
            
    }
}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
	// TODO: add undo
    (*m_currentRow).insert((*m_currentRow).begin()+m_col, ch);
    
    m_col++;
}

void StudentTextEditor::enter() {
	// TODO: add undo
    if(m_row + 1 == m_lines.size() && m_col == m_currentRow->size())
    {
        m_row++;
        m_col = 0;
        m_lines.push_back("");
        m_currentRow++;
    }
    else
    {
        //TODO: seems very messy!
        std::string temp = m_currentRow->substr(m_col, m_currentRow->size()-m_col);
        *m_currentRow = m_currentRow->substr(0, m_col);
        m_currentRow++;
        m_lines.insert(m_currentRow, temp);
        m_currentRow--;
    }
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col; // TODO
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	// TODO
    //return -1 if passed invalid startRow
    if(startRow < 0 || startRow > m_lines.size())
        return -1;
    
    //empty the lines vector
    lines.clear();
    
    //copy the current row iterator, then move it either up or down to the starting row
    std::list<std::string>::iterator start = m_currentRow;
    for(int i = 0; i < abs(startRow-m_row); i++)
    {
        if(m_row > startRow)
            start--;
        else
            start++;
    }
    
    int count = 0;
    while(start != m_lines.end() && count < numRows)
    {
        lines.push_back((*start));
        start++;
        count++;
    }
    return count;

}

void StudentTextEditor::undo() {
	// TODO
}
