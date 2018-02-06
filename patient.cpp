#include "patient.h"

Patient::Patient(
        const QString & f,
        const QString & l,
        const QString & father,
        const QString & a,
        const QDate & bD,
        const QDate & arr,
        QDate & le)
    : _FirstName{f}, _LastName{l}, _FatherName{father}, _Address{a}
     , _BirthDate{bD}, _ArriveDate{arr}, m_pLeaveDate{&le}, m_pPhotosVector{nullptr}
{
}

Patient::Patient(const Patient & p)
    : id(p.id),
      _FirstName(p._FirstName),
      _LastName(p._LastName),
      _FatherName(p._FatherName),
      _Address(p._Address),
      _BirthDate(p._BirthDate),
      _ArriveDate(p._ArriveDate),
      m_pLeaveDate(p.m_pLeaveDate)
{}


Patient::~Patient() {
    if (m_pLeaveDate)
        delete m_pLeaveDate;
}

const QString Patient::GetFullName() const {
    QString str{};
    str += _FirstName + " " + _LastName + " " + _FatherName;
    return str;
}

void Patient::loadPhotos(std::vector<Photo*> * photos) {
    if (!m_pPhotosVector)
        m_pPhotosVector = photos;
    else
        Patient_Ex::PhotosAlreadyLoadedError().raise();
}

void Patient::ClearPhotos() {
    if (m_pPhotosVector)
        for (Photo * pPhoto: * m_pPhotosVector)
            delete pPhoto;
}

const std::vector<Photo *> * Patient::Photos() const {
    return m_pPhotosVector;
}

void Patient::DropPhoto(int row_num) {
    m_pPhotosVector->erase(m_pPhotosVector->begin() + row_num);
}

void Patient::AddPhoto(Photo * pPhoto) {
    m_pPhotosVector->push_back(pPhoto);
}

using namespace Patient_Ex;

BirthAndArriveDateLogicError::BirthAndArriveDateLogicError(const QDate & bdate,
                                                           const QDate & arrdate)
{
    m_message += "Birth date '" + bdate.toString() + "' can't be less than ";
    m_message += "arrive date '" + arrdate.toString() + "'.";
}
