#include "patients_window.h"
#include "ui_mainwindow.h"

#include "dbmanager_exceptions.h"

#include "new_patient.h"
#include "view_patient.h"

#include <QtSql>
#include <QTableWidget>
#include <QMessageBox>
#include <QRegExp>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <string>
#include <algorithm>
#include <regex>

PatientsWindow::PatientsWindow(DBManager * db, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDatabaseManager(db)
{
    ui->setupUi(this);



    //  Selecting all patients from database into std::vector
    std::vector<Patient *> * allPatients = m_pDatabaseManager->SelectAllPatients();

    //  Creating custom MODEL for patients
    m_pModel = new PatientsModel(allPatients, ui->tvPatients);

    //  Creating PROXY for main model
    m_pProxyModel = new PatientsFilterProxyModel(this);
    m_pProxyModel->setSourceModel(m_pModel);
    m_pProxyModel->setStillHere(ui->checkBox_StillHere->isChecked());
    m_pProxyModel->setLeft(ui->checkBox_Left->isChecked());

    //  Creating custom DELEGATE for third column in TableView
    m_pButtonDelegate = new DeleteButtonDelegate(ui->tvPatients);

    //  Configuring table VIEW for data representation
    ui->tvPatients->setModel(m_pProxyModel);
    ui->tvPatients->verticalHeader()->hide();
    ui->tvPatients->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tvPatients->setItemDelegateForColumn(2, m_pButtonDelegate);
    ui->tvPatients->setColumnWidth(0, ((ui->tvPatients->width() * 10) / 2));
    ui->tvPatients->setColumnWidth(2, 60);
    ui->btn_DeletePatient->setDisabled(true);

    //  Connecting 'itemForRemove' signal with this->DeleteByIndex function
    //  to enable communication MODEL and VIEW via custom DELEGATE 'DeleteButtonDelegate'
    connect(m_pModel, &PatientsModel::itemForRemove,
            this, &PatientsWindow::DeleteByIndex);
}

PatientsWindow::~PatientsWindow()
{
    delete ui;
    delete m_pModel;
    delete m_pProxyModel;
    delete m_pButtonDelegate;
}

void PatientsWindow::InsertPatient(Patient & p) {
    m_pDatabaseManager->insert(p);
    m_pModel->AddPatient(p);
}

void PatientsWindow::InsertPhoto(Photo & photo) {
    m_pDatabaseManager->insert(photo);
}

void PatientsWindow::UpdatePatient(Patient & p) {
    m_pDatabaseManager->Update(p);
    int row = ui->tvPatients->currentIndex().row();
    m_pModel->dataChanged(m_pModel->index(row, 0), m_pModel->index(row, 1));
}

void PatientsWindow::DeletePhotoFromDB(int id) {
    m_pDatabaseManager->removePhoto(id);
}

void PatientsWindow::on_btn_DeletePatient_clicked() {
    int source_index_row =
            m_pProxyModel->mapToSource(ui->tvPatients->currentIndex()).row();
    m_pDatabaseManager->removePatient(m_pModel->GetPatient(source_index_row)->GetId());
    m_pModel->DeletePatient(source_index_row);
    ui->btn_DeletePatient->setDisabled(true);
    ui->tvPatients->setCurrentIndex(QModelIndex(m_pModel->index(-1, -1)));
    this->setFocus();
}

void PatientsWindow::on_btn_NewPatient_clicked() {
    NewPatient newpatient_dialog(this);
    newpatient_dialog.exec();
}

//  SHOW Photos!
void PatientsWindow::on_tvPatients_doubleClicked(const QModelIndex &proxy_index){
    int source_index_row = m_pProxyModel->mapToSource(proxy_index).row();
    Patient * pP = m_pModel->GetPatient(source_index_row);
    std::vector<Photo *> * photos = m_pDatabaseManager->SelectByPatientId(pP->GetId());

    try {
        //  TODO: Load photos in different threads
        pP->loadPhotos(photos);
        ViewPatient photos_window(pP, this);
        //  TODO: NOT MODAL
        photos_window.exec();
        pP->clearPhotos();
    } catch (Patient_Ex::PhotosAlreadyLoadedError) {
        for (Photo * pPhoto: *photos)
            delete pPhoto;
        delete photos;
        QMessageBox::critical(this, QString("Photos error"),
                              QString("Photos already loaded."),
                              QMessageBox::Ok);
    }
}

void PatientsWindow::on_lineEdit_Search_textChanged(const QString & str1) {
    m_pProxyModel->setFilterFixedString(str1);
}

void PatientsWindow::on_checkBox_StillHere_stateChanged(int arg1){
    m_pProxyModel->setStillHere(ui->checkBox_StillHere->isChecked());
}

void PatientsWindow::on_checkBox_Left_stateChanged(int arg1) {
    m_pProxyModel->setLeft(ui->checkBox_Left->isChecked());
}

void PatientsWindow::on_tvPatients_clicked(const QModelIndex &index) {
    ui->btn_DeletePatient->setDisabled(index.row() == -1);
}

void PatientsWindow::on_tvPatients_activated(const QModelIndex &index) {
    ui->btn_DeletePatient->setDisabled(index.row() == -1);
}

void PatientsWindow::DeleteByIndex(const QModelIndex & index) {

    int source_index = m_pProxyModel->mapToSource(index).row();
    Patient * pP = m_pModel->GetPatient(source_index);

    QMessageBox::StandardButton reply;
    QString question{"Patient "};
    question += pP->GetFullName() + " will be deleted. Are you sure?";
    reply = QMessageBox::question(this, "Patient deletion", question, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_pDatabaseManager->removePatient(pP->GetId());
        m_pModel->DeletePatient(source_index);
    }
}
