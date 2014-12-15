#include <QCoreApplication>
#include "Huff.h"

void help()
{
    qDebug() << "Para que o programa execute a compressão, é necessário digitar:";
    qDebug() << "1º - Nome do arquivo a ser Comprimido/Descomprimido, 2º - Nome do arquivo de saída.";
    qDebug() << "//--------------------------------------------------------------------";
    qDebug() << "Se for comprimir um arquivo, esse não deve possuir formato '.huff',";
    qDebug() << "O arquivo de saída terá formato .huff automaticamente.";
    qDebug() << "//--------------------------------------------------------------------";
    qDebug() << "Se for descomprimir um arquivo, esse deve possuir obrigatoriamente formato '.huff',";
    qDebug() << "O arquivo de saída pode ter o formato mais desejado.";
    qDebug() << "//--------------------------------------------------------------------";
    qDebug() << "SOMENTE SERÃO ACEITOS 2(DOIS) ARGUMENTOS.";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(argc == 2) // O programa só aceita 1 ou dois argumentos como entrada.
    {
        if(strcmp(argv[1],"help") == 0)
            help();
        else
            qDebug() << argv[1] << "<-- este comando não é reconhecido pelo programa.";
        exit(1);
    }
    else if(argc == 3)
    {
        Huffman huff; // Uma variável da nossa classe Huffman que é essencial no programa
        QString file = argv[1], newfile = argv[2];
        file = file.remove(0, file.size() - 5);
        newfile = newfile.remove(0, newfile.size() - 5);
        if(strcmp(file, ".huff") == 0 && strcmp(newfile, ".huff") != 0)
        {
            file = argv[1];
            newfile = argv[2];
            huff.descomprimir(file, newfile);
        }
        else if(strcmp(file, ".huff") != 0)
        {
            if(strcmp(newfile, ".huff") == 0)
                newfile = argv[2];
            else
            {
                newfile = argv[2];
                newfile = newfile.remove(newfile.size() - 4, 4).append(".huff");
            }
            file = argv[1];
            huff.comprimir(file, newfile);
        }
        else
        {
            qDebug() << "Os dois argumentos são do tipo .huff, não há operação possível no programa";
            exit(1);
        }
    }
    else
    {
        qDebug() << "O número de argumentos passados não está correto.";
        exit(1);
    }
    return a.exec();
}
