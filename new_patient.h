#ifndef NEW_PATIENT_H
#define NEW_PATIENT_H

#include "patient.h"
#include <QDialog>
#include <vector>
#include <QLineEdit>

namespace Ui {
class NewPatient;
}


class NewPatient : public QDialog
{
    Q_OBJECT

public:
    explicit NewPatient(QWidget *parent = 0);
    ~NewPatient();

private slots:
    void on_pushButton_AddPatient_clicked();

    void on_lineEdit_FirstName_textChanged(const QString &arg1);

    void on_lineEdit_LastName_textChanged(const QString &arg1);

    void on_lineEdit_FatherName_textChanged(const QString &arg1);

    void on_lineEdit_Address_textChanged(const QString &arg1);

    void on_checkBox_LeaveDateIsDefined_stateChanged(int arg1);

private:
    Ui::NewPatient *ui;

    bool NoErrors(std::vector<QLineEdit*> & _lst);

    bool CreatePatient();
};

#endif // NEW_PATIENT_H
