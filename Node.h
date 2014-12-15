#ifndef NODE_H
#define NODE_H

#include <QByteArray>

class Node // A classe dos nós que são fundamentais em todo o processo
{
public:
    Node *left;
    Node *right;
    unsigned char content;
    int number;
    QByteArray code;
    Node()
    {
        this->content = 0;
        this->left = 0;
        this->right = 0;
        number = 0;
    }
};

#endif // NODE_H
