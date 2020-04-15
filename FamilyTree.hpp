#ifndef ANCESTOR_TREE_A_FAMILYTREE_HPP
#define ANCESTOR_TREE_A_FAMILYTREE_HPP

#include <string>

using namespace std;

namespace family {
    class Tree {
        class Node {
        public:
            string _name;
            Node *_father;
            Node *_mother;
            Node *_son;
            int _height;//Height of the node in the tree
            //Initialization list :)
            Node(string name, Node *father, Node *mother, Node *son, int height) : _name(name), _father(father),
                                                                                   _mother(mother), _son(son),
                                                                                   _height(height) {};

            bool operator==(const Node &other) {
                if (_father == other._father && _mother == other._mother) {
                    return true;
                }

                return false;
            }
        };

        Tree::Node *_root;//Root father

        Tree &add(Node *&node, Node &son, const string &older);

        Node *findInner(Node *start, string name);

        //How fun is overloading
        string resolveHeight(Node &node);

        int resolveHeight(const string &relation);

        size_t stringCount(const string &reference_str,
                           const string &sub_str);

        bool validateRelation(const string &relation);

        void deleteSubtree(Node *&node);//Reference to the pointer in order to avoid double pointer :)
        void display(const string &prefix, Node *node, bool is_left);

    public:
        Tree(string name);

        ~Tree();

        Tree &addFather(const string &son, const string &father);

        Tree &addMother(const string &son, const string &mother);

        void display(void);

        string relation(const string &name);

        string find(const string &relation);

        void remove(string name);
    };
}

#endif //ANCESTOR_TREE_A_FAMILYTREE_HPP