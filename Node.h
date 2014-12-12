#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node *left;
    Node *right;
    unsigned char content;
    int number;
    Node()
    {
        this->content = 0;
        this->left = 0;
        this->right = 0;
        number = 0;
    }
};

#endif // NODE_H
