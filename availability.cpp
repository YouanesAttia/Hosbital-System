#include "availability.h"

Availability::Availability()
{
}

Availability::Availability(QString doctorID,
                           QString dayOfWeek,
                           QTime startTime,
                           QTime endTime)
{
    this->doctorID = doctorID;
    this->dayOfWeek = dayOfWeek;
    this->startTime = startTime;
    this->endTime = endTime;
}

QString Availability::getDoctorID() const
{
    return doctorID;
}

QString Availability::getDayOfWeek() const
{
    return dayOfWeek;
}

QTime Availability::getStartTime() const
{
    return startTime;
}

QTime Availability::getEndTime() const
{
    return endTime;
}
