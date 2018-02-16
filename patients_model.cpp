
#include "patients_model.h"

#include <QString>
#include <QPushButton>


PatientsModel::PatientsModel(std::vector<Patient *> * PatientsData, QObject * parent)
      :QAbstractTableModel(parent)
{
    m_pPatientsVector = PatientsData;
}

PatientsModel::~PatientsModel() {
    for (Patient * patient: *m_pPatientsVector)
        delete patient;
    delete m_pPatientsVector;
}

QVariant PatientsModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();
    if (role == Qt::DisplayRole) {
        Patient * pPatient = m_pPatientsVector->at(row);
        if (col == 0) {
            return pPatient->GetFullName();
        } else if (col == 1) {
            if (pPatient->LeaveDate())
                return pPatient->LeaveDate()->toString("dd/MM/yyyy");
            else
                return QString("Still here");
        } else if (col == 2) {
            return QString("");
        }
    }
    return QVariant();
}

QVariant PatientsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Full Name");
            case 1:
                return QString("Leave Date");
            }
        }
    }
    return QVariant();
}


void PatientsModel::AddPatient(Patient & p) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_pPatientsVector->push_back(&p);
    endInsertRows();
}

/*
bool PatientsModel::removeRows(int row, int count, const QModelIndex &parent)
{

}
*/

void PatientsModel::DeletePatient(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    m_pPatientsVector->erase(m_pPatientsVector->begin() + row);
    endRemoveRows();
}

Patient * PatientsModel::GetPatient(int row) const {
    Patient * pP = m_pPatientsVector->at(row);
    return pP;

}
