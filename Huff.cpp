#include "Huff.h"

bool lessthan(Node *a, Node *b)
{
    if(a->number == b->number)
        return a->content < b->content;
    return a->number < b->number;
}


void Huffman::comprimir(QString string, QString string2)
{
        int count[256] = {0};
        QFile file(string);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Ocorreu um erro ao tentar abrir o arquivo original.";
            exit(1);
        }
        QByteArray line, linha; // Conterá o texto do arquivo original completo
        while (!file.atEnd())
        {
            line = file.readLine(1024);
            linha.append(line);
            for(int i = 0; i < line.size(); ++i)
                ++count[(unsigned char) line.at(i)];
        }
        int i = 0, j;
        QVector<Node*> y(0); // vetor de nós.
        Node *no;
        for(j = 0; j < 256; ++j) // monta o nosso vetor
            if(count[j])
            {
                no = new Node;
                no->number = count[j];
                no->content = (unsigned char)j;
                y.append(no);
            }
        tree arvore;
        Node *raiz;
        for(; y.size() != 1;) // monta a árvore
        {
            qSort(y.begin(), y.end(), lessthan);
            raiz = new Node;
            raiz->left = y[0];
            raiz->right = y[1];
            raiz->number = y[0]->number + y[1]->number;
            y.remove(0, 2);
            y.append(raiz);
        }
        arvore.append(y.at(0)); // aqui o nó principal vira a raiz da árvore
        QVector<QByteArray> cod(256);
        QByteArray xcv = arvore.byte(cod);
        QByteArray codif; // será o bytearray com a codificação do arquivo
        QByteArray aux;
        qDebug() << cod;
        int k, g;
        for(i = 0; i < linha.size(); ++i)
            codif.append(cod[(unsigned char)linha.at(i)]);
        for(i = 0, k = 7, j = 0; i < codif.size(); ++i, --k)
        {
            if(codif[i] == 49)
                j = j + (1 << k);
            if(k == 0 || i == codif.size() - 1)
            {
                aux.append(j);
                j = 0;
                k = 8;
            }
        }
        int lixo = 8 - codif.size()%8; // tamanho do lixo da codificação
        linha.clear();
        for(i = 0, k = 2, j = 0, g = 0; i < 16; ++i, --k)
        {
            if(i <= 2)
            {
                if(g + (1 << k) <= lixo)
                {
                    j += (1 << (k + 5));
                    g += (1 << k);
                }
                if(i == 2)
                {
                    g = 0;
                    k = 13;
                }
            }
            else if(i > 2)
            {
                if(g + (1 << k) <= xcv.size())
                {
                    if(i <= 7)
                        j += (1 << (k - 8));
                    else
                        j += (1 << k);
                    g += (1 << k);
                }
                if(i == 7)
                {
                    linha.append(j);
                    j = 0;
                }
                if(g == xcv.size())
                {
                    linha.append(j);
                    break;
                }
            }
        }
        linha.append(string.size()).append(string).append(xcv).append(aux);
        file.close(); // já fiz oq tinha que fazer no arquivo original, então eu o fecho.
        QFile newfile(string2); // criação do novo arquivo que é o original comprimido.
        if(!newfile.open(QIODevice::WriteOnly))
            {
                qDebug() << "Ocorreu um erro ao abrir o arquivo da compressão.";
                exit(1);
            }
        newfile.write(linha); // nesta linha ocorre a compressão.
        newfile.close(); // compressão concluída.
        qDebug() << "Arquivo comprimido com êxito.";
}







    // compressão acima







    // ------------------------------------------------------------------------------------------






    // Descompressão abaixo








    void Huffman::descomprimir(QString string)
    {
        QFile fil(string);
        if(!fil.open(QIODevice::ReadOnly)){
            qDebug() << "O arquivo comprimido não pôde ser aberto.";
            exit(1);
        }
        QByteArray x;
        x = fil.readAll(); // necessário para saber os dados originais do arquivo comprimido
        QBitArray tam(24);
        int j, k, i, m;
        unsigned char o;
        for(j = 0, k = 0, i = 7, m = 0, o = x[0]; k < 24; ++k, --i)
        {
            if(j + (1 << i) <= o)
            {
                j = j + (1 << i);
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
                    j = j + (1 << i);
                if(k == 2)
                    i = 13;
            }
            else if(k >= 3 && k < 16)
            {
                if(tam[k] == true)
                    m = m + (1 << i);
                if(k == 15)
                    i = 8;
            }
            else
            {
                if(tam[k] == true)
                    nome = nome + (1 << i);
            }
        }
        QByteArray rp;
        for(i = 3 + nome; i < 3 + nome + m; ++i) // aqui eu refaço o meu bytearray da árvore
            rp.append(x[i]);
        tree dec;
        Node *node = new Node; // cria a minha raiz que irá para a árvore
        dec.append(node); // aqui ela vai pra árvore
        dec.mont(node, rp); // aqui eu monto a árvore
        int c = x.size() - 3 - m - nome, r; // grava a quantidade de bytes correspondentes à codificação
        QBitArray z(c*8); // cria o bit array da decodificação
        for(c = 0, i = 7, r = 0, k = 3 + m + nome, o = x[k]; c < z.size(); --i, ++c) // constrói o meu bitarray
        {                                                                               // da decodificação.
            if(r + (1 << i) <= o)
            {
                r = r + (1 << i);
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
        QString string2;
        string2 = x.remove(0, 3);
        string2 = x.remove(nome, x.size() - nome);
        string2.prepend("new");
        QFile huff(string2); // aqui eu crio o novo arquivo descomprimido
        if(!huff.open(QIODevice::WriteOnly))
        {
            qDebug() << "O arquivo da descompressão não pôde ser aberto.";
            exit(1);
        };
        QByteArray b; // auxiliar para a função ".write" de QFile(essa função só aceita QByteArray como argumento)
        for(node = dec.raiz(), k = 0; k < z.size() - j; ++k) // aqui eu escrevo o código do arquivo para a descompressão
        {
            if(node->left && node->right)
            {
                if(z[k] == false)
                    node = node->left;
                else
                    node = node->right;
            }
            if(!node->left && !node->right)
            {
                b.append(node->content);
                node = dec.raiz();
            }
        }
        huff.write(b); // O arquivo descompresso agora é igual ao original
        huff.close(); // fecho o arquivo descomprimido
        fil.close(); // descompressão concluída(fecho o meu arquivo comprimido)
        qDebug() << "Arquivo descomprimido com êxito.";
    }
