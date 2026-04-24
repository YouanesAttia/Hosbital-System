#include "manager.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

Manager::Manager()
{
        loadInitialData();
        loadPatientData();
        loadAppointments();
}

void Manager::loadInitialData()
{
        QFile dFile("doctors.csv");
        if (dFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&dFile);
                if(!in.atEnd()) in.readLine();

                while (!in.atEnd()) {
                        QString line = in.readLine();
                        QStringList fields = line.split(',');
                        if(fields.size() >= 3)
                                doctors.append(Doctor(fields[1].trimmed(), fields[0].trimmed(), fields[2].trimmed()));
                }
                dFile.close();
        }
        QFile aFile("availability.csv");
        if(aFile.open(QIODevice::ReadOnly))
        {
                QTextStream in(&aFile);
                if(!in.atEnd()) in.readLine();

                while(!in.atEnd()){
                        QString line = in.readLine();
                        QStringList fields = line.split(',');
                        if(fields.size() >= 4)
                        {
                                QString id = fields[0].trimmed();
                                QString day = fields[1].trimmed();
                                QTime start = QTime::fromString(fields[2].trimmed(), "h:mm");
                                QTime end = QTime::fromString(fields[3].trimmed(), "h:mm");
                                availabilities.append(Availability(id, day, start, end));
                        }
                }
        aFile.close();
        }
}
void Manager::savePatientData()
{
        QFile file("patients.csv");
        if(file.open(QIODevice::WriteOnly))
        {
                QTextStream out(&file);
                for(auto &p : patients){
                        out << p.getNationalID() << "," << p.getName() << ","
                        << p.getBirthdate().toString(Qt::ISODate) << ","
                        << (int)p.getGender() << "," << p.getMobileNumber() << "\n";
                }
                file.close();
        }
}

void Manager::loadPatientData()
{
        QFile file("patients.csv");
        if(file.open(QIODevice::ReadOnly))
        {
                QTextStream in(&file);
                while(!in.atEnd())
                {
                        QStringList f = in.readLine().split(',');
                        if (f.size() >= 5)
                        {
                                Patient::Gender g = (f[3].toInt() == 0) ? Patient::Male : Patient::Female;
                                patients.append(Patient(f[0], f[1], f[4], QDate::fromString(f[2], Qt::ISODate), g));
                        }
                }
                file.close();
        }
}

void Manager::saveAppointments()
{
        QFile file("appointments.csv");
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
                QTextStream out(&file);
                for (auto &app : appointments) {
                        out << app.getDoctorID() << ","
                            << app.getPatientID() << ","
                            << app.getDate().toString(Qt::ISODate) << ","
                            << app.getTimeSlot().getStartTime().toString("h:mm") << ","
                            << app.getTimeSlot().getEndTime().toString("h:mm") << "\n";
                }
                file.close();
        }

}

void Manager::loadAppointments()
{
        QFile file("appointments.csv");
        if (!file.exists()) return;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                QTextStream in(&file);
                appointments.clear();
                while(!in.atEnd())
                {
                        QString line = in.readLine();
                        if(line.isEmpty()) continue;
                        QStringList a = line.split(',');

                        if(a.size() >= 5)
                        {
                                QString dID = a[0].trimmed();
                                QString pID = a[1].trimmed();
                                QDate dt = QDate::fromString(a[2].trimmed(), Qt::ISODate);
                                QTime start = QTime::fromString(a[3].trimmed(), "h:mm");
                                QTime end = QTime::fromString(a[4].trimmed(), "h:mm");

                                TimeSlot slot(start, end);
                                appointments.append(Appointment(dID, pID, dt, slot));

                        }
                }
                file.close();
        }
}

QList<TimeSlot> Manager::generate20MinSlots(QString doctorID, QDate date)
{
        QList<TimeSlot> availSlots;
        QString dayOfWeek = date.toString("dddd");

        for(auto &avail : availabilities)
        {
                if(avail.getDoctorID() == doctorID && avail.getDayOfWeek() == dayOfWeek)
                {
                        QTime start = avail.getStartTime();
                        while(start.addSecs(1200) <= avail.getEndTime())
                        {
                                availSlots.append(TimeSlot(start, start.addSecs(1200)));
                                start = start.addSecs(1200);
                        }
                }
        }
        return availSlots;
}

