#ifndef FORMMZB_H
#define FORMMZB_H

#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QSettings>
#include <QString>
#include <QSignalMapper>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>

namespace Ui
{
    class FormMZB;
}


class FormMZB : public QWidget
{
    Q_OBJECT

    public:
        explicit FormMZB(QWidget *parent = 0);
        ~FormMZB();

    private slots :

        void ajoutServoPressed(void);
        void ajoutRoue(void);
        void ajoutChenille(void);
        void ajoutSES(void);
        void ajoutPropulseur(void);
        void suppServoPressed(QWidget* suppServoPB);
        void changeComboBox(QWidget* cbChange);
        void majComboBox(QComboBox * cbMaj, QString clef);
        void nomChange(QWidget*);
        void slotEspaceChange(QWidget*);
        void slotKillChange(QWidget*);


    signals :


    private:
        Ui::FormMZB *ui;
        int nbServo;
        int nbServoCrees; //Comprend aussi les servo supprimés
        double poidsTot;
        double poidsChassis;
        double pcChassis;
        double pcTot;
        double mvtSol;
        double mvtAir;
        double valMan; //Valeur de Manoevre

        //double correspondancePoidsMvtMan[3][10]; // Tableau de correspondance entre le poids et le mouvement ou la valeur de manoeuvre

        bool changementDeNom; //Vaut true quand le check nom change le nom
        bool changementEspaceKill; //Vaut true quand la valeur d'un des SpinBox du Chassis est changé via les flèches et lors de l'attribution du type de chassis

        typedef struct {QLineEdit* nom;
                        QComboBox* localisation;
                        QComboBox* typeServo;
                        double pcServo;
                        QSpinBox* espace;
                        int espaceDispo ;
                        int espaceBase ;
                        QSpinBox* killServo;
                        int killDispo;
                        int killBase;
                        double poidsServo;
                        int niveauServo; // Exemple "2" pour Léger, "5" pour Striker Lourd
                        QComboBox* typeBlindage;
                        QLabel* killBlindage;
                        double pcBlindage;
                        double poidsBlindage;
                        QPushButton* suppServo;
                        int ligneUI;}servo; //Le paramètre ligneUI n'a rien à voir avec le servo à proprement parler, c'est pour faire le lien avec l'UI

                        //ATTENTION\\ Si ajout de pointeur, penser à faire le delete dessus dans suppServo(servo*)

        typedef struct {QString* localisation;
                        double pc;
                        QSpinBox* killRoue;}roue;

        typedef struct {QString* localisation;
                        double pc;
                        QSpinBox* kill;}chenille;

        typedef struct {QString* localisation;
                        double pc;
                        QSpinBox* mouvement;}ses;

        typedef struct {QString* localisation;
                        double pc;
                        QSpinBox* mouvement;}propulseur;

        QList< servo* > listServo;

        void ckeckNomServo(int i);

        // // Mise à jour // //
        void majMecha();

        void majServo(servo* servoMaj);

        void majTypeServo(servo* servoMaj);

        void majTypeBlindage(servo* servoMaj);

        void affichageServo(servo *servoAAfficher);

        void affichageFixe();

        void suppServo(servo* servoASupprimer);
        // // /// // //

        void initUI();

        void mappageWidget(QWidget *emetteur, QWidget *emis, const char *signal, const char *slot);
};

#endif // FORMMZB_H
