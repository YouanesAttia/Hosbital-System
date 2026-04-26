#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDate>

class Patient
{
public:
    enum Gender
    {
        Male,
        Female
    };

private:
    QString nationalID;
    QString name;
    QString mobileNumber;
    QDate birthdate;
    Gender gender;

public:
    Patient();

    Patient(QString id,
            QString pname,
            QString mobile,
            QDate birth,
            Gender g);

    QString getNationalID() const;
    QString getName() const;
    QString getMobileNumber() const;
    QDate getBirthdate() const;
    Gender getGender() const;

    void setMobileNumber(QString newNumber);
};

