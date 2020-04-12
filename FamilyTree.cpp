#include "FamilyTree.hpp"
#include <stdexcept>

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
        remove(_root->_name);
    }

    Tree & Tree::addFather(const string & son, const string & father)
    {
        Node * son_node = findInner(_root, son);
        if(son_node != NULL) {
            return add(son_node->_father, *son_node, father);
        } else{
            throw std::invalid_argument("trying to add father to none existing node");
        }


    }

    Tree & Tree::addMother(const string & son, const string & mother)
    {
        Node * son_node = findInner(_root, son);
        if(son_node != NULL) {
            return add(son_node->_mother, *son_node, mother);
        }else{
            throw std::invalid_argument("trying to add mother to none existing node");
        }
    }

    void Tree::display(void){}

    string Tree::relation(const string & name)
    {
        Node * node = findInner(_root, name);
        string result = "unrelated";
        if(node != NULL)
        {
            result = resolve_height(*node);
        }

        return result;
    }
    string Tree::find(const string & relation){ return "";}
    void Tree::remove(string name){}


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
            Node * father = findInner(_root->_father, _root->_father->_name);
            Node * mother = findInner(_root->_mother, _root->_mother->_name);

            if(father != NULL){
                return father;
            }
            else if(mother != NULL)
            {
                return mother;
            }

            return NULL;
        }
    }

    string Tree::resolve_height(Node & node)
    {
        string relation = "";
        switch(node._height)
        {

            case 0:
            {
                relation = "me";
                break;
            }
            case 1:
            {
                relation = "mother";
                if(node == *node._son->_father)
                {
                    relation = "father";
                }
                break;
            }

            case 2:
            {
                relation = "grandmother";
                if(node == *node._son->_father)
                {
                    relation = "grandfather";
                }
                break;
            }
            //Otherwise we need to check how many great- to add
            default:
            {

            }
        }

        return relation;

    }


}