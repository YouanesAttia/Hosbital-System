#include "TimeSlot.h"

TimeSlot::TimeSlot()
{
}

TimeSlot::TimeSlot(QTime start,
                   QTime end)
{
    startTime=start;
    endTime=end;
}

QTime TimeSlot::getStartTime() const
{
    return startTime;
}

QTime TimeSlot::getEndTime() const
{
    return endTime;
}
