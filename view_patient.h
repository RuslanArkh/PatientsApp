#ifndef PATIENT_PHOTOS_H
#define PATIENT_PHOTOS_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QDateEdit>

#include <vector>
#include <memory>

#include "patient.h"
#include "photoloaderthread.h"

class Photo;
class PatientsWindow;

#define ICON_WIDTH 200
#define ICON_HEIGHT 200

namespace Ui {
class ViewPatient;
}

class ViewPatient : public QDialog
{
    Q_OBJECT

public:
    explicit ViewPatient(Patient * patient,
                           QWidget *parent = 0);
    ~ViewPatient();

private slots:

    void on_btnNewPhoto_clicked();
    void on_btnDeletePhoto_clicked();
    void on_btnUpdatePatient_clicked();
    void on_btnSetLeaveDate_clicked();
    void on_btnUnstageChanges_clicked();

    //  Used to add photos, which are loaded in another thread.
    void AddPhotoToWidget( Photo * p);
    void AddPhotoToVector( Photo * p);
    void EnablePhotoButtons();

public slots:

    void reject() override;

private:

    void AddItemToWidget(Photo * _p);
    void SetLineEditFields();

    Ui::ViewPatient *ui;
    //  Pointer to parent window
    PatientsWindow * m_pPatientsWindow;
    //  Header for table list
    QStringList m_TableHeader;

    QLabel * myLabel;
    Patient * m_pCurrentPatient;

    void SetControlButtonsState(bool is_enabled);

    PhotoLoaderThread * mLoaderThread;

};


#endif // PATIENT_PHOTOS_H
