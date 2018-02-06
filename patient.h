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
    const QString _FirstName, _LastName, _FatherName;
    const QString _Address;
    const QDate _BirthDate;
    const QDate _ArriveDate;

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
    const QString & GetFirstName() const;
    const QString & GetLastName() const;
    const QString & GetFatherName() const;
    const QString & GetAddress() const;
    const QDate & GetBirthDate() const;
    const QDate & GetArriveDate() const;

    QDate * LeaveDate();

    bool StillHere() const;
    bool Left() const;

    const QString GetFullName() const;

    void loadPhotos(std::vector<Photo*> * photos);
    void ClearPhotos();

    const std::vector<Photo *> * Photos() const;
    void DropPhoto(int row_num);
    void AddPhoto(Photo *);
};

inline void Patient::SetId(int _id) const { id = _id; }

inline int Patient::GetId() const { return id; }
inline const QString & Patient::GetFirstName() const { return _FirstName; }
inline const QString & Patient::GetLastName() const { return _LastName; }
inline const QString & Patient::GetFatherName() const { return _FatherName; }
inline const QString & Patient::GetAddress() const { return _Address; }
inline const QDate & Patient::GetBirthDate() const { return _BirthDate; }
inline const QDate & Patient::GetArriveDate() const { return _ArriveDate; }
inline QDate * Patient::LeaveDate() { return m_pLeaveDate; }

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
