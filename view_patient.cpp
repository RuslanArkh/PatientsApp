#include "view_patient.h"
#include "ui_view_patient.h"

#include "patients_window.h"
#include "imageview.h"

#include "photo.h"

#include <QPixmap>
#include <QIcon>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QListWidget>
#include <QSize>
#include <QList>
#include <QtGui>

#include <list>
#include <string>

PatientPhotos::PatientPhotos(Patient * patient,
                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientPhotos)
{
    ui->setupUi(this);
    m_pPatientsWindow = dynamic_cast<PatientsWindow *>(parentWidget());

    m_pCurrentPatient = patient;

    SetLineEditFields();

    ui->listWidget_ImageGallery->setViewMode(QListWidget::IconMode);
    ui->listWidget_ImageGallery->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));
    ui->listWidget_ImageGallery->setResizeMode(QListWidget::Adjust);

    for (Photo * _p: *m_pCurrentPatient->Photos())
        AddItemToWidget(_p);

    connect(this->ui->leFirstName, SIGNAL(textChanged(const QString &)),
            this, SLOT(CheckChangesFname(const QString &)));
    connect(this->ui->leLastName, SIGNAL(textChanged(const QString &)),
            this, SLOT(CheckChangesLname(const QString &)));
    connect(this->ui->leFatherName, SIGNAL(textChanged(const QString &)),
            this, SLOT(CheckChangesFathername(const QString &)));
    connect(this->ui->leAddress, SIGNAL(textChanged(const QString &)),
            this, SLOT(CheckChangesAddress(const QString &)));
    connect(this->ui->deBirthDate, SIGNAL(dateChanged(const QDate &)),
            this, SLOT(CheckChangesBDate(const QDate &)));
    connect(this->ui->deArriveDate, SIGNAL(dateChanged(const QDate &)),
            this, SLOT(CheckChangesArriveDate(const QDate &)));
    connect(this->ui->deLeaveDate, SIGNAL(dateChanged(const QDate &)),
            this, SLOT(CheckChangesLeaveDate(const QDate &)));
}

PatientPhotos::~PatientPhotos()
{
    delete ui;
}

void PatientPhotos::SetLineEditFields() {
    ui->leFirstName->setText(m_pCurrentPatient->GetFirstName());
    ui->leLastName->setText(m_pCurrentPatient->GetLastName());
    ui->leFatherName->setText(m_pCurrentPatient->GetFatherName());
    ui->leAddress->setText(m_pCurrentPatient->GetAddress());

    ui->deBirthDate->setDate(m_pCurrentPatient->GetBirthDate());
    ui->deArriveDate->setDate(m_pCurrentPatient->GetArriveDate());

    QDate * temp_leave_date = m_pCurrentPatient->LeaveDate();

    if (temp_leave_date) {
        ui->deLeaveDate->setDate(*m_pCurrentPatient->LeaveDate());
        ui->btnSetLeaveDate->setDisabled(true);
    }
    else
        ui->deLeaveDate->setDisabled(true);
}

void PatientPhotos::AddItemToWidget(Photo * p) {
    QPixmap outPixmap {p->GetPixmap()};
    QIcon icon(outPixmap.scaled(ICON_WIDTH, ICON_HEIGHT,
                                Qt::KeepAspectRatio));

    QListWidgetItem * itm = new QListWidgetItem(icon, p->GetFileName());
    itm->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    ui->listWidget_ImageGallery->addItem(itm);
}

void PatientPhotos::on_btnNewPhoto_clicked(){
    QStringList file_names = QFileDialog::getOpenFileNames(this,
                                                     "Open a file",
                                                     qApp->applicationDirPath() + QDir::separator(),
                                                     tr("Image Files (*.jpg *.JPG);;"));
    std::list<QString> file_list = file_names.toStdList();
    for (QString & name: file_list) {
        qDebug() << name[name.size()-1];

        QString just_name{};
        //  Getting filename from path
        for (unsigned int i = name.size()-1; name[i] != QDir::separator(); --i)
            just_name.insert(0, name[i]);
        qDebug() << just_name;

        //  Opening file and saving to QPixmap
        QFile file(name);
        QPixmap temp_pixMap;
        if (file.open(QIODevice::ReadOnly))
            temp_pixMap.loadFromData(file.readAll());

        Photo * new_file = new Photo(this->m_pCurrentPatient->GetId(),
                                     just_name,
                                     temp_pixMap,
                                     QDate::currentDate());
        m_pCurrentPatient->AddPhoto(new_file);
        m_pPatientsWindow->InsertPhoto(*new_file);
        AddItemToWidget(new_file);
    }
}

