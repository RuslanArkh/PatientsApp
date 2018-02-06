#ifndef PHOTO_H
#define PHOTO_H

#include <QtCore>
#include <QDate>
#include <QString>
#include <QByteArray>
#include <QException>
#include <QPixmap>

//  TODO: make regexes for all data fields
class Photo {

    mutable int id;
    const int patient_id;
    const QString fileName;
    QPixmap _imagePixmap;
    const QDate created_on;

public:

    Photo(int _id, int parent_id, const QString & _f, const QDate & _created);
    Photo(int parent_id, const QString & _f, const QPixmap & _pMap, const QDate & _created  = QDate::currentDate());

    void SetId(int _id) const;

    int GetId() const;
    int GetPatientId() const;
    const QString & GetFileName() const;
    const QPixmap & GetPixmap() const;
    const QDate & GetCreationDate() const;

    const QByteArray GetImageBytes() const;
};

//  TODO: check invariants
inline void Photo::SetId(int _id) const  { id = _id; }

inline int Photo::GetId() const { return id; }
inline int Photo::GetPatientId() const { return patient_id; }
inline const QString & Photo::GetFileName() const { return fileName; }
inline const QPixmap & Photo::GetPixmap() const { return _imagePixmap; }
inline const QDate & Photo::GetCreationDate() const { return created_on; }

class OpenFileProblem : public QException {
public:
    void raise() const { throw *this; }
    OpenFileProblem *clone() const { return new OpenFileProblem(*this); }
};

#endif // PHOTO_H
