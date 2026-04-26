#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <QTime>

class TimeSlot
{
private:
    QTime startTime;
    QTime endTime;

public:
    TimeSlot();

    TimeSlot(QTime start,
             QTime end);

    QTime getStartTime() const;
    QTime getEndTime() const;
};
