#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


//must be O(1)
TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

//initialize the row and col to 0, push back the first line into m_lines
StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_col(0), m_row(0) {
	// TODO
     m_lines.push_back("");
     m_currentRow = m_lines.begin();
}

// This method must run in O(N) time where N is the number of lines in the file currently being edited.
StudentTextEditor::~StudentTextEditor()
{
    for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end();)
    {
        p = m_lines.erase(p);
    }
}

//. This method must run in O(M+N+U) time where M is the number of characters in the editor currently being edited, N is the number of characters in the new file being loaded, and U is the number of items in the undo stack.
bool StudentTextEditor::load(std::string file) {
	// TODO
    std::ifstream infile(file);
    if(infile)
    {
        //resets editor
        reset();
        std::string nextLine;
        while(getline(infile, nextLine))
        {
            //checks for carriage return character
            if(nextLine != "" && nextLine[nextLine.size()-1] == '\r')
                nextLine = nextLine.substr(0,nextLine.size()-1);
            m_lines.push_back(nextLine);
        }
        m_currentRow = m_lines.begin();
        //return true if load successful
        return true;
    }
    //return false if failed
    return false;
}
//This method must run in O(M) time where M is the number of characters in the editor currently being edited.
bool StudentTextEditor::save(std::string file) {
    std::ofstream outfile(file);
    if(outfile)
    {
        //add a newline character after every line
        for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end(); p++)
        {
            outfile << *p << '\n';
        }
        return true;
    }
    return false;
}

//This operation must run in O(N+U) time, where N is the number of rows being edited and U is the number of items in the undo stack.
void StudentTextEditor::reset() {
	// TODO: clear undo stack
    //clears each line of the list
    for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end();)
    {
        p = m_lines.erase(p);
    }
    
    //resets row/col position
    m_col = 0;
    m_row = 0;
    m_currentRow = m_lines.begin();
    
    //clears undo stack
    getUndo()->clear();

}

//Each of these operations must run in O(1) time, where the operation’s runtime does not depend on the number of lines being edited or the length of the current line.
void StudentTextEditor::move(Dir dir) {
	// TODO
    switch(dir)
    {
        case Dir::UP:
            //if it's not the first row, go up a row
            if(m_row > 0)
            {
                m_row--;
                m_currentRow--;
                int n = m_currentRow->size();
                //if the row above is smaller, go to it's last char
                m_col = m_col > n ? n : m_col;
            }
            break;
        case Dir::DOWN:
            //if it's not the last row then go down a row
            if(m_row+1 < m_lines.size())
            {
                m_row++;
                m_currentRow++;
                int n = m_currentRow->size();
                //if the row below is smaller, then go to it's last char
                m_col = m_col > n ? n : m_col;
            }
            break;
        case Dir::LEFT:
            //if it's not the first char, go left
            if(m_col > 0)
            {
                m_col--;
            }
            //if it's the first char and not the first row, go to the last col of the previous row
            else if(m_col == 0 && m_row > 0)
            {
                m_row--;
                m_currentRow--;
                m_col = m_currentRow->size();
            }
            break;
        case Dir::RIGHT:
            //if it's not past the last char then move right
            if(m_col < m_currentRow->size())
            {
                m_col++;
            }
            //if it's past the last char and not the last row, move down a row
            else if(m_col == m_currentRow->size() && m_row+1 < m_lines.size())
            {
                m_col = 0;
                m_row++;
                m_currentRow++;
            }
            break;
        case Dir::HOME:
            //go to the beginning of the row
            m_col = 0;
            break;
        case Dir::END:
            //go to the last col of the row
            m_col = m_currentRow->size();
            break;
        default:
            break;
            
            
    }
}

//joins the current line and the previous line
void StudentTextEditor::helperJoin()
{
    std::string temp = *m_currentRow;
    //erases the current line and sets m_currentRow to the previous line
    m_currentRow = --m_lines.erase(m_currentRow);
    //sets the col to the last spot on the previous line before the join
    m_col = m_currentRow->size();
    //joins the text together
    *m_currentRow += temp;
    
}

/*Deletion of a character in the middle of a line must run in O(L) time where L is the length
 of the line of text containing the current editing position. Deletion at the end of a line
 resulting a merge must run in O(L1+L2) where L1 is the length of the current line of text
 at the editing position and L2 is the length of the next line in the editor. This command
 must not have a runtime that depends on the number of lines being edited!*/

void StudentTextEditor::del() {
	// TODO: undo
    //this is a deleted character
    if(m_col < m_currentRow->size())
    {
        //collects the character to submit
        char ch = (*m_currentRow)[m_col];
        
        //adds the substrings without the character
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


/*Backspacing that does not result in a merge must run in O(L) time where L is the length
 of the line of text containing the current editing position. Backspacing at the front of a
 line resulting in a merge with the previous line must run in O(L1+L2) where L1 is the
 length of the line of text containing the current editing position and L2 is the length of the
 previous line in the editor. This command must not have a runtime that depends on the
 number of lines being edited!*/
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

//Insertion of a character into a line must run in O(L) time where L is the length of the line of text containing the current editing position.
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

//Using the enter command anywhere on a line must run in O(L) time where L is the length of the line of text containing the current editing position. This command must not have a runtime that depends on the number of lines being edited!
void StudentTextEditor::enter() {
    //submits the position before the split
    getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
    
    //splits the line into two parts
    helperSplit();
}

//This operation must run in O(1) time, where the operation’s runtime does not depend on the number of lines being edited or the length of the current line.
void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col;
}

/*Let oldR be lines.size() at the time this function was entered. This operation must run in
 O(oldR + abs(current row number - startRow) + numRows*L) time, where L is the average
 line length. The runtime must not depend on the number of lines being edited.*/
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

/*This method must run in a time proportional to the nature of the undo operation plus the
 distance from the current cursor position to the position of the operation. For example, if
 the undo is of a deleted character, then its runtime must be the cost to re-insert the
 deleted character into the editor. If the undo is of a line break, then the runtime must be
 the cost to re-merge the line that was broken into two parts, etc. */
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
            //move to the correct spot and delete the given number of characters
            moveTo(row, col);
            *m_currentRow = m_currentRow->substr(0, m_col-count) + m_currentRow->substr(m_col, m_currentRow->size());
            m_col -= count;
            break;
        case Undo::Action::SPLIT:
            //move to the correct spot and split lines
            moveTo(row, col);
            helperSplit();
            break;
        case Undo::Action::JOIN:
            //move to the corect spot and join lines
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
