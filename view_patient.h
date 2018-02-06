#ifndef PATIENT_PHOTOS_H
#define PATIENT_PHOTOS_H

#include <QDialog>
#include <QString>
#include <vector>
#include <QLabel>
#include <QDateEdit>

#include "patient.h"

class Photo;
class PatientsWindow;

#define ICON_WIDTH 200
#define ICON_HEIGHT 200

namespace Ui {
class PatientPhotos;
}

class PatientPhotos : public QDialog
{
    Q_OBJECT

public:
    explicit PatientPhotos(Patient * patient,
                           QWidget *parent = 0);
    ~PatientPhotos();

private slots:
    void on_btnNewPhoto_clicked();
    void on_btnDeletePhoto_clicked();

    void on_btnUpdatePatient_clicked();

    void CheckChangesFname(const QString & _arg);
    void CheckChangesLname(const QString & _arg);
    void CheckChangesFathername(const QString & _arg);
    void CheckChangesAddress(const QString & _arg);
    void CheckChangesBDate(const QDate & _arg);
    void CheckChangesArriveDate(const QDate & _arg);
    void CheckChangesLeaveDate(const QDate & _arg);

    void on_btnSetLeaveDate_clicked();

    void on_btnUnstageChanges_clicked();

private:

    void AddItemToWidget(Photo * _p);
    void SetLineEditFields();

    Ui::PatientPhotos *ui;
    //  Pointer to parent window
    PatientsWindow * m_pPatientsWindow;
    //  Header for table list
    QStringList m_TableHeader;

    QLabel * myLabel;
    Patient * m_pCurrentPatient;
};


#endif // PATIENT_PHOTOS_H
