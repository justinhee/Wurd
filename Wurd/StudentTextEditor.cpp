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
            if(nextLine != "" && nextLine[nextLine.size()-1] == '\r')
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
    getUndo()->clear();

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


void StudentTextEditor::helperJoin()
{
    std::string temp = *m_currentRow;
    m_currentRow = --m_lines.erase(m_currentRow);
    m_col = m_currentRow->size();
    *m_currentRow += temp;
    //submit undo action
    
}

void StudentTextEditor::del() {
	// TODO: undo
    //this is a deleted character
    if(m_col < m_currentRow->size())
    {
        //collects the character to submit
        char ch = (*m_currentRow)[m_col];
        
        *m_currentRow = m_currentRow->substr(0, m_col) + m_currentRow->substr(m_col+1, m_currentRow->size());
        //submit the undo action
        getUndo()->submit(Undo::Action::DELETE, m_row, m_col, ch);
        
    }
    //this joins lines if it's not the last line
    else if(m_row+1 < m_lines.size())
    {
        m_currentRow++;
        helperJoin();
        //submit undo action
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
        
    }
   
}

//removes the character before the curent location and moves the cursor left
void StudentTextEditor::helperBackspace()
{
    *m_currentRow = m_currentRow->substr(0, m_col-1) + m_currentRow->substr(m_col, m_currentRow->size());
    m_col--;
}

void StudentTextEditor::backspace() {
	// TODO: undo
    //regular backspace, no joining lines
    if(m_col > 0)
    {
        char ch = (*m_currentRow)[m_col-1];
        helperBackspace();
        //submit undo action
        getUndo()->submit(Undo::Action::DELETE, m_row, m_col, ch);
    }
    //this joins lines if it's not the first line
    else if(m_row != 0)
    {
        m_row--;
        helperJoin();
        //submit undo action
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
       
        
    }
}

void StudentTextEditor::helperInsert(char ch)
{
    //inserts ch into the current spot
    (*m_currentRow).insert((*m_currentRow).begin()+m_col, ch);
    m_col++;
}

void StudentTextEditor::insert(char ch) {
	// TODO: add undo
    //if the char inserted is a tab, insert 4 spaces instead
    if(ch == '\t')
    {
        for(int i = 0; i < 4; i++)
        {
            helperInsert(' ');
            getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
        }
    }
    //inserts char at current pos
    helperInsert(ch);
    //submits undo insert
    getUndo()->submit(Undo::Action::INSERT, m_row, m_col);
}


void StudentTextEditor::helperSplit()
{
    //splits the current line into two strings
    std::string next = m_currentRow->substr(m_col, m_currentRow->size()-m_col);
    std::string curr = m_currentRow->substr(0, m_col);
    *m_currentRow = curr;
    
    //creates a new row at the last line
    if(m_row + 1 == m_lines.size())
    {
        m_lines.push_back(next);
        m_row++;
        m_col = 0;
        m_currentRow++;
    }
    //creates a new row in a middle line
    else
    {
        m_currentRow++;
        m_lines.insert(m_currentRow, next);
        m_currentRow--;
        m_row++;
        m_col = 0;
    }
}

void StudentTextEditor::enter() {
	// TODO: add undo
    //submits the position before the split
    getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
    
    //splits the line into two parts
    helperSplit();
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
    //push count numRows rows or until it reaches the last row
    while(start != m_lines.end() && count < numRows)
    {
        lines.push_back((*start));
        start++;
        count++;
    }
    return count;

}

//moves the m_currentRow iterator and the row/col variables to the given values
void StudentTextEditor::moveTo(int row, int col)
{
    while(m_row < row)
    {
        m_row++;
        m_currentRow++;
    }
    while(m_row > row)
    {
        m_row--;
        m_currentRow--;
    }
    m_col = col;
}

void StudentTextEditor::undo() {
	// TODO
    int row;
    int col;
    int count;
    std::string text;
    switch (getUndo()->get(row, col, count, text)) {
        case Undo::Action::INSERT:
            //move to the correct row and insert the text string
            moveTo(row, col);
            m_currentRow->insert(m_col, text);
            break;
        case Undo::Action::DELETE:
            moveTo(row, col);
            *m_currentRow = m_currentRow->substr(0, m_col-count) + m_currentRow->substr(m_col, m_currentRow->size());
            m_col -= count;
            break;
        case Undo::Action::SPLIT:
            moveTo(row, col);
            helperSplit();
            break;
        case Undo::Action::JOIN:
            moveTo(row, col);
            m_currentRow++;
            helperJoin();
            break;
        case Undo::Action::ERROR:
            return;
        default:
            break;
    }
}
