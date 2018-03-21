#ifndef PATIENT_H
#define PATIENT_H

#include <QDate>
#include <QException>
#include <QString>

#include <vector>

#include "photo.h"

//  TODO: make regexes for all data fields
class Patient {

    mutable int id;
    QString _FirstName, _LastName, _FatherName;
    QString _Address;
    QDate _BirthDate;
    QDate _ArriveDate;

    QDate * m_pLeaveDate;

    std::vector<Photo *> * m_pPhotosVector;

public:
    Patient(
            const QString & f,
            const QString & l,
            const QString & father,
            const QString & a,
            const QDate & bD,
            const QDate & arr,
            QDate & le);
    Patient(const Patient & p);

    ~Patient();

    void SetId(int _id) const;

    int GetId() const;
    QString GetFirstName() const;
    QString GetLastName() const;
    QString GetFatherName() const;
    QString GetAddress() const;
    const QDate & GetBirthDate() const;
    const QDate & GetArriveDate() const;
    const QDate * LeaveDate() const;

    void SetFirstName(const QString & fname);
    void SetLastName(const QString & lname);
    void SetFatherName(const QString & fathername);
    void SetAddress(const QString & address);
    void SetBirthDate(const QDate & birthDate);
    void SetArriveDate(const QDate & arriveDate);
    void SetLeaveDate(const QDate & leaveDate);

    bool StillHere() const;
    bool Left() const;

    const QString GetFullName() const;

    void loadPhotos(std::vector<Photo*> * photos);
    void clearPhotos();

    const std::vector<Photo *> * Photos() const;
    void DropPhoto(int row_num);
    void AddPhoto(Photo *);
};

inline void Patient::SetId(int _id) const { id = _id; }

inline int Patient::GetId() const { return id; }
inline QString Patient::GetFirstName() const { return _FirstName; }
inline QString Patient::GetLastName() const { return _LastName; }
inline QString Patient::GetFatherName() const { return _FatherName; }
inline QString Patient::GetAddress() const { return _Address; }
inline const QDate & Patient::GetBirthDate() const { return _BirthDate; }
inline const QDate & Patient::GetArriveDate() const { return _ArriveDate; }
inline const QDate * Patient::LeaveDate() const { return m_pLeaveDate; }

inline void Patient::SetFirstName(const QString & fname )  { this->_FirstName = fname; }
inline void Patient::SetLastName(const QString & lname )  { this->_LastName = lname; }
inline void Patient::SetFatherName(const QString & fathername )  { this->_FatherName = fathername; }
inline void Patient::SetAddress(const QString & address )  { this->_Address = address; }
inline void Patient::SetBirthDate(const QDate & bdate )  { this->_BirthDate = bdate; }
inline void Patient::SetArriveDate(const QDate & arrdate )  { this->_ArriveDate = arrdate; }
inline void Patient::SetLeaveDate(const QDate & leaveDate ) { *m_pLeaveDate = leaveDate; }

inline bool Patient::StillHere() const {
    return (!m_pLeaveDate) || (m_pLeaveDate && *m_pLeaveDate > QDate::currentDate());
}

inline bool Patient::Left() const {
    return (m_pLeaveDate) && (*m_pLeaveDate < QDate::currentDate());
}

/**************************************************************************/

//  EXCEPTIONS:

namespace Patient_Ex {

    class BirthAndArriveDateLogicError : public QException {
        QString m_message;
    public:
        BirthAndArriveDateLogicError(const QDate & bdate, const QDate & arrdate);
        void raise() const { throw *this; }
        BirthAndArriveDateLogicError *clone() const {
            return new BirthAndArriveDateLogicError(*this);
        }
        const QString & message() const;
    };
    inline const QString & BirthAndArriveDateLogicError::message() const { return m_message; }

    class ArriveAndLeaveDateLogicError : public QException {
        const QString _message;
    public:
        ArriveAndLeaveDateLogicError(const QString & message);
        void raise() const { throw *this; }
        ArriveAndLeaveDateLogicError *clone() const {
            return new ArriveAndLeaveDateLogicError(*this);
        }
        const QString & message() const;
    };
    inline const QString & ArriveAndLeaveDateLogicError::message() const { return _message; }

    class PhotosAlreadyLoadedError : public QException {
    public:
        PhotosAlreadyLoadedError() {}
        void raise() const { throw *this; }
        PhotosAlreadyLoadedError *clone() const {
            return new PhotosAlreadyLoadedError(*this);
        }
    };
}

#endif // PATIENT_H
