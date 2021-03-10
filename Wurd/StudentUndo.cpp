#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
    m_stack.push(UndoAction(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if(m_stack.empty())
        return Action::ERROR;  // TODO'

    UndoAction top = m_stack.top();
    m_stack.pop();
    

    row = top.m_row;
    col = top.m_col;
    count = 1;
    text = "";
    
    if(top.m_action == Action::INSERT)
    {
        //if there's more undo actions, check if they're inserts
        if(!m_stack.empty())
        {
            UndoAction next = m_stack.top();
            //get all the consecutive inserts and add to the count of deletes
            while(next.m_action == Action::INSERT && next.m_row == top.m_row && next.m_col == top.m_col-1)
            {
                count++;
                m_stack.pop();
                top = next;
                
                if(!m_stack.empty())
                    next = m_stack.top();
            }
        }
        return Action::DELETE;
    }
    if(top.m_action == Action::DELETE)
    {
        text += top.m_ch;
        UndoAction next = m_stack.top();
        //get all the consective deletes/backspaces and add the characters to the text string
        while(!m_stack.empty() && next.m_action == Action::DELETE && next.m_row == top.m_row && (next.m_col == top.m_col && next.m_col == top.m_col+1))
        {
            //this means it was a delete, so add the char to the end
            if(next.m_col  == top.m_col)
            {
                text += next.m_ch;
            }
            //this means it was a backspace, so add the char to the beginning
            else
            {
                text = next.m_ch + text;
            }
            m_stack.pop();
            top = next;
            next = m_stack.top();

        }
        return Action::INSERT;
        
    }
    
    
    switch (top.m_action) {
        case Undo::INSERT:
            return Action::DELETE;
            break;
        case Undo::DELETE:
            return Action::INSERT;
            break;
        case Undo::SPLIT:
            return Action::JOIN;
            break;
        case Undo::JOIN:
            return Action::SPLIT;
            break;
        default:
            return Action::ERROR;
    };
    
}

void StudentUndo::clear() {
	// TODO
}
