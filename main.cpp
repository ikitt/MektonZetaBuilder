/*
A checker
    pour la création dynamique : http://www.yosko.net/article5/qt-creation-dynamique-de-widgets
    pour l'init des différentes valeurs : les fichiers .ini et leur utilisation
    utilisation de QList et connexion dynamique : http://www.qtfr.org/viewtopic.php?id=15430
*/
#include <QApplication>
#include <QPushButton>

#include "formmzb.h"

/*            *******                */

int main(int argc, char *argv[])
{
    QApplication ui(argc, argv);

    FormMZB interface;

    interface.show();

//    QPushButton bouton("Jean Némar ...");
//    bouton.show();

    return ui.exec();
}
