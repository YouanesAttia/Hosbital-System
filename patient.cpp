#include "Patient.h"

Patient::Patient()
{
}

Patient::Patient(QString id,
                 QString pname,
                 QString mobile,
                 QDate birth,
                 Gender g)
{
    nationalID=id;
    name=pname;
    mobileNumber=mobile;
    birthdate=birth;
    gender=g;
}
QString Patient::getNationalID() const
{
    return nationalID;
}

QString Patient::getName() const
{
    return name;
}

QString Patient::getMobileNumber() const
{
    return mobileNumber;
}

QDate Patient::getBirthdate() const
{
    return birthdate;
}
Patient::Gender Patient::getGender() const
{
    return gender;
}

void Patient::setMobileNumber(QString newNumber)
{
    mobileNumber=newNumber;
}
