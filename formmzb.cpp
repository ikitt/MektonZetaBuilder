/*PROCHAINE ETAPES :
 * Faire une classe par onglet ?
 * Gérer les roues, chenilles, SES et micro propulseurs



*/




#include "formmzb.h"
#include "ui_formmzb.h"

#include <iostream>

//TODO faire une colonne "Nota" pour chaque élément, ignorer en général servant pour les capacité spéciales, notamment celle de MZP
//TODO chargeur infini à prendre en compte et tous les trucs spé

/* CONSTRUCTEUR */

FormMZB::FormMZB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMZB)
{
    ui->setupUi(this);

    connect(ui->pushButtonAjoutServo, SIGNAL( released() ), this, SLOT( ajoutServoPressed() ) );
    connect(ui->pushButtonAjoutRoue, SIGNAL( released() ), this, SLOT( ajoutRoue() ) );
    connect(ui->pushButtonAjoutChenille, SIGNAL( released() ), this, SLOT( ajoutChenille() ) );
    connect(ui->pushButtonAjoutSES, SIGNAL( released() ), this, SLOT( ajoutSES()) ) ;
    connect(ui->pushButtonAjoutPropulseur, SIGNAL( released() ), this, SLOT( ajoutPropulseur() ) );

    this->initUI();
}


/* DESTRUCTEUR */

FormMZB::~FormMZB()
{
    //TODO ajouter la suppression des servo de listServo pour être propre

    delete ui;
}


//fonction d'initialisation de l'interface
void FormMZB::initUI()
{
    nbServo = 0;
    nbServoCrees = 0;
    poidsTot = 0;
    poidsChassis = 0;
    mvtSol = 0;
    mvtAir = 0;
    valMan = 0;

    ui->gridLayoutChassis->setColumnStretch(2,5);
    ui->gridLayoutChassis->setColumnStretch(5,5);

    changementDeNom = false;
    changementEspaceKill = false;

//    QScrollArea * srcollTabCrea = new QScrollArea();
//    srcollTabCrea->setWidget(ui->tabChassi);
//    srcollTabCrea->show();
}

// // // // // // // // // // // // // //
              // SLOTS //
// // // // // // // // // // // // // //

