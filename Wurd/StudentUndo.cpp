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

    UndoAction curr = m_stack.top();
    m_stack.pop();
    

    row = curr.m_row;
    col = curr.m_col;
    count = 1;
    text = "";
    
    if(curr.m_action == Action::INSERT)
    {
        //if there's more undo actions, check if they're inserts
        while(!m_stack.empty())
        {
            UndoAction next = m_stack.top();
            //if the next action is an insert and next to the current one, increment the count of characters to delete
            if(next.m_action == Action::INSERT && next.m_row == curr.m_row && next.m_col == curr.m_col-1)
            {
                count++;
                m_stack.pop();
                curr = next;
            }
            else
                break;
        }
        return Action::DELETE;
    }
    if(curr.m_action == Action::DELETE)
    {
        text += curr.m_ch;
        //if there's more undo actions, check if they're deletes
        while(!m_stack.empty())
        {
            UndoAction next = m_stack.top();
            //if the next action is an delete and next to or in the same place as the current one, add it's char to text
            if(next.m_action == Action::DELETE && next.m_row == curr.m_row && (next.m_col == curr.m_col || next.m_col == curr.m_col+1))
            {
                //this means that it was a delete, so add the char to the beginning
                if(next.m_col == curr.m_col)
                {
                    text = next.m_ch + text;
                }
                //this means that it was a backspace, so add the char to the end
                else
                {
                    text += next.m_ch;
                }
                
                m_stack.pop();
                curr = next;
            }
            else
                break;
        }
        return Action::INSERT;
        
    }
    
    
    switch (curr.m_action) {
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
