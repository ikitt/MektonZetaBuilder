#include "formmzb.h"
#include "ui_formmzb.h"

#include <iostream>



/* CONSTRUCTEUR */

FormMZB::FormMZB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMZB)
{
    ui->setupUi(this);

    connect(ui->pushButtonAjoutServo, SIGNAL( released() ), this, SLOT( ajoutServoPressed() ) );


    this->initUI();
}


/* DESTRUCTEUR */

FormMZB::~FormMZB()
{
    //TODO ajouter la suppression des servo de listServo pour être propre

    delete ui;
}

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
}

// // // // // // // // // // // // // //
              // SLOTS //
// // // // // // // // // // // // // //

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
    //TODO Connecter le signal de perte de focus (ou de changement) à un slot qui vérifie que le nom saisi n'existe pas déjà, si déjà pris alors oblige à modifier

    // // //

    QComboBox * localisationCourant = new QComboBox(); //Choix de la localisation, exemple "Torse"
    servoCourant->localisation = localisationCourant;
    majComboBox(localisationCourant, "Localisation Servo");
    localisationCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QSignalMapper *signalMapperLoc = new QSignalMapper(this);
    QObject::connect(signalMapperLoc, SIGNAL(mapped (QWidget*) ), this, SLOT(changeComboBox(QWidget*)));
    signalMapperLoc->setMapping(localisationCourant, localisationCourant);
    QObject::connect(localisationCourant, SIGNAL(activated(int)), signalMapperLoc, SLOT(map()));

    //TODO Initialisation de la combo box
    //TODO Connecter le signal de changement à un slot mettant à jour le reste de la ligne

    // // //

    QComboBox * typeServoCourant = new QComboBox(); //Choix du type de servo, exemple "Striker"
    servoCourant->typeServo = typeServoCourant;
    majComboBox(typeServoCourant, "Servo Torse");
    typeServoCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QSignalMapper *signalMapperTS = new QSignalMapper(this);
    QObject::connect(signalMapperTS, SIGNAL(mapped (QWidget*) ), this, SLOT(changeComboBox(QWidget*)));
    signalMapperTS->setMapping(typeServoCourant, typeServoCourant);
    QObject::connect(typeServoCourant, SIGNAL(activated(int)), signalMapperTS, SLOT(map()));

    //TODO Fonction d'nitialisation de la combo box appelée après choix de la localisation
    //TODO Connecter le signal de changement à un slot mettant à jour le reste de la ligne et appelant majPoids()

    // // //

    QSpinBox * espaceCourant = new QSpinBox(); //Nombre d'espace disponible dans le servo, exemple "5"
    servoCourant->espace = espaceCourant;
    //TODO connecter les signaux + et - et vérifier que ce n'est pas éditable directement

    // // //

    QLabel * killServoCourant = new QLabel("Kills : "); //Nombre de Kills du servo, exemple "5"
    servoCourant->killServo=killServoCourant;

    // // //

    QComboBox * typeBlindageCourant = new QComboBox();//Choix du type de blindage du servo, exemple "Striker"
    servoCourant->typeBlindage = typeBlindageCourant;
    majComboBox(typeBlindageCourant, "Blindage");
    typeBlindageCourant->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QSignalMapper *signalMapperTB = new QSignalMapper(this);
    QObject::connect(signalMapperTB, SIGNAL(mapped (QWidget*) ), this, SLOT(changeComboBox(QWidget*)));
    signalMapperTB->setMapping(typeBlindageCourant, typeBlindageCourant);
    QObject::connect(typeBlindageCourant, SIGNAL(activated(int)), signalMapperTB, SLOT(map()));

    //TODO Connecter le signal de changement à un slot mettant à jour les PB

    // // //

    QLabel * killBlindageCourant = new QLabel("PB : 0"); //Nombre de Kills de blindage du type de blindage, exemple "5"
    servoCourant->killBlindage = killBlindageCourant;

    // // //

    QPushButton * suppServoCourant = new QPushButton("-");
    servoCourant->suppServo = suppServoCourant;

    QSignalMapper *signalMapperSupp = new QSignalMapper(this);
    QObject::connect(signalMapperSupp, SIGNAL(mapped (QWidget*) ), this, SLOT(suppServoPressed(QWidget*)));
    signalMapperSupp->setMapping(suppServoCourant, suppServoCourant);
    QObject::connect(suppServoCourant, SIGNAL(released()), signalMapperSupp, SLOT(map()));

    // // //

    affichageFixe();

    affichageServo(servoCourant);

    majServo(servoCourant);

}


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
    if (cbChange == servoAMaj->localisation) majComboBox( servoAMaj->typeServo , QString("Servo %1").arg(servoAMaj->localisation->currentText()) );

    majServo(servoAMaj);
}


