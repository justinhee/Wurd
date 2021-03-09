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
    reset();
}


bool StudentTextEditor::load(std::string file) {
	// TODO
    std::ifstream infile(file);
    if(infile)
    {
        reset();

        std::string nextLine;
        while(getline(infile, nextLine))
        {
            if(nextLine[nextLine.size()-1] == '\r')
                nextLine = nextLine.substr(0,nextLine.size()-1);
            m_lines.push_back(nextLine);
        }
        m_currentRow = m_lines.begin();

        return true;
    }
    return false;
}

bool StudentTextEditor::save(std::string file) {
    std::ofstream outfile(file);
    if(outfile)
    {
        for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end(); p++)
        {
            outfile << *p << '\n';
        }
        return true;
    }
    return false;
}

void StudentTextEditor::reset() {
	// TODO: clear undo stack
    for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end();)
    {
        p = m_lines.erase(p);
    }
    m_col = 0;
    m_row = 0;
    m_currentRow = m_lines.begin();

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
    if(m_col < m_currentRow->size())
    {
        *m_currentRow = m_currentRow->substr(0, m_col) + m_currentRow->substr(m_col+1, m_currentRow->size());
    }
    else if(m_row+1 < m_lines.size())
    {
        m_currentRow++;
        std::string temp = *m_currentRow;
        m_currentRow = --m_lines.erase(m_currentRow);
        *m_currentRow += temp;
    }
   
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
    //splits the current line into two parts
    std::string next = m_currentRow->substr(m_col, m_currentRow->size()-m_col);
    std::string curr = m_currentRow->substr(0, m_col);
    *m_currentRow = curr;
    
    //creates a new row at the end
    if(m_row + 1 == m_lines.size())
    {
        m_lines.push_back(next);
        m_row++;
        m_col = 0;
        m_currentRow++;
    }
    //creates a new row in the middle
    else
    {
        m_currentRow++;
        m_lines.insert(m_currentRow, next);
        m_currentRow--;
        m_row++;
        m_col = 0;
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
