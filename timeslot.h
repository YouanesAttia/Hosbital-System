#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <QTime>

class TimeSlot
{
private:
	QTime startTime, endTime;
public:
	TimeSlot(QTime n, QTime m): startTime(n), endTime(m){};
	QTime getStartTime(){return startTime;}
	QTime getEndTime(){return endTime;}
};

#endif
