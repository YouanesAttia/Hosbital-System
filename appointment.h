#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QString>
#include <QDate>
#include "TimeSlot.h"

class Appointment
{
private:
    QString doctorID;
    QString patientID;
    QDate date;
    TimeSlot slot;
public:
    Appointment();

    Appointment(QString dID,
                QString pID,
                QDate appDate,
                TimeSlot ts);

    QString getDoctorID() const;
    QString getPatientID() const;
    QDate getDate() const;

    TimeSlot getTimeSlot() const;

    QTime getStartTime() const;
    QTime getEndTime() const;
};