bool Manager::isValidSlot(QString doctorID, QDate date, TimeSlot slot)
{
	QList<TimeSlot> validSlots = generate20MinSlots(doctorID, date);
	for (auto &a : validSlots)
	{
		if(a.getStartTime() == slot.getStartTime())
			return true;
	}
	return false;
}

//A patient cannot have two appointments at the same time
bool Manager::hasPatientConflict(QString patientID, QDate date, TimeSlot slot)
{
	for (auto &a: appointments)
	{
		if(a.getPatientID() == patientID && a.getDate() == date && a.getStartTime() == slot.getStartTime())
		{
			return true;
		}
	}
	return false;
}

//  A doctor cannot have two appointments at the same time
bool Manager::hasDoctorConflict(QString doctorID, QDate date, TimeSlot slot)
{
	for (&a : appointments)
	{
		if(a.getDoctorID() == doctorID && a.getDate() == date && a.getStartTime() == slot.getStartTime())
		{
			return true;
		}
	}
	return false;
}

//A single patient cannot have more than two appointments per calendar day
bool Manager::exceedsDailyLimit(QString patientID, QDate date)
{
	int count = 0;
	for (auto &a : appointments
	{
		if(a.getPatientID() == patientID && a.getDate() == date)
		{
			count ++;
		}
	}
	return (count > 2);
}

//Reservations must be made at least 1 hour prior to the slot’s start time
bool Manager::isWithinBufferTime(QDate date, TimeSlot slot)
{
	QDateTime appointmentStartTime(date, slot.getStartTime());
	QDateTime currentTime = QDateTime::currentDateTime();
	return currentTime.addSecs(3600) <= appointmentStartTime;
}

//The appointment can be made only within a 2-month period
bool Manager::isWithinDateLimit(QDate date)
{
	QDate today = QDate::currentDate();
	QDate maxDate = today.addMonths(2);
	return (date >= today && date <=maxDate);
}

//Combine all logic rules
bool Manager::validateAppointment(QString patientID, QString doctorID, QDate date, TimeSlot slot) 
{
	if (!isWithinDateLimit(date)) return false;
	if (!isWithinBufferTime(date, slot)) return false;
	if (!isValidSlot(doctorID, date, slot)) return false;
	if (exceedsDailyLimit(patientID, date)) return false;
	if (hasDoctorConflict(doctorID, date, slot)) return false;
	if (hasPatientConflict(patientID, date, slot)) return false;
	return true;
}

bool Manager::addAppointment(Appointment a)
{
	if(validateAppointment(a.getPatientID(), a.getDoctorID(), a.getDate(), a.getTimeSlot()))
	{
		appointments.append(a);
		saveAppointments();
		return true;
	}
	return false;
}

bool Manager::isNationalIDUnique(QString nationalID)
{
	for (auto &a : patients)
	{
		if (a.getNationalID() == nationalID)
			return false;
	}
	return true;
}

QList<Doctor> Manager::filterDoctorsByName(QString name)
{
	QList<Doctor> results;
	for (auto &d : doctors)
	{
		if(d.getName().contains(name, Qt::CaseInsensitive))
			results.append(d);
	}
	return results;
}

QList<Doctor> Manager::filterDoctorsByDepartment(QString dept)
{
	QList<Doctor> results;
	for (auto &d : doctors)
	{
		if(d.getDepartment().contains(dept, Qt::CaseInsensitive))
			results.append(d);
	}
	return results;
}

bool Manager::addPatient(Patient p)
{
	if(isNationalIDUnique(p.getNationalID()))
	{
		patients.append(p);
		savePatientData();
		return true;
	}
	return false;
}

Doctor* Manager::getDoctorByID(QString id)
{
	for (int i = 0; i < doctors.size(); ++i)
	{
		if (doctors[i].getID() == id)
			return &doctors[i];
	}
	return nullptr;
}

Patient* Manager::getPatientByID(QString id) 
{
        for (int i = 0; i < patients.size(); ++i) 
        {
                if (patients[i].getNationalID() == id)
                        return &patients[i];
        }
        return nullptr;
}