//Slot appelé quand le bouton d'ajout de servo est pressé, crée tout les widget associés au servo créé et appelle les fonction de mise à jour adéquat
void FormMZB::ajoutServoPressed(void)
{
    //TODO condition de sortie, par exemple si le servo précédent n'est pas rempli

    nbServoCrees++;

    servo *servoCourant = new servo;

    listServo.push_back(servoCourant);


    nbServo++;

    servoCourant->ligneUI = nbServo;

    // // //

    QLineEdit * nomCourant = new QLineEdit(); //Là où met le nom du servo, exemple "Bras Gauche"
    servoCourant->nom = nomCourant;
    nomCourant->setText("Servo " + QString::number(nbServoCrees));
    mappageWidget(nomCourant, nomCourant, SIGNAL(editingFinished ()), SLOT(nomChange(QWidget*)));

    // // //

    QComboBox * localisationCourant = new QComboBox(); //Choix de la localisation, exemple "Torse"
    servoCourant->localisation = localisationCourant;
    majComboBox(localisationCourant, "Localisation Servo");
    localisationCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    mappageWidget(localisationCourant, localisationCourant, SIGNAL(activated(int)), SLOT(changeComboBox(QWidget*)));

    // // //

    QComboBox * typeServoCourant = new QComboBox(); //Choix du type de servo, exemple "Striker"
    servoCourant->typeServo = typeServoCourant;
    majComboBox(typeServoCourant, "Servo Torse");
    typeServoCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    mappageWidget(typeServoCourant, typeServoCourant, SIGNAL(activated(int)), SLOT(changeComboBox(QWidget*)));

    // // //

    QSpinBox * espaceCourant = new QSpinBox(); //Nombre d'espace disponible dans le servo, exemple "5"
    servoCourant->espace = espaceCourant;
    espaceCourant->findChild<QLineEdit*>()->setReadOnly(true);
    espaceCourant->setSingleStep(2);
    espaceCourant->setMinimum(1);
    mappageWidget(espaceCourant, espaceCourant, SIGNAL(valueChanged(int)), SLOT(slotEspaceChange(QWidget*)));
    //TODO ? virer la possibilité de changer avec la molette pour les spinbox

    // // //
    // TODODODODO gérer le signal de changement avec le booléen changementEspaceKill et la différence entre la valeur d'origine de la ComboBox et la valeur actuelle de la SpinBox (ce qui permet de se passer d'une variable intermédiaire)

    QSpinBox * killServoCourant = new QSpinBox(); //Nombre d'espace disponible dans le servo, exemple "5"
    servoCourant->killServo = killServoCourant;
    killServoCourant->findChild<QLineEdit*>()->setReadOnly(true);
    killServoCourant->setMinimum(1);
    mappageWidget(killServoCourant, killServoCourant, SIGNAL(valueChanged(int)), SLOT(slotKillChange(QWidget*)));

    // // //

    QComboBox * typeBlindageCourant = new QComboBox();//Choix du type de blindage du servo, exemple "Striker"
    servoCourant->typeBlindage = typeBlindageCourant;
    majComboBox(typeBlindageCourant, "Blindage");
    typeBlindageCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    mappageWidget(typeBlindageCourant, typeBlindageCourant, SIGNAL(activated(int)), SLOT(changeComboBox(QWidget*)));

    //TODO Connecter le signal de changement à un slot mettant à jour les PB

    // // //

    QLabel * killBlindageCourant = new QLabel("PB : 0"); //Nombre de Kills de blindage du type de blindage, exemple "5"
    servoCourant->killBlindage = killBlindageCourant;

    // // //

    QPushButton * suppServoCourant = new QPushButton("-");
    servoCourant->suppServo = suppServoCourant;

    mappageWidget(suppServoCourant, suppServoCourant, SIGNAL(released()), SLOT(suppServoPressed(QWidget*)));

    // // //

    affichageFixe();

    affichageServo(servoCourant);

    majTypeBlindage(servoCourant);
    majTypeServo(servoCourant);

    changeComboBox(typeServoCourant); //Vérifier que c'est bien là que ça va

}


//Slot appelé quand le bouton de suppression de servo est appuyé, il appelle la fonction de suppression.
void FormMZB::suppServoPressed(QWidget* suppServoPB)
{

    servo * servoASupprimer = new servo;

    for (int ligne=0 ; ligne < listServo.size() ; ligne ++)
    {
        if (suppServoPB == listServo[ligne]->suppServo) servoASupprimer = listServo[ligne];
    }

    suppServo(servoASupprimer);

  //TODO suppression de la QList correpondant au servo et de chacun des QWidjet qui la constitue + Supression de tout les éléments rattachés à ce servo
  //TODO delete du servo supprimé
}


//Slot appelé quand le signal de changement de la comboBox "cbChange" est émis, il appelle les fonction adéquat en fonction de la comboBox changée
void FormMZB::changeComboBox(QWidget* cbChange)
{
    servo * servoAMaj = new servo;

    for (int ligne=0 ; ligne < listServo.size() ; ligne ++)
    {
        if (cbChange == listServo[ligne]->localisation || cbChange == listServo[ligne]->typeServo || cbChange == listServo[ligne]->typeBlindage)
        {
            servoAMaj = listServo[ligne];
        }
    }

    //Si on change la localisation on change la CB type qui en dépend
    if (cbChange == servoAMaj->localisation)
    {
        majComboBox( servoAMaj->typeServo , QString("Servo %1").arg(servoAMaj->localisation->currentText()) );
        majTypeServo(servoAMaj);
    }

    if (cbChange == servoAMaj->typeServo) majTypeServo(servoAMaj);

    if (cbChange == servoAMaj->typeBlindage) majTypeBlindage(servoAMaj);
}


//Slot appelé quand le signal de validation du label "nom" est émis
void FormMZB::nomChange(QWidget* ligneNom)
{
    for (int i=0 ; i<listServo.size() ; i++)
    {
        if(listServo[i]->nom == ligneNom) ckeckNomServo(i);
    }
}


