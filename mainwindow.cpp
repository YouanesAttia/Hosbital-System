#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    
    ui->doctorComboBox->addItem("D1");
    ui->doctorComboBox->addItem("D2");
    ui->doctorComboBox->addItem("D3");

    displayTimetable();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::displayTimetable()
{
    ui->timetableWidget->clearContents();
    ui->timetableWidget->setRowCount(0);

    QString doctorID =
        ui->doctorComboBox->currentText();

    QDate date =
        ui->dateEdit->date();

    QList<TimeSlot> slots =
        manager.generate20MinSlots(
            doctorID,
            date
        );

    for(int i=0; i<slots.size(); i++)
    {
        ui->timetableWidget->insertRow(i);

        QString slotText =
            slots[i].getStartTime().toString("h:mm")
            + " - " +
            slots[i].getEndTime().toString("h:mm");

        ui->timetableWidget->setItem(
            i,
            0,
            new QTableWidgetItem(slotText)
        );
    }
}


void MainWindow::openPatientRegistration()
{
    QString id =
        QInputDialog::getText(
            this,
            "Patient Registration",
            "Enter National ID:"
        );

    QString name =
        QInputDialog::getText(
            this,
            "Patient Registration",
            "Enter Name:"
        );

    QString phone =
        QInputDialog::getText(
            this,
            "Patient Registration",
            "Enter Mobile Number:"
        );

    Patient p(
        id,
        name,
        phone,
        QDate(2000,1,1),
        Patient::Male
    );

    if(manager.addPatient(p))
    {
        QMessageBox::information(
            this,
            "Success",
            "Patient added successfully"
        );
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "National ID already exists"
        );
    }
}


void MainWindow::handleBookingClick()
{
    int row =
        ui->timetableWidget->currentRow();

    if(row < 0)
    {
        QMessageBox::warning(
            this,
            "Error",
            "Please select a time slot"
        );
        return;
    }

    QString doctorID =
        ui->doctorComboBox->currentText();

    QString patientID =
        ui->patientIdLineEdit->text();

    QDate date =
        ui->dateEdit->date();

    QList<TimeSlot> slots =
        manager.generate20MinSlots(
            doctorID,
            date
        );

    TimeSlot selectedSlot = slots[row];

    Appointment a(
        doctorID,
        patientID,
        date,
        selectedSlot
    );

    if(manager.addAppointment(a))
    {
        QMessageBox::information(
            this,
            "Booked",
            "Appointment confirmed"
        );
    }
    else
    {
        QMessageBox::warning(
            this,
            "Rejected",
            "Booking violates rules"
        );
    }

    refreshTimetable();
}


void MainWindow::applyFilters()
{
    QString search =
        ui->filterLineEdit->text();

    QList<Doctor> docs =
        manager.filterDoctorsByName(search);

    ui->doctorComboBox->clear();

    for(auto &d : docs)
    {
        ui->doctorComboBox->addItem(
            d.getID()
        );
    }
}


void MainWindow::refreshTimetable()
{
    displayTimetable();
}


void MainWindow::on_bookButton_clicked()
{
    handleBookingClick();
}


void MainWindow::on_registerButton_clicked()
{
    openPatientRegistration();
}


void MainWindow::on_doctorComboBox_currentIndexChanged(int)
{
    refreshTimetable();
}
