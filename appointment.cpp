#include "Appointment.h"

Appointment::Appointment()
{
}

Appointment::Appointment(QString dID,
                         QString pID,
                         QDate appDate,
                         TimeSlot ts)
{
    doctorID=dID;
    patientID=pID;
    date=appDate;
    slot=ts;
}

QString Appointment::getDoctorID() const
{
    return doctorID;
}QString Appointment::getPatientID() const
{
    return patientID;
}

QDate Appointment::getDate() const
{
    return date;
}

TimeSlot Appointment::getTimeSlot() const
{
    return slot;
}QTime Appointment::getStartTime() const
{
    return slot.getStartTime();
}

QTime Appointment::getEndTime() const
{
    return slot.getEndTime();
}