//Slot appelé quand le signal de changement de la spinBox "espace" est émis, met à jour les valeurs de kill, d'espace et de pc adéquates
void FormMZB::slotEspaceChange(QWidget* espaceChange)
{
    if(!changementEspaceKill)
    {
        changementEspaceKill = true;

        servo * servoEspaceChange = new servo; //TODO delete du pointeur de servo

        for (int ligne=0 ; ligne < listServo.size() ; ligne ++)
        {
            if (espaceChange == listServo[ligne]->espace) servoEspaceChange = listServo[ligne];
        }

        int changement = (servoEspaceChange->espace->value() - servoEspaceChange->espaceDispo);

        // Si on passe à 0 Kill ou si on n'a pas assez d'espace pour en perdre
        if(abs(changement) != 2 || ( servoEspaceChange->killDispo - ((changement)/2) ) < 1)
        {
            servoEspaceChange->espace->setValue(servoEspaceChange->espaceDispo);
        }
        else
        {
            servoEspaceChange->killDispo = servoEspaceChange->killDispo - ( (changement)/2 ) ;
            servoEspaceChange->killServo->setValue(servoEspaceChange->killDispo);

            servoEspaceChange->espaceDispo = servoEspaceChange->espace->value();

            //Si la valeur de départ ou d'arriver est inférieur à la valeur de départ alors ça coûte des PC
            if (servoEspaceChange->espaceDispo < servoEspaceChange->espaceBase || servoEspaceChange->espaceDispo - changement < servoEspaceChange->espaceBase)
            {
                servoEspaceChange->pcServo -= (changement) ;
            }
        }

        majMecha();

        changementEspaceKill = false;
    }
}


//Slot appelé quand le signal de changement de la spinBox "killServo" est émis, met à jour les valeurs de kill, d'espace et de pc adéquates
void FormMZB::slotKillChange(QWidget* killChange)
{
    if(!changementEspaceKill)
    {
        changementEspaceKill = true;

        servo * servoKillChange = new servo; //TODO delete du pointeur de servo

        for (int ligne=0 ; ligne < listServo.size() ; ligne ++)
        {
            if (killChange == listServo[ligne]->killServo) servoKillChange = listServo[ligne];
        }

        int changement = (servoKillChange->killServo->value() - servoKillChange->killDispo);

        // Si on passe à 0 Kill ou si on n'a pas assez d'espace pour en perdre
        if(abs(changement) != 1 || ( servoKillChange->espaceDispo - ((changement)*2) ) < 1)
        {
            servoKillChange->killServo->setValue(servoKillChange->killDispo);
        }
        else
        {
            servoKillChange->espaceDispo = servoKillChange->espaceDispo - ( (changement)*2 ) ;
            servoKillChange->espace->setValue(servoKillChange->espaceDispo);

            servoKillChange->killDispo = servoKillChange->killServo->value();

            //Si la valeur de départ ou d'arriver est inférieur à la valeur de départ alors ça coûte des PC
            if (servoKillChange->killDispo > servoKillChange->killBase || servoKillChange->killDispo - (changement) > servoKillChange->killBase)
            {
                servoKillChange->pcServo += (changement*2) ;
            }
        }

        majMecha();

        changementEspaceKill = false;
    }
}


//Slot appelé lors de l'ajout d'une nouvelle roue
void FormMZB::ajoutRoue(void)
{

}


//Slot appelé lors de l'ajout d'une nouvelle chenille
void FormMZB::ajoutChenille(void)
{

}


//Slot appelé lors de l'ajout d'un nouveau Système à Effet de Sol
void FormMZB::ajoutSES(void)
{

}


void FormMZB::ajoutPropulseur(void)
{

}

// // // // // // // // // // // // // //
             // MISE A JOUR //
// // // // // // // // // // // // // //

