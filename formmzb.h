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

namespace Ui {
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
    void suppServoPressed(QWidget* suppServoPB);
    void changeComboBox(QWidget* cbChange);

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

    typedef struct {QLineEdit* nom;
                    QComboBox* localisation;
                    QComboBox* typeServo;
                    double pcServo;
                    QSpinBox* espace;
                    int espaceDispo ;
                    QLabel* killServo;
                    double poidsServo;
                    int niveauServo; // Exemple "2" pour Léger, "5" pour Striker Lourd
                    QComboBox* typeBlindage;
                    QLabel* killBlindage;
                    double pcBlindage;
                    double poidsBlindage;
                    QPushButton* suppServo;
                    int ligneUI;}servo; //Le paramètre ligneUI n'a rien à voir avec le servo à proprement parler, c'est pour faire le lien avec l'UI
                    //ATTENTION\\ Si ajout de pointeur, penser à faire le delete dessus dans suppServo(servo*)

    QList< servo* > listServo;

    void initUI();

    void majMecha();

    void majServo(servo* servoMaj);

    void majComboBox(QComboBox * cbMaj, QString clef);
    void suppServo(servo* servoASupprimer);
    void affichageServo(servo *servoAAfficher);
    void affichageFixe();
};

#endif // FORMMZB_H