// // // // // // // // // // // // // //
             // MISE A JOUR //
// // // // // // // // // // // // // //
void FormMZB::majServo(servo* servoMaj)
{
    QStringList lectureTypeServo = (servoMaj->typeServo->currentText()).split(" ; ");

    servoMaj->pcServo = ( ( (lectureTypeServo[1].remove("PC")) ).toDouble() );
    servoMaj->poidsServo = ( lectureTypeServo[lectureTypeServo.size()-1].remove("Poids") ).toDouble();

    QStringList lectureTypeBlidage = (servoMaj->typeBlindage->currentText()).split(" ; ");

    servoMaj->pcBlindage = ( ( (lectureTypeBlidage[1].remove("PC")) ).toDouble() );
    servoMaj->killBlindage->setText(QString("PB : %1").arg( (lectureTypeBlidage[1].remove("PC")).toDouble() ) );
    servoMaj->poidsBlindage = ( lectureTypeBlidage[lectureTypeBlidage.size()-1].remove("Poids") ).toDouble();

    for(int i=0 ; i<lectureTypeServo.size() ; i++)
    {

    }

    //TODO maj de espaceDispo
    majMecha();
}

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


}

void FormMZB::majComboBox(QComboBox *cbMaj, QString clef)
{
    int nbrItem = cbMaj->count();
    std::cout << nbrItem << "items à virer" << std::endl;
    for (int i=0 ; i<nbrItem ; i++)
    {
        std::cout << "item " << i << " virer" << std::endl;
        cbMaj->removeItem(0);
    }

    //TODO doit dépendre du type de servo
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
        colone = ( ( dataBase.value(*itListType) ).toString() ).split(" ");
        int nbCol = colone.size();
        //if(colone.isEmpty()) nbCol = 0; //Pour ne pas afficher de ';' si il n'y a pas de paramètre
        itListType++;

        listType.pop_front();//Retire de la list la clé Colone

        QStringList carac;
        for (itListType = listType.begin() ; itListType != listType.end() ; itListType++)
        {
            ligneCB = (dataBase.value(*itListType)).toString();
            carac = ligneCB.split(" ");
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

    //TODO Supprimer tout les composants attachés au servo
}

void FormMZB::affichageServo(servo* servoAAfficher)
{
    ui->gridLayoutChassis->addWidget(servoAAfficher->nom,servoAAfficher->ligneUI,0);
    ui->gridLayoutChassis->addWidget(servoAAfficher->localisation,servoAAfficher->ligneUI,1);
    ui->gridLayoutChassis->addWidget(servoAAfficher->typeServo,servoAAfficher->ligneUI,2);
    ui->gridLayoutChassis->addWidget(servoAAfficher->killServo,servoAAfficher->ligneUI,3);
    ui->gridLayoutChassis->addWidget(servoAAfficher->espace,servoAAfficher->ligneUI,4);
    ui->gridLayoutChassis->addWidget(servoAAfficher->typeBlindage,servoAAfficher->ligneUI,5);
    ui->gridLayoutChassis->addWidget(servoAAfficher->killBlindage,servoAAfficher->ligneUI,6);
    ui->gridLayoutChassis->addWidget(servoAAfficher->suppServo,servoAAfficher->ligneUI,7);
}

void FormMZB::affichageFixe()
{
    ui->gridLayoutChassis->addItem(ui->spacerChassis, nbServo+2 , 0);
    ui->gridLayoutChassis->setRowStretch(nbServo + 2,5);
    ui->gridLayoutChassis->setRowStretch(nbServo + 1,0);
    ui->gridLayoutChassis->addWidget(ui->pushButtonAjoutServo , nbServo+1 , 0);
    ui->gridLayoutChassis->addWidget(ui->labelPoidsChassis, nbServo+1 , 6);
    ui->gridLayoutChassis->addWidget(ui->labelPCChassis, nbServo+1 , 7);
}

/* A VIRER


*/
