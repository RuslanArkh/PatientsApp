#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include <QException>
#include <QScopedPointer>
#include <QStringListModel>
#include <QStringList>
#include <QTableView>

#include <vector>


#include "photo.h"
#include "patient.h"
#include "patients_model.h"
#include "patients_filter_proxy_model.h"
#include "delete_button_delegate.h"


namespace Ui {
class MainWindow;
}


//  UI file is called mainwindow.ui
//  tvPatients stands for Table View Patients

class PatientsWindow
        : public QMainWindow
{
    Q_OBJECT

public:
    explicit PatientsWindow(QWidget *parent = 0);
    ~PatientsWindow();

    void InsertPatient(Patient & _p);
    void InsertPhoto(Photo & _photo);
    void UpdatePatient(Patient & _p);
    void DeletePhotoFromDB(int _id);

private slots:
    void on_btn_DeletePatient_clicked();

    void on_btn_NewPatient_clicked();
    void on_lineEdit_Search_textChanged(const QString &arg1);
    void on_checkBox_StillHere_stateChanged(int arg1);
    void on_tvPatients_doubleClicked(const QModelIndex &index);

    void on_checkBox_Left_stateChanged(int arg1);

    void on_tvPatients_clicked(const QModelIndex &index);

    void on_tvPatients_activated(const QModelIndex &index);

    void DeleteByIndex(const QModelIndex & index);

private:

    Ui::MainWindow *ui;

    PatientsModel * m_pModel;
    PatientsFilterProxyModel * m_pProxyModel;

    DeleteButtonDelegate * m_pButtonDelegate;
};

#endif // MAINWINDOW_H
