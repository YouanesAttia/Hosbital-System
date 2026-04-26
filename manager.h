#include <QList>
#include <QString>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include "doctor.h"
#include "patient.h"
#include "appointment.h"
#include "availability.h"
#include "timeslot.h"

class Manager
{
private:
	QList<Doctor> doctors;
	QList<Patient> patients;
	QList<Appointment> appointments;
	QList<Availability> availabilities;
public:
	Manager();
	void loadInitialData();
	void savePatientData();
	void loadPatientData();
	void saveAppointments();
	void loadAppointments();

	bool validateAppointment(QString patientID, QString doctorID, QDate date, TimeSlot slot);
	QList<TimeSlot> generate20MinSlots(QString doctorID, QDate date);
	bool isValidSlot(QString doctorID, QDate date, TimeSlot slot);
	bool hasPatientConflict(QString patientID, QDate date, TimeSlot slot);
	bool hasDoctorConflict(QString doctorID, QDate date, TimeSlot slot);
	bool exceedsDailyLimit(QString patientID, QDate date);
	bool isWithinBufferTime(QDate date, TimeSlot slot);
	bool isWithinDateLimit(QDate date);
	bool isNationalIDUnique(QString nationalID);

	QList<Doctor> filterDoctorsByName(QString name);
	QList<Doctor> filterDoctorsByDepartment(QString dept);

	bool addPatient(Patient p);
	bool addAppointment(Appointment a);

	Doctor* getDoctorByID(QString id);
	Patient* getPatientByID(QString id);

	QList<Appointment>& loadAllAppointmentsInternal() { return appointments; }

};
