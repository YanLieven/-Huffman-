#ifndef TREE_H
#define TREE_H

#include <QString>
#include <QByteArray>
#include <QBitArray>

#include "Node.h"

class tree
{
public:
    Node *root;
    tree()
    {
        root = 0;
    }
    void append(Node *node)
    {
        root = node;
    }
    QByteArray aux(Node *node)
    {
       if(node->left == 0 && node->right == 0)
          {
            QByteArray ret;
            if(node->content == 0x21)
            {
                ret.append(0x2A);
                ret.append(0x21);
            }
            else if(node->content == 0x2A)
            {
                ret.append(0x2A);
                ret.append(0x2A);
            }
            else
                ret.append(node->content);
            return ret;
          }
       return QByteArray().append(0x21).append(aux(node->left)).append(aux(node->right));
    }
    QByteArray byte()
    {
        return aux(root);
    }
    QBitArray cod(QByteArray array)
    {
        int i;
        QBitArray ar(0);
        Node *a;
        for(i = 0; i < array.size(); ++i)
        {
            for(a = root; a->content != array[i] && a->left != 0 && a->right != 0;)
            {
                if(codif(a->left, array[i]) == true)
                {
                    ar.resize(ar.size() + 1);
                    ar[ar.size() - 1] = false;
                    a = a->left;
                }
                else
                {
                    ar.resize(ar.size() + 1);
                    ar[ar.size() - 1] = true;
                    a = a->right;
                }
            }
        }
        return ar;
    }
    bool codif(Node *node, unsigned char c)
    {
        if(node->left == 0 && node->right == 0)
        {
            if(node->content == c)
                return true;
            else
                return false;
        }
        return codif(node->left, c) + codif(node->right, c);
    }
    void mont(Node *node, QByteArray x)
    {
        if(x[0] == 0x21)
        {
            node->left = new Node;
            node->right = new Node;
            int y = 1, pos = 2;
            while(y < pos)
            {
                if(x[y] == 0x21)
                    pos = pos + 2;
                else if(x[y] == 0x2A)
                {
                    ++pos;
                    ++y;
                }
                ++y;
            }
            x.remove(0,1);
            mont(node->left, x);
            x.remove(0, pos - 1);
            mont(node->right, x);
        }
        else if(x[0] == 0x2A)
            node->content = x[1];
        else
            node->content = x[0];
    }
    Node *raiz()
    {
        return root;
    }
};

#endif // TREE_H
