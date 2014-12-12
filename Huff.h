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

int power(int i) // usada para trabalhar com potências de 2
{
    int j = 1;
    for(; i > 0; --i)
    {
        j = 2*j;
    }
    return j;
}

class Huffman
{
public:
    void comprimir(QString string, QString string2)
    {
        int count[256] = {0};
        QFile file(string);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "The file could not be read";
            exit (1);
        }
        QByteArray line;
        QByteArray linha;
        while (!file.atEnd()) {
            line = file.readLine(1024);
            linha = linha.append(line);
            for(int i = 0; i < line.size(); ++i) {
            ++count[(unsigned char) line.at(i)];
            }
        }
        int i = 0, j;
        for(j = 0; j < 256; ++j) {
            if(count[j]){
                ++i;
            }
        }
        if(i <= 1)
        {
            qDebug() << "O arquivo não pode ser mais comprimido.";
            exit (1);
        }
        QVector<Node*> y(0); // vetor de nós.
        Node *no;
        for(j = 0; j < 256; ++j) // monta o nosso vetor
        {
            if(count[j])
            {
                no = new Node;
                no->number = count[j];
                no->content = (unsigned char)j;
                y.append(no);
            }
        }
        tree arvore;
        Node *raiz;
        Node *d;
        int c = i, k;
        for(k = 0; k < c; k = k + 2) // monta a árvore.
        {
            raiz = new Node;
            for(j = 0; j < c - 1; ++j) // ordena o vetor.
            {
                for(i = j + 1; i < c; ++i)
                {
                    if(y.at(j)->number > y.at(i)->number)
                    {
                        d = new Node;
                        d->number = y.at(j)->number;
                        d->content = y.at(j)->content;
                        d->left = y.at(j)->left;
                        d->right = y.at(j)->right;
                        y.at(j)->number = y.at(i)->number;
                        y.at(j)->content = y.at(i)->content;
                        y.at(j)->left = y.at(i)->left;
                        y.at(j)->right = y.at(i)->right;
                        y.at(i)->number = d->number;
                        y.at(i)->content = d->content;
                        y.at(i)->left = d->left;
                        y.at(i)->right = d->right;
                    }
                }
            }
            if(k + 1 < c) // cria o nosso nó que irá para a árvore.
            {
                raiz->number = y.at(k)->number + y.at(k+1)->number;
                raiz->left = y.at(k);
                raiz->right = y.at(k+1);
                y.append(raiz);
                ++c;
            }
        }
        k = k - 2;
        arvore.append(y.at(k)); // aqui o nó principal vira a raiz da árvore
        QBitArray cod = arvore.cod(linha); // codificação do arquivo
        int lixo = 8 - cod.size()%8; // tamanho do lixo da codificação
        cod.resize(cod.size() + lixo);
        QBitArray lixoarv(16); // aqui eu junto o tamanho do lixo com o tamanho da representação da árvore
        for(k = 0, i = 2, j = 0; k < 16; ++k, --i) // com o auxílio desse "for"
        {
            if(k >= 0 && k <= 2)
            {
                if(j + power(i) <= lixo)
                {
                    j = j + power(i);
                    lixoarv[k] = true;
                }
                if(k == 2)
                {
                    ++k;
                    i = 12;
                    j = 0;
                }
            }
            else if(k >= 3 && k <= 15)
            {
                if(j + power(i) <= arvore.byte().size())
                {
                    lixoarv[k] = true;
                    j = j + power(i);
                }
                if(j == arvore.byte().size())
                    break;
            }
        }
        QByteArray tamanho; // esse aqui conterá os 3 primeiros bytes do cabeçalho.
        tamanho.resize(2);
        tamanho.fill(0);
        for(k = 0, i = 7, j = 0, lixo = 0; k < 16; ++k, --i)
        {
            if(k >= 0 && k < 8)
            {
                if(lixoarv[k] == true)
                    lixo = lixo + power(i);
                if(k == 7)
                    i = 8;
            }
            else
                if(lixoarv[k] == true)
                    j = j + power(i);
        }
        tamanho[0] = lixo;
        tamanho[1] = j;
        QByteArray nomeori = string;
        tamanho.append(nomeori.size());
        QByteArray codif; // aqui eu transformo os bits da codificação em bytes
        codif.resize(cod.size()/8); // com a ajuda do "for" abaixo
        for(k = 0, j = 0, i = 7, lixo = 0; k < cod.size(); ++k, --i)
        {
            if(cod[k] == true)
            {
                j = j + power(i);
            }
            if((k+1)%8 == 0)
            {
                i = 8;
                codif[lixo] = j;
                ++lixo;
                j = 0;
            }
        }
        file.close(); // já fiz oq tinha que fazer no arquivo original, então eu o fecho.
        QFile newfile(string2); // criação do novo arquivo que é o original comprimido.
        if(!newfile.open(QIODevice::ReadWrite))
            {
                qDebug() << "The new file could not be opened.";
            }
        newfile.write(tamanho); // nesta e nas 3 linhas abaixo ocorre a compressão.
        newfile.write(nomeori);
        newfile.write(arvore.byte());
        newfile.write(codif);
        newfile.close(); // compressão concluída.
    }


    // Compressão acima


