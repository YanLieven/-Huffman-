#include <QCoreApplication>
#include "Huff.h"

void help()
{
    qDebug() << "//------------------------------------------------------------------------------";
    qDebug() << "Se for comprimir um arquivo, esse não deve possuir formato '.huff',";
    qDebug() << "é necessário digitar 1º- '-c', 2º- 'nome do arquivo original'.";
    qDebug() << "O arquivo de saída terá formato .huff automaticamente.";
    qDebug() << "//------------------------------------------------------------------------------";
    qDebug() << "Se for descomprimir um arquivo, esse deve possuir obrigatoriamente formato '.huff',";
    qDebug() << "//------------------------------------------------------------------------------";
    qDebug() << "SOMENTE SERÃO ACEITOS 2(DOIS) ARGUMENTOS NA COMPRESSÃO E 1(UM) NA DESCOMPRESSÃO.";
    qDebug() << "//------------------------------------------------------------------------------";
    qDebug() << "para mostrar essa mensagem novamente, execute o programa com 1 argumento apenas: '-h'.";
    qDebug() << "//------------------------------------------------------------------------------;";
}

int main(int argc, char *argv[])
{
    Huffman huff;
    QCoreApplication a(argc, argv);
    if(argc == 3)
    {
        QString com(argv[1]), file(argv[2]), filef;
        filef = file.remove(0, file.size() - 4);
        if(!com.compare("-c"))
        {
            if(filef.compare("huff"))
            {
                file = argv[2];
                filef = file.remove(file.size() - 4, 4);
                file = argv[2];
                filef.append(".huff");
                huff.comprimir(file, filef);
            }
            else
            {
                qDebug() << "O arquivo de entrada não pode ter formato '.huff'.";
                help();
                exit(1);
            }
        }
        else
        {
            qDebug() << "O primeiro argumento não é reconhecido pelo programa.";
            help();
            exit(1);
        }
    }
    else if(argc == 2)
    {
        QString file(argv[1]), filef;
        filef = file.remove(0, file.length() - 4);
        file = argv[1];
        if(!file.compare("-h"))
            help();
        else if(!filef.compare("huff"))
            huff.descomprimir(file);
        else
        {
            qDebug() << "O arquivo não possui formato '.huff'!.";
            help();
            exit(1);
        }
    }
    else
    {
        qDebug() << "Não foi passado um número correto de argumentos para o programa.";
        help();
        exit(1);
    }
    return 0;
}
