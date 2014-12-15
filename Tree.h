#ifndef TREE_H
#define TREE_H

#include <QString>
#include <QByteArray>
#include <QBitArray>
#include <QVector>
#include <QDebug>

#include "Node.h"

class tree
{
public:
    Node *root;
    tree();
    void append(Node *node); // cria a árvore
    QByteArray byte(QVector<QByteArray> &cod); // retorna a representação da árvore
    QByteArray aux(Node *node, QVector<QByteArray> &cod); // auxiliar para o método byte()
    void mont(Node *node, QByteArray x); // monta a árvore para a descompressão
    Node *raiz(); // retorna a raiz da árvore
};

#endif // TREE_H
