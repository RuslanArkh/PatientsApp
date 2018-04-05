#ifndef PHOTODAO_H
#define PHOTODAO_H

#include <vector>
#include <QSqlQuery>

class Photo;
class QSqlDatabase;
class Patient;
class QSqlQuery;
class QString;

class Photo;

class PhotoDao {

public:

    PhotoDao(QSqlDatabase & database);
    void init() const;

    void addPhoto(const Photo & photo) const;
    void removePhoto(int id) const;

    std::vector<Photo *> * photosByPatientId(int id) const;

    QSqlQuery photosByPatientIdQuery(int id) const;

private:

    QSqlDatabase & mDatabase;
};

#endif // PHOTODAO_H
