#include "view_patient.h"
#include "ui_view_patient.h"

#include "patients_window.h"
#include "imageview.h"

#include "photo.h"

#include "dbmanager.h"

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

ViewPatient::ViewPatient(Patient * patient,
                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewPatient)
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

    auto reflect = [this] (QVariant arg_prev, QVariant arg_cur) {
        this->SetControlButtonsState(arg_prev != arg_cur);
    };

    connect(this->ui->leFirstName, &QLineEdit::textChanged, [this, reflect] (const QString & new_str) {
        reflect(QVariant(this->m_pCurrentPatient->GetFirstName()), QVariant(new_str));
    });

    connect(this->ui->leLastName, &QLineEdit::textChanged, [this, reflect] (const QString & new_str) {
        reflect(QVariant(this->m_pCurrentPatient->GetLastName()), QVariant(new_str));
    });

    connect(this->ui->leFatherName, &QLineEdit::textChanged, [this, reflect] (const QString & new_str) {
        reflect(QVariant(this->m_pCurrentPatient->GetFatherName()), QVariant(new_str));
    });

    connect(this->ui->leAddress, &QLineEdit::textChanged, [this, reflect] (const QString & new_str) {
        reflect(QVariant(this->m_pCurrentPatient->GetAddress()), QVariant(new_str));
    });

    connect(this->ui->deBirthDate, &QDateEdit::dateChanged, [this, reflect] (const QDate & new_date) {
        reflect(QVariant(this->m_pCurrentPatient->GetBirthDate()), QVariant(new_date));
    });

    connect(this->ui->deArriveDate, &QDateEdit::dateChanged, [this, reflect] (const QDate & new_date) {
        reflect(QVariant(this->m_pCurrentPatient->GetArriveDate()), QVariant(new_date));
    });

    connect(this->ui->deLeaveDate, &QDateEdit::dateChanged, [this] (const QDate & new_date) {
        if (this->m_pCurrentPatient->LeaveDate())
            this->SetControlButtonsState(new_date != *this->m_pCurrentPatient->LeaveDate());
        else
            this->SetControlButtonsState(true);
    });
}

ViewPatient::~ViewPatient()
{
    delete ui;
}

void ViewPatient::SetControlButtonsState(bool is_enabled) {
    ui->btnUpdatePatient->setEnabled(is_enabled);
    ui->btnUnstageChanges->setEnabled(is_enabled);
}

void ViewPatient::SetLineEditFields() {
    ui->leFirstName->setText(m_pCurrentPatient->GetFirstName());
    ui->leLastName->setText(m_pCurrentPatient->GetLastName());
    ui->leFatherName->setText(m_pCurrentPatient->GetFatherName());
    ui->leAddress->setText(m_pCurrentPatient->GetAddress());

    ui->deBirthDate->setDate(m_pCurrentPatient->GetBirthDate());
    ui->deArriveDate->setDate(m_pCurrentPatient->GetArriveDate());

    const QDate * temp_leave_date = m_pCurrentPatient->LeaveDate();

    if (temp_leave_date) {
        ui->deLeaveDate->setDate(*m_pCurrentPatient->LeaveDate());
        ui->btnSetLeaveDate->setDisabled(true);
    }
    else
        ui->deLeaveDate->setDisabled(true);
}

void ViewPatient::AddItemToWidget(Photo * p) {
    QPixmap outPixmap {p->GetPixmap()};
    QIcon icon(outPixmap.scaled(ICON_WIDTH, ICON_HEIGHT,
                                Qt::KeepAspectRatio));

    QListWidgetItem * itm = new QListWidgetItem(icon, p->GetFileName());
    itm->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    ui->listWidget_ImageGallery->addItem(itm);
}

void ViewPatient::on_btnNewPhoto_clicked(){
    //  Open dialog box for selectig .jpg images
    QStringList file_names = QFileDialog::getOpenFileNames(this,
                                                     "Open a file",
                                                     qApp->applicationDirPath() + QDir::separator(),
                                                     tr("Image Files (*.jpg *.JPG *.jpeg *.JPEG);;"));
    //  Converting list of selected images to standard list
    std::list<QString> file_list = file_names.toStdList();

    for (QString & name: file_list) {

        QFile file(name);
        QFileInfo fileInfo(file.fileName());
        QString fileName(fileInfo.fileName());

        //  Opening file and saving to QPixmap
        QPixmap temp_pixMap;
        if (file.open(QIODevice::ReadOnly))
            temp_pixMap.loadFromData(file.readAll());

        Photo * new_file = new Photo(this->m_pCurrentPatient->GetId(),
                                     fileName,
                                     temp_pixMap,
                                     QDate::currentDate());
        m_pCurrentPatient->AddPhoto(new_file);

        DBManager::instance().photoDao.addPhoto(*new_file);
        AddItemToWidget(new_file);
    }
}

void ViewPatient::on_btnDeletePhoto_clicked() {
    QList<QListWidgetItem*> items = ui->listWidget_ImageGallery->selectedItems();
    foreach (QListWidgetItem * item, items) {
        int row_num = ui->listWidget_ImageGallery->row(item);
        Photo * _photo = m_pCurrentPatient->Photos()->at(row_num);
        int photo_id = _photo->GetId();
        delete ui->listWidget_ImageGallery->takeItem(row_num);
        m_pCurrentPatient->DropPhoto(row_num);

        DBManager::instance().photoDao.removePhoto(photo_id);
    }
}

void ViewPatient::on_btnUpdatePatient_clicked() {
    m_pCurrentPatient->SetFirstName(ui->leFirstName->text());
    m_pCurrentPatient->SetLastName(ui->leLastName->text());
    m_pCurrentPatient->SetFatherName(ui->leFatherName->text());
    m_pCurrentPatient->SetAddress(ui->leAddress->text());
    m_pCurrentPatient->SetBirthDate(ui->deBirthDate->date());
    m_pCurrentPatient->SetArriveDate(ui->deArriveDate->date());

    DBManager::instance().patientDao.updatePatient(*m_pCurrentPatient);

    SetControlButtonsState(false);
}

//void ViewPatient::CheckChangesLeaveDate(const QDate & _arg) {
//    if (m_pCurrentPatient->LeaveDate()) {
//        SetControlButtonsState(_arg != *m_pCurrentPatient->LeaveDate());
//    } else {
//        SetControlButtonsState(true);
//    }
//}

void ViewPatient::on_btnSetLeaveDate_clicked() {
    ui->deLeaveDate->setEnabled(true);
    ui->deLeaveDate->setFocus();
    SetControlButtonsState(true);
}

void ViewPatient::on_btnUnstageChanges_clicked() {
    SetLineEditFields();
    SetControlButtonsState(false);
}
