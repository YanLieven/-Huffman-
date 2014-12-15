#ifndef HUFF_H
#define HUFF_H

#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <algorithm>
#include <QVector>
#include <QBitArray>

#include "Tree.h"
#include "Node.h"

bool lessthan(Node *a, Node *b);

class Huffman
{
public:
    Huffman(){}
    void comprimir(QString string, QString string2);
    void descomprimir(QString string, QString string2);
};

#endif // HUFF_H
