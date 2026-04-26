#include "manager.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

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

        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if(fields.size() >= 4)
            {
                QString id = fields[0].trimmed();
                QString day = fields[1].trimmed();
                QTime start = QTime::fromString(fields[2].trimmed(), "hh:mm");
                QTime end = QTime::fromString(fields[3].trimmed(), "hh:mm");
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
                patients.append(Patient(f[0].trimmed(), f[1].trimmed(), QDate::fromString(f[2].trimmed(), Qt::ISODate), g, f[4].trimmed()));
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
                << app.getTimeSlot().getStartTime().toString("hh:mm") << ","
                << app.getTimeSlot().getEndTime().toString("hh:mm") << "\n";
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


bool Manager::hasDoctorConflict(QString doctorID, QDate date, TimeSlot slot)
{
    for (auto &a : appointments)
    {
        if(a.getDoctorID() == doctorID && a.getDate() == date && a.getStartTime() == slot.getStartTime())
        {
            return true;
        }
    }
    return false;
}

// A patient can't have more than two appointments in the same day
bool Manager::exceedsDailyLimit(QString patientID, QDate date)
{
    int count = 0;
    for (auto &a : appointments)
    {
        if(a.getPatientID() == patientID && a.getDate() == date)
        {
            count ++;
        }
    }
    return (count >= 2);
}

// The appointment must be reserved at least one hour from the current time
bool Manager::isWithinBufferTime(QDate date, TimeSlot slot)
{
    QDateTime appointmentStartTime(date, slot.getStartTime());
    QDateTime currentTime = QDateTime::currentDateTime();
    return currentTime.addSecs(3600) <= appointmentStartTime;
}

//Ensure that the appointment is reserved max after 2 months.
bool Manager::isWithinDateLimit(QDate date)
{
    QDate today = QDate::currentDate();
    QDate maxDate = today.addMonths(2);
    return (date >= today && date <=maxDate);
}

bool Manager::validateAppointment(QString patientID, QString doctorID, QDate date, TimeSlot slot) {
    if (!isWithinDateLimit(date)) {
        QMessageBox::warning(nullptr, "Error", "Appointment must be within 2 months.");
        return false;
    }
    if (!isWithinBufferTime(date, slot)) {
        QMessageBox::warning(nullptr, "Error", "Reservations must be made at least 1 hour in advance.");
        return false;
    }
    if (!isValidSlot(doctorID, date, slot)) {
        QMessageBox::warning(nullptr, "Error", "This slot is not in the doctor's working schedule.");
        return false;
    }
    if (exceedsDailyLimit(patientID, date)) {
        QMessageBox::warning(nullptr, "Error", "This patient already has 2 appointments today (Limit reached).");
        return false;
    }
    if (hasDoctorConflict(doctorID, date, slot)) {
        QMessageBox::warning(nullptr, "Error", "The doctor is already booked for this time.");
        return false;
    }
    if (hasPatientConflict(patientID, date, slot)) {
        QMessageBox::warning(nullptr, "Error", "The patient already has another appointment at this time.");
        return false;
    }
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
