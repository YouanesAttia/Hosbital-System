#include <QString>
#include <QDate>
#include "timeslot.h"

class Appointment
{
private:
	QString doctorID;
	QString patientID;
	QDate date;
	TimeSlot slot;
public:
	Appointment(QString dID, QString pID, QDate dt, TimeSlot s): doctorID(dID), patientID(pID), date(dt), slot(s) {}
	QDate getDate() { return date; }
	QTime getStartTime() { return slot.getStartTime(); }
	QString getDoctorID() { return doctorID; }
	QString getPatientID() { return patientID; }
	TimeSlot getTimeSlot() { return slot; }
};
