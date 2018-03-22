#include "new_patient.h"
#include "ui_new_patient.h"
#include "patients_window.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

NewPatient::NewPatient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPatient)
{
    ui->setupUi(this);
}

NewPatient::~NewPatient()
{
    delete ui;
}

bool NewPatient::CheckInputInvariants(std::vector<QLineEdit*> & _lst) {
    bool isInt = false;
    bool no_error = true;
    for (QLineEdit * _line: _lst) {
        _line->text().toInt(&isInt);
        if (_line->text().isEmpty() || isInt) {
            _line->setStyleSheet("border: 1px solid red");
            no_error = false;
        }
    }
    if (!no_error)
        QMessageBox::critical(this, tr("New patient"), tr("Fill all text fields with some text!"),
                              QMessageBox::Ok);
    return no_error;
}

bool NewPatient::CreatePatient() {
    std::vector<QLineEdit*> vec{ui->lineEdit_FirstName,
                                ui->lineEdit_LastName,
                                ui->lineEdit_FatherName,
                                ui->lineEdit_Address};
    if (!CheckInputInvariants(vec))
        return false;
    QString fname = ui->lineEdit_FirstName->text();
    QString lname = ui->lineEdit_LastName->text();
    QString father_name = ui->lineEdit_FatherName->text();
    QString address = ui->lineEdit_Address->text();
    QDate bdate { ui->dateEdit_BirthDate->date() };
    QDate arrDate { ui->dateEdit_ArriveDate->date() };
    QDate * leaveDate;
    if (ui->dateEdit_LeaveDate->isEnabled())
        leaveDate = new QDate( ui->dateEdit_LeaveDate->date() );
    else
        leaveDate = nullptr;

    PatientsWindow * win =  dynamic_cast<PatientsWindow *>(parentWidget());
    try {
        Patient * patient = new Patient(fname, lname, father_name, address, bdate, arrDate, *leaveDate);
//        win->InsertPatient(*p);
        DBManager::instance().patientDao.addPatient(*patient);
        //  TODO: signal/slots for PatientsModel
    }
    catch (Patient_Ex::BirthAndArriveDateLogicError & _ex) {
        QString exc_msg{};
        exc_msg += _ex.message() + "\n";
        exc_msg += "Press OK to cancel person creation\n";
        QMessageBox::StandardButton res = QMessageBox::critical(
                    this, "Creation error",
                    exc_msg,
                    QMessageBox::Ok | QMessageBox::Cancel);
        if (res == QMessageBox::Ok)
            close();
    }
    return true;
}

void NewPatient::on_lineEdit_FirstName_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->lineEdit_FirstName->setStyleSheet("border: 1px solid gray;");
}

void NewPatient::on_lineEdit_LastName_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->lineEdit_LastName->setStyleSheet("border: 1px solid gray;");
}

void NewPatient::on_lineEdit_FatherName_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->lineEdit_FatherName->setStyleSheet("border: 1px solid gray;");
}

void NewPatient::on_lineEdit_Address_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->lineEdit_Address->setStyleSheet("border: 1px solid gray;");
}

void NewPatient::on_pushButton_AddPatient_clicked(){
    if (CreatePatient())
        close();
}

void NewPatient::on_checkBox_LeaveDateIsDefined_stateChanged(int arg1){
    ui->dateEdit_LeaveDate->setDisabled(arg1);
}
