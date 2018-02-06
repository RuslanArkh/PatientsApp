#ifndef PATIENTS_MODEL_H
#define PATIENTS_MODEL_H

#include <QAbstractTableModel>

#include <vector>
#include <map>

#include "dbmanager.h"

const int COLS = 3;

class PatientsModel : public QAbstractTableModel {

    Q_OBJECT

public:

    PatientsModel(std::vector<Patient *> * PatientsData, QObject * parent = nullptr);
    ~PatientsModel();

    int rowCount(const QModelIndex & index = QModelIndex()) const override;
    int columnCount(const QModelIndex & index = QModelIndex()) const override;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void AddPatient(Patient &);
    void DeletePatient(int row);

    Patient * GetPatient(int pId) const;


signals:

    void itemForRemove(const QModelIndex & index) const;

private:
    // Data:
    std::vector<Patient *> * m_pPatientsVector;

};

inline int PatientsModel::rowCount(const QModelIndex &) const {
    return m_pPatientsVector->size();
}

inline int PatientsModel::columnCount(const QModelIndex &) const {
    return COLS;
}

#endif // PATIENTS_MODEL_H