//------------------------------------------------------------------------------------------------------------


    // Descompressão abaixo


    void descomprimir(QFile &fil)
    {
        if(!fil.open(QIODevice::ReadOnly)){
            qDebug() << "The Compressed file could not be read";
            exit (1);
        }
        QByteArray x;
        x = fil.readAll();
        QBitArray tam(24);
        int j, k, i, m;
        unsigned char o;
        for(j = 0, k = 0, i = 7, m = 0, o = x[0]; k < 24; ++k, --i)
        {
            if(j + power(i) <= o)
            {
                j = j + power(i);
                tam[k] = true;
                if(j == o)
                {
                    i = 8;
                    j = 0;
                    k = m*8 + 7;
                    ++m;
                    o = x[m];
                }
            }
        }
        int nome;
        for(k = 0, i = 2, m = 0, j = 0, nome = 0; k < 24; ++k, --i) // depois desse for: j = lixo da cod.,
        {                                                        // m = tamanho da árvore., nome = tamanho do nome.;
            if(k >= 0 && k < 3)
            {
                if(tam[k] == true)
                    j = j + power(i);
                if(k == 2)
                    i = 13;
            }
            else if(k >= 3 && k < 16)
            {
                if(tam[k] == true)
                    m = m + power(i);
                if(k == 15)
                    i = 8;
            }
            else
            {
                if(tam[k] == true)
                    nome = nome + power(i);
            }
        }
        QByteArray rp;
        for(i = 3 + nome; i < 3 + nome + m; ++i) // aqui eu refaço o meu bytearray da árvore
            rp.append(x[i]);
        tree dec;
        Node *node = new Node; // cria a minha raiz que irá para a árvore
        dec.append(node); // aqui ela vai pra árvore
        dec.mont(node, rp); // aqiu eu monto a árvore
        int c = x.size() - 3 - m - nome, r; // grava a quantidade de bytes correspondentes à codificação
        QBitArray z(c*8); // cria o bit array da decodificação
        for(c = 0, i = 7, r = 0, k = 3 + m + nome, o = x[k]; c < z.size(); --i, ++c) // constrói o meu bitarray
        {                                                                               // da decodificação.
            if(r + power(i) <= o)
            {
                r = r + power(i);
                z[c] = true;
            }
            if((c + 1)%8 == 0 && c != 0)
            {
                r = 0;
                i = 8;
                ++k;
                o = x[k];
            }
        }
        QFile huff("novo.txt"); // aqui eu crio o novo arquivo descomprimido
        if(!huff.open(QIODevice::WriteOnly))
        {
            qDebug() << "Could not open the new file.";
            exit (1);
        };
        QByteArray b; // auxiliar para a função ".write" de QFile(essa função só aceita QByteArray como argumento)
        b.resize(1);
        for(node = dec.raiz(), k = 0; k < z.size() - j; ++k) // aqui eu escrevo o código do arquivo para a descompressão
        {
            if(node->left != 0 && node->right != 0)
            {
                if(z[k] == false)
                    node = node->left;
                else
                    node = node->right;
            }
            if(node->left == 0 && node->right == 0)
            {
                b[0] = node->content;
                huff.write(b);
                node = dec.raiz();
            }
        }
        huff.close(); // fecho o arquivo descomprimido
        fil.close(); // descompressão concluída(fecho o meu arquivo comprimido)
    }
};

#endif // HUFF_H
