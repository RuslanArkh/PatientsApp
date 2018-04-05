#include "photo.h"
#include <QFile>
#include <QException>
#include <QBuffer>


Photo::Photo(int _patient_id, const QString _f, const QPixmap _pMap, const QDate _created)
    : id{-1}, patient_id{_patient_id}, fileName{_f}, created_on{_created}
{
    _imagePixmap = _pMap;
}

Photo::Photo(int _id, int _patient_id, const QString _f, const QDate _created)
    : id(_id), patient_id(_patient_id), fileName{_f}, created_on{_created}
{}

const QByteArray Photo::GetImageBytes() const {
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    _imagePixmap.save(&buffer, "JPG");
    return bArray;
}
