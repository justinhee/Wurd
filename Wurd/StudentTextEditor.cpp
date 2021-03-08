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
 : TextEditor(undo), m_row(m_lines.begin()), m_col(0) {
	// TODO
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
}

bool StudentTextEditor::load(std::string file) {
	// TODO
    std::ifstream infile(file);
    if(infile)
    {
        for(std::list<std::string>::iterator p = m_lines.begin(); p != m_lines.end();)
        {
            p = m_lines.erase(p);
        }
        m_row = m_lines.begin();
        m_col = 0;
        std::string nextLine;
        while(getline(infile, nextLine))
        {
            if(nextLine[nextLine.size()-1] == '\r')
                nextLine = nextLine.substr(0,nextLine.size()-1);
            m_lines.insert(m_row, nextLine);
        }
        m_row = m_lines.begin();
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
}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
	// TODO
}

void StudentTextEditor::enter() {
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = 0; col = 0; // TODO
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	return 0; // TODO
}

void StudentTextEditor::undo() {
	// TODO
}
