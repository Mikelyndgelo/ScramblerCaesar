#include "ScramblerCaesar.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScramblerCaesar w;
    w.show();
    return a.exec();
}
