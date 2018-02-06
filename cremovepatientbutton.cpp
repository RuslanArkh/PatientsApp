#include "cremovepatientbutton.h"
#include "patients_model.h"

CRemovePatientButton::CRemovePatientButton(QWidget* parent)
    : QPushButton(parent)
{
    connect(this, &CRemovePatientButton::clicked,
            this, &CRemovePatientButton::onClicked);
}

void CRemovePatientButton::onClicked() {
    emit dynamic_cast<const PatientsModel *>
            (m_sourceIndex.model())->itemForRemove(m_sourceIndex);
}

void CRemovePatientButton::setModelIndex( const QModelIndex& index ) {
    m_sourceIndex = index;
}