//Met à jour des variables dépendant de la comboBox "typeServo" quand cette dernière est modifiée
void FormMZB::majTypeServo(servo* servoMaj)
{
    QStringList lectureTypeServo = (servoMaj->typeServo->currentText()).split(" ; ");
    QStringList::iterator lectureIt;

    for (lectureIt = lectureTypeServo.begin() ; lectureIt != lectureTypeServo.end() ; lectureIt++) //Maj des différentes variables du servo
    {
        if( (*lectureIt).endsWith(" PC") ) servoMaj->pcServo = ( ( ( (*lectureIt).remove("PC") ) ).toDouble() );

        if( (*lectureIt).endsWith(" Poids") ) servoMaj->poidsServo = ( (*lectureIt).remove("Poids") ).toDouble();

        if( (*lectureIt).endsWith(" K") )
        {
            servoMaj->killDispo = ( (*lectureIt).remove("K") ).toInt();
            servoMaj->killBase = servoMaj->killDispo;
            servoMaj->killServo->setValue(servoMaj->killDispo);
        }

        if( (*lectureIt).endsWith(" Espace") )
        {
            servoMaj->espaceDispo = ( (*lectureIt).remove("Espace") ).toInt();
            servoMaj->espaceBase = servoMaj->espaceDispo;
            servoMaj->espace->setValue(servoMaj->espaceDispo);
        }
    }

    majMecha();
}


//Met à jour des variables dépendant de la comboBox "typeBlindage" quand cette dernière est modifiée
void FormMZB::majTypeBlindage(servo* servoMaj) //Fonction appelé au changement de la comboBox
{
    QStringList lectureTypeBlidage = (servoMaj->typeBlindage->currentText()).split(" ; ");
    QStringList::iterator lectureIt;

    for (lectureIt = lectureTypeBlidage.begin() ; lectureIt != lectureTypeBlidage.end() ; lectureIt++) //Maj des différentes variables du servo
    {
        if( (*lectureIt).endsWith(" PC") )
        {
            servoMaj->pcBlindage = ( ( ( (*lectureIt).remove("PC") ) ).toDouble() );
            servoMaj->killBlindage->setText(QString("PB : %1").arg(servoMaj->pcBlindage) );
        }

        if( (*lectureIt).endsWith(" Poids") ) servoMaj->poidsBlindage = ( (*lectureIt).remove("Poids") ).toDouble();
    }

    majMecha();
}


//Met à jour les variables du mecha et leur affichage
void FormMZB::majMecha()
{
    poidsChassis = 0 ;
    pcChassis = 0;
    poidsTot = 0 ;
    pcTot = 0;

    for(int i=0 ; i <listServo.size() ; i++)
    {
        poidsChassis = poidsChassis + listServo[i]->poidsServo + listServo[i]->poidsBlindage;

        pcChassis = pcChassis + listServo[i]->pcServo + listServo[i]->pcBlindage;
    }

    poidsTot = poidsChassis ;
    pcTot = pcChassis;
    //TODO Compter le poids de chaque composant

    //TODO Maj MV air/sol et VM

    ui->labelPoidsChassis->setText(QString("Poids Chassis : %1").arg(poidsChassis));
    ui->labelPCChassis->setText(QString("PC Chassis : %1").arg(pcChassis));

    ui->labelPoids->setText(QString("Poids : %1").arg(poidsTot));
    ui->labelPC->setText(QString("PC : %1").arg(pcTot));

    QSettings dataBase(QString("../MektonZBuilderUI/mektonZetaBuilderData.ini"),QSettings::IniFormat);

    dataBase.beginGroup("Caracteristiques Mecha");
    QStringList clesCarac = dataBase.allKeys();
    QStringList carac;
    clesCarac.pop_front();

    for (int i=0 ; i<clesCarac.size() ; i++)
    {
        carac = ( ( dataBase.value(clesCarac[i]) ).toString() ).split("_");

        clesCarac[i].remove(0,3);
        std::cout << clesCarac[i].toStdString() << std::endl; //TODO ligne à virer


        if (poidsTot >= clesCarac[i].toDouble())
        {
            mvtSol = carac[0].toDouble();
            valMan = carac[1].toDouble();
        }
    }

    dataBase.endGroup();

    ui->labelMVSol->setText(QString("MV au Sol : %1").arg(mvtSol));
    ui->labelVM->setText(QString("VM : %1").arg(valMan));

}


