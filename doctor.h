#ifndef DOCTOR_H
#define DOCTOR_H

#include <QString>
class Doctor
{
private:
    QString id;
    QString name;
    QString department;

public:
    
    Doctor();

   
    Doctor(QString docID,
           QString docName,
           QString dept);

    QString getID() const;
    QString getName() const;
    QString getDepartment() const;
};

#endif
