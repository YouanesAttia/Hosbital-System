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