//Rempli la comboBox "cbMaj" avec les élément adéquat récupérer dans le .ini à la clef "clef"
void FormMZB::majComboBox(QComboBox *cbMaj, QString clef)
{
    int nbrItem = cbMaj->count();

    for (int i=0 ; i<nbrItem ; i++)
    {
        cbMaj->removeItem(0);
    }

    QSettings dataBase(QString("../MektonZBuilderUI/mektonZetaBuilderData.ini"),QSettings::IniFormat);

    //TODO why not mettre un separateur définie dans le .ini

    dataBase.beginGroup(clef);
    QStringList listType = dataBase.allKeys();
    if (!listType.isEmpty())
    {

        QStringList::iterator itListType;
        QStringList colone;
        QString ligneCB;

        itListType = listType.begin();
        colone = ( ( dataBase.value(*itListType) ).toString() ).split("_");
        int nbCol = colone.size();
        //if(colone.isEmpty()) nbCol = 0; //Pour ne pas afficher de ';' si il n'y a pas de paramètre
        itListType++;

        listType.pop_front();//Retire de la list la clé Colone

        QStringList carac;
        for (itListType = listType.begin() ; itListType != listType.end() ; itListType++)
        {
            ligneCB = (dataBase.value(*itListType)).toString();
            carac = ligneCB.split("_");
            ligneCB = "";
            for (int i=0 ; i<nbCol ; i++)
            {
                if ( ! (carac[i].isEmpty() && colone[i].isEmpty()) )ligneCB = ligneCB + " ; " + carac[i] + " " + colone[i];
            }
            ligneCB = (*itListType) + ligneCB;

            ligneCB = ligneCB.right(ligneCB.size()-3);

            cbMaj->addItem(ligneCB);
        }
    }
    dataBase.endGroup();
    cbMaj->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}


//Supprime les différents widget affilié à servoASupprimer puis fait un delete sur le pointeur et met à jour
void FormMZB::suppServo(servo* servoASupprimer)
{
    int ligneList = (servoASupprimer->ligneUI)-1; //Ligne à supprimer dans listServo

    delete servoASupprimer->nom;
    delete servoASupprimer->localisation;
    delete servoASupprimer->typeServo;
    delete servoASupprimer->espace;
    delete servoASupprimer->killServo;
    delete servoASupprimer->typeBlindage;
    delete servoASupprimer->killBlindage;
    delete servoASupprimer->suppServo;

    nbServo--;

    if (listServo.size() > servoASupprimer->ligneUI)
    {
        for (int ligne = servoASupprimer->ligneUI ; ligne < listServo.size() ; ligne++) //un peu moche mais Ok
        {
            listServo[ligne]->ligneUI = (listServo[ligne]->ligneUI) - 1;
            affichageServo(listServo[ligne]);
        }
    }

    affichageFixe();

    delete servoASupprimer;
    servoASupprimer = 0;

    listServo.removeAt(ligneList);

    majMecha();
}

//Ajoute les différents Widget affilié au servo "servoAAfficher" à la fenêtre dans l'espace prévu à cet effet
void FormMZB::affichageServo(servo* servoAAfficher)
{
    ui->gridLayoutChassis->addWidget(servoAAfficher->nom,servoAAfficher->ligneUI,0);
    ui->gridLayoutChassis->addWidget(servoAAfficher->localisation,servoAAfficher->ligneUI,1);
    ui->gridLayoutChassis->addWidget(servoAAfficher->typeServo,servoAAfficher->ligneUI,2);
    ui->gridLayoutChassis->addWidget(servoAAfficher->espace,servoAAfficher->ligneUI,3);
    ui->gridLayoutChassis->addWidget(servoAAfficher->killServo,servoAAfficher->ligneUI,4);
    ui->gridLayoutChassis->addWidget(servoAAfficher->typeBlindage,servoAAfficher->ligneUI,5);
    ui->gridLayoutChassis->addWidget(servoAAfficher->killBlindage,servoAAfficher->ligneUI,6);
    ui->gridLayoutChassis->addWidget(servoAAfficher->suppServo,servoAAfficher->ligneUI,7);
}


