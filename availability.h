#ifndef AVAILABILITY_H
#define AVAILABILITY_H

#include <QString>
#include <QTime>

class Availability
{
private:
    QString doctorID;
    QString dayOfWeek;
    QTime startTime;
    QTime endTime;

public:

    
    Availability();

  
    Availability(QString doctorID,
                 QString dayOfWeek,
                 QTime startTime,
                 QTime endTime);

    
    QString getDoctorID() const;
    QString getDayOfWeek() const;
    QTime getStartTime() const;
    QTime getEndTime() const;
};

#endif