void PatientPhotos::on_btnDeletePhoto_clicked() {
    QList<QListWidgetItem*> items = ui->listWidget_ImageGallery->selectedItems();
    foreach (QListWidgetItem * item, items) {
        int row_num = ui->listWidget_ImageGallery->row(item);
        Photo * _photo = m_pCurrentPatient->Photos()->at(row_num);
        int photo_id = _photo->GetId();
        delete ui->listWidget_ImageGallery->takeItem(row_num);
        m_pCurrentPatient->DropPhoto(row_num);;
        m_pPatientsWindow->DeletePhotoFromDB(photo_id);
    }
}

void PatientPhotos::on_btnUpdatePatient_clicked() {
    int temp_id = m_pCurrentPatient->GetId();

    QString fname = ui->leFirstName->text();
    QString lname = ui->leLastName->text();
    QString father_name = ui->leFatherName->text();
    QString address = ui->leAddress->text();
    QDate bdate = ui->deBirthDate->date();
    QDate arrdate = ui->deArriveDate->date();

    QDate * leaveDate;
    if (ui->deLeaveDate->isEnabled())
        leaveDate = new QDate( ui->deLeaveDate->date() );
    else
        leaveDate = nullptr;

    delete m_pCurrentPatient;
    m_pCurrentPatient = nullptr;
    m_pCurrentPatient =
            new Patient(fname, lname, father_name, address, bdate, arrdate, *leaveDate);
    m_pCurrentPatient->SetId(temp_id);
    m_pPatientsWindow->UpdatePatient(*m_pCurrentPatient);

    ui->btnUpdatePatient->setEnabled(false);
    ui->btnUnstageChanges->setEnabled(false);
}

void PatientPhotos::CheckChangesFname(const QString & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetFirstName());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetFirstName());
}
void PatientPhotos::CheckChangesLname(const QString & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetLastName());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetLastName());
}
void PatientPhotos::CheckChangesFathername(const QString & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetFatherName());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetFatherName());
}
void PatientPhotos::CheckChangesAddress(const QString & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetAddress());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetAddress());
}
void PatientPhotos::CheckChangesBDate(const QDate & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetBirthDate());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetBirthDate());
}
void PatientPhotos::CheckChangesArriveDate(const QDate & _arg) {
    ui->btnUpdatePatient->setEnabled(_arg != m_pCurrentPatient->GetArriveDate());
    ui->btnUnstageChanges->setEnabled(_arg != m_pCurrentPatient->GetArriveDate());
}
void PatientPhotos::CheckChangesLeaveDate(const QDate & _arg) {
    if (m_pCurrentPatient->LeaveDate()) {
        ui->btnUpdatePatient->setEnabled(_arg != *m_pCurrentPatient->LeaveDate());
        ui->btnUnstageChanges->setEnabled(_arg != *m_pCurrentPatient->LeaveDate());
    } else {
        ui->btnUpdatePatient->setEnabled(true);
        ui->btnUnstageChanges->setEnabled(true);
    }
}

void PatientPhotos::on_btnSetLeaveDate_clicked(){
    ui->deLeaveDate->setEnabled(true);
    ui->deLeaveDate->setFocus();
    ui->btnUpdatePatient->setEnabled(true);
    ui->btnUnstageChanges->setEnabled(true);
}

void PatientPhotos::on_btnUnstageChanges_clicked() {
    SetLineEditFields();
    ui->btnUpdatePatient->setEnabled(false);
    ui->btnUnstageChanges->setEnabled(false);
}