//Place les affichages et metteur en formes qui ne dépendent pas d'un servo mais qui sont déplacé quand on a un servo en plus ou en moins
void FormMZB::affichageFixe()
{
    ui->gridLayoutChassis->addItem(ui->spacerChassis, nbServo+2 , 0);
    ui->gridLayoutChassis->setRowStretch(nbServo + 2,5);
    ui->gridLayoutChassis->setRowStretch(nbServo + 1,0);
    ui->gridLayoutChassis->addWidget(ui->pushButtonAjoutServo , nbServo+1 , 0);
    ui->gridLayoutChassis->addWidget(ui->labelPoidsChassis, nbServo+1 , 6);
    ui->gridLayoutChassis->addWidget(ui->labelPCChassis, nbServo+1 , 7);
}


//Vérifie que le nom du servo numéro "i" ne corresond à aucun autre nom d'aucun autre servo
void FormMZB::ckeckNomServo(int i)
{
    if(changementDeNom == false)
    {
        for(int j=0 ; j<listServo.size() ; j++)
        {
            if(j != i && listServo[i]->nom->text() == listServo[j]->nom->text())
            {
                changementDeNom = true;

                QMessageBox::information(this, "Nom incorrect", "Ce nom de servo est déjà pris");

                nbServoCrees++;
                listServo[i]->nom->setText("Servo " + QString::number(nbServoCrees));

                changementDeNom = false;
            }
        }
    }
}

// // // // // // // // // // // // // //
             // A Classer ... //
// // // // // // // // // // // // // //

//Permet de connecter le signal "signal" émis par "émetteur" au slot "slot" avec comme paramètre le QWidget* "émis"
void FormMZB::mappageWidget(QWidget* emetteur, QWidget* emis, const char* signal, const char* slot)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mapped (QWidget*) ), this, slot);
    signalMapper->setMapping(emetteur, emis);
    QObject::connect(emetteur, signal, signalMapper, SLOT(map()));
}

/* A VIRER
void FormMZB::majServo(servo* servoMaj)
{
    QStringList lectureTypeServo = (servoMaj->typeServo->currentText()).split(" ; ");
    QStringList::iterator lectureIt;

    for (lectureIt = lectureTypeServo.begin() ; lectureIt != lectureTypeServo.end() ; lectureIt++) //Maj des différentes variables du servo
    {
        if( (*lectureIt).endsWith(" PC") ) servoMaj->pcServo = ( ( ( (*lectureIt).remove("PC") ) ).toDouble() );

        if( (*lectureIt).endsWith(" Poids") ) servoMaj->poidsServo = ( (*lectureIt).remove("Poids") ).toDouble();

        if( (*lectureIt).endsWith(" K") )
        {
            servoMaj->killDispo = ( (*lectureIt).remove("K") ).toInt();
            servoMaj->killBase = servoMaj->killDispo;
            servoMaj->killServo->setValue(servoMaj->killDispo);
        }

        if( (*lectureIt).endsWith(" Espace") )
        {
            servoMaj->espaceDispo = ( (*lectureIt).remove("Espace") ).toInt();
            servoMaj->espaceBase = servoMaj->espaceDispo;
            servoMaj->espace->setValue(servoMaj->espaceDispo);
        }
    }

    QStringList lectureTypeBlidage = (servoMaj->typeBlindage->currentText()).split(" ; ");

    for (lectureIt = lectureTypeBlidage.begin() ; lectureIt != lectureTypeBlidage.end() ; lectureIt++) //Maj des différentes variables du servo
    {
        if( (*lectureIt).endsWith(" PC") )
        {
            servoMaj->pcBlindage = ( ( ( (*lectureIt).remove("PC") ) ).toDouble() );
            servoMaj->killBlindage->setText(QString("PB : %1").arg(servoMaj->pcBlindage) );
        }

        if( (*lectureIt).endsWith(" Poids") ) servoMaj->poidsBlindage = ( (*lectureIt).remove("Poids") ).toDouble();
    }

    majMecha();
}

*/
