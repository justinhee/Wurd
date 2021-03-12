#include "StudentUndo.h"
#include <list>

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	//push an UndoAction to m_stack
    m_stack.push(UndoAction(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    //return error if there's no undo actions
    if(m_stack.empty())
        return Action::ERROR;  // TODO'
    
    UndoAction curr = m_stack.top();
    m_stack.pop();
    
    //these values are the same for all actions
    row = curr.m_row;
    col = curr.m_col;
    count = 1;
    text = "";
    
    
    
    //CHECK EACH UNDO ACTION
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
    else if(curr.m_action == Action::DELETE)
    {
        std::list<char> list;
        std::list<char>::iterator i = list.begin(); //this will be the cursor position
        
        while(!m_stack.empty())
        {
            UndoAction next = m_stack.top();
            //use the next action to tell whether the current one is a delete or backspace, if it's the same col it's delete, else it's a backspace
            if(next.m_action == Action::DELETE && next.m_row == curr.m_row && (next.m_col == curr.m_col || next.m_col == curr.m_col+1))
            {
                
                //since col is the same, this operation was a delete
                //all deletes removed characters from the right, so add to the right side
                //set i = the new node to keep it in the middle of deletes/backspaces
                if(next.m_col == curr.m_col)
                {
                    i = list.insert(i, curr.m_ch);
                }
                //else this was a backspace
                //all backspaces removed characters from the left, so add to the left side
                else
                {
                    list.insert(i,curr.m_ch);
                }
                
                m_stack.pop();
                curr = next;
            }
            //if the action was not a delete/backspace or wasn't consecutive, break
            else
            {
                break;
            }
        }
        //the final char goes in the middle
        list.insert(i, curr.m_ch);
        //all the deleted chars go to the right of the cursor, all the backspaced ones go to the left. The most recent char goes in the middle
        
        //construct the string
        for(std::list<char>::iterator p = list.begin(); p != list.end(); p++)
        {
            text += *p;
        }
        
        return Action::INSERT;
        

//previous attempt:
//        std::string deletes;
//        std::string backspaces;
//        while(!m_stack.empty())
//        {
//            UndoAction next = m_stack.top();
//            //use the next action to tell whether the current one is a delete or backspace, if it's the same col it's delete, else it's a backspace
//            if(next.m_action == Action::DELETE && next.m_row == curr.m_row && (next.m_col == curr.m_col || next.m_col == curr.m_col+1))
//            {
//                //this means that it was a delete, so add the char to the beginning
//                if(next.m_col == curr.m_col)
//                {
//                    deletes = curr.m_ch + deletes;
//                }
//                //this means that it was a backspace, so add the char to the end
//                else
//                {
//                    backspaces += curr.m_ch;
//                }
//
//                m_stack.pop();
//                curr = next;
//            }
//            //if the action was not a delete/backspace or wasn't consecutive, break
//            else
//            {
//                break;
//            }
//        }
//        //all the deleted chars go to the right of the cursor, all the backspaced ones go to the left. The most recent char goes in the middle
//        text = backspaces + curr.m_ch + deletes;
//
//        return Action::INSERT;
        
    }
    //don't need to worry about consecutives for join/split
    else if(curr.m_action == Action::SPLIT)
    {
        return Action::JOIN;
    }
    else if(curr.m_action == Action::JOIN)
    {
        return Action::SPLIT;
    }
    
    //default return error
    return Action::ERROR;
    
}

void StudentUndo::clear() {
	//pop everything out of the undo stack
    while(!m_stack.empty())
        m_stack.pop();
    
}
