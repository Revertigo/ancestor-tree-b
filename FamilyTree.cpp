#include "FamilyTree.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>

namespace family
{
    //Ctor
    Tree::Tree(string name)
    {
        _root = new Node(name, NULL, NULL, NULL, 0);
    }

    //Dtor
    Tree::~Tree()
    {
        deleteSubtree(_root->_father);
        deleteSubtree(_root->_mother);
        //Only then delete the root itself
        delete _root;
        _root = NULL;
    }

    Tree & Tree::addFather(const string & son, const string & father)
    {
        Node * son_node = findInner(_root, son);
        if(son_node != NULL) {
            return add(son_node->_father, *son_node, father);
        } else{
            throw invalid_argument("Trying to add father to none existing node");
        }
    }

    Tree & Tree::addMother(const string & son, const string & mother)
    {
        Node * son_node = findInner(_root, son);
        if(son_node != NULL) {
            return add(son_node->_mother, *son_node, mother);
        }else{
            throw invalid_argument("Trying to add mother to none existing node");
        }
    }

    void Tree::display(void)
    {
        display("", _root, false);
    }

    string Tree::relation(const string & name)
    {
        Node * node = findInner(_root, name);
        string result = "unrelated";
        if(node != NULL)
        {
            result = resolveHeight(*node);
        }

        return result;
    }

    string Tree::find(const string & relation)
    {
        string result = _root->_name;//Assuming it's me
        bool found = false;

        if(validateRelation(relation)) //let's find this bastard !
        {
            Node * temp = _root; //We don't want to lose the pointer to the root
            int height = 0;
            int relation_height = resolveHeight(relation);

            do
            {
                while(temp->_father != NULL || temp->_mother != NULL)
                {
                    //First traverse all left child(father)
                    while(temp->_father != NULL)
                    {
                        temp = temp->_father;
                        height++;
                    }

                    //Then check if there is a right child(mother)
                    if(temp->_mother != NULL)
                    {
                        temp = temp->_mother;
                        height++;
                    }
                }

                while(temp != _root)//Now, going up.
                {
                    if(height == relation_height && ((relation.find("father") != string::npos && temp == temp->_son->_father) ||
                                                     (relation.find("mother") != string::npos && temp == temp->_son->_mother)))
                    {
                        //We have a match !!
                        result = temp->_name;
                        found = true;
                        break;
                    }


                    if(temp == temp->_son->_mother || ((temp->_son->_father == temp) && (temp->_son->_mother == NULL)))
                    {
                        //Actually go up
                        temp = temp->_son;
                        height--;
                    }
                    else//Means we are a left child and we have right child.
                    {
                        //Go to the right child(stay on the same height)
                        temp = temp->_son->_mother;
                        break;//Only here we can start new iteration.
                    }
                }
            }while(temp != _root && !found);

            if(relation == "me")
            {
                found = true;
            }
        }

        if(!found)
        {
            throw invalid_argument("The tree cannot handle the " + relation + " relation");
        }

        return result;
    }

    void Tree::remove(string name)
    {
        Node * node_to_remove = findInner(_root, name);
        if(node_to_remove == _root || node_to_remove == NULL)
        {
            throw invalid_argument("Can't remove this node(root or none existing)");
        }

        if(node_to_remove == node_to_remove->_son->_father)
        {
            node_to_remove->_son->_father = NULL;//unlink the father from his son
        } else{
            node_to_remove->_son->_mother = NULL;//unlink the mother from his son
        }

        deleteSubtree(node_to_remove);
    }


    Tree & Tree::add(Node *& node, Node & son, const string & older)
    {
        if(node != nullptr)
        {
            node->_name = older;
        }
        else{
            node = new Node(older, NULL, NULL, &son, son._height + 1);
        }

        return *this;
    }

    Tree::Node * Tree::findInner(Node * start, string name)
    {
        if(!start)
        {
            return NULL;
        }
        else if(start->_name == name)
        {
            return start;
        }
        else {
            Node * father = findInner(start->_father, name);
            Node * mother = findInner(start->_mother, name);

            if(father != NULL){
                return father;
            }
            if(mother != NULL)
            {
                return mother;
            }

            return NULL;
        }
    }

    string Tree::resolveHeight(Node & node)
    {
        string relation = "";
        switch(node._height)
        {
            case 0:
            {
                relation = "me";
                break;
            }

                //Otherwise we need to check how many great- to add, whether to add grand or not
            default:
            {
                int height = node._height;
                Node * temp = &node;

                //Check if this node came is left or right(left = father, right = mother)
                if(temp == temp->_son->_father)
                {
                    relation += "father";
                } else{
                    relation += "mother";
                }

                //Higher then one it means the node is above mother/father
                if(height > 1)
                {
                    //Append to the start of the string
                    relation.insert(0, "grand");
                }

                while(height > 2)
                {
                    temp = temp->_son;
                    height--;
                    relation.insert(0, "great-");
                }

                break;
            }
        }

        return relation;
    }

    /**
     * This function translate relation to height of the node. For example
     * it will translate the relation great-great-grandmother to the height 4
     * @param relation - relation to translate
     * @return height of the relation
     */
    int Tree::resolveHeight(const string & relation)
    {
        int height = 0;

        if(relation  == "father" || relation == "mother")
        {
            height = 1;
        }
        else if(relation == "grandfather" || relation == "grandmother")
        {
            height = 2;
        }
        else{
            //+2 since only after grand something we counting with great
            string a(relation);
            height = stringCount(relation, "great-") + 2;
        }

        return  height;
    }

    /**
     * https://stackoverflow.com/questions/6067460/c-how-to-calculate-the-number-time-a-string-occurs-in-a-data
     */
    size_t Tree::stringCount(const string& referenceString,
                       const string& subString) {

        const size_t step = subString.size();

        size_t count(0);
        size_t pos(0) ;

        while( (pos=referenceString.find(subString, pos)) !=std::string::npos) {
            pos +=step;
            ++count ;
        }

        return count;

    }

    bool Tree::validateRelation(const string & relation)
    {
        bool is_valid = true;

        string base_str(relation);
        vector<string> delims = {"great-", "grand"};
        //We want to clean all possible none relevant words to validate our input using split
        for(auto const& delimiter: delims)
        {

            int pos = 0;
            while ((pos = base_str.find(delimiter)) != std::string::npos) {
                base_str.erase(pos,  delimiter.length());
            }
        }


        if(base_str != "father" && base_str != "mother" && base_str != "me")
        {
            is_valid = false;
        }

        return  is_valid;
    }

    void Tree::deleteSubtree(Node *& node)
    {
        if(node == NULL)
        {
            return;
        }

        deleteSubtree(node->_father);
        deleteSubtree(node->_mother);
        node->_father = NULL;
        node->_mother = NULL;
        node->_son = NULL;
        delete node;
        node = NULL;//We don't want a dangling pointer
    }

    /**
     * https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
     * @param prefix
     * @param node
     * @param is_left
     */
    void Tree::display(const std::string& prefix, Node * node, bool is_left)
    {
        if( node != nullptr )
        {
            cout << prefix;

            std::cout << (is_left ? "├──" : "└──" );

            // print the value of the node
            std::cout << node->_name << std::endl;

            // enter the next tree level - left and right branch
            display( prefix + (is_left ? "│   " : "    "), node->_father, true);
            display( prefix + (is_left ? "│   " : "    "), node->_mother, false);
        }
    }

}