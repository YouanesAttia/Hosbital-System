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
	Availability(QString id, QString day, QTime start, QTime end): doctorID(id), dayOfWeek(day), startTime(start), endTime(end) {}
	QTime getStartTime() { return startTime; }
	QTime getEndTime() { return endTime; }
	QString getDoctorID() { return doctorID; }
	QString getDayOfWeek() { return dayOfWeek; }
};
