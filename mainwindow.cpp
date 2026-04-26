#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDateEdit>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    manager = new Manager();
    datePicker = new QDateEdit(QDate::currentDate());
    datePicker->setCalendarPopup(true);
    datePicker->setDisplayFormat("MM/dd/yyyy");

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- FILTER SECTION ---
    QHBoxLayout *filterLayout = new QHBoxLayout();

    nameFilter = new QLineEdit();
    nameFilter->setPlaceholderText("Filter by Doctor...");

    deptFilter = new QLineEdit();
    deptFilter->setPlaceholderText("Filter by Department...");

    datePicker = new QDateEdit(QDate::currentDate());
    datePicker->setCalendarPopup(true);

    QPushButton *filterBtn = new QPushButton("Apply Filters / Refresh");

    filterLayout->addWidget(new QLabel("Doctor:"));
    filterLayout->addWidget(nameFilter);
    filterLayout->addWidget(new QLabel("Dept:"));
    filterLayout->addWidget(deptFilter);
    filterLayout->addWidget(new QLabel("Date:"));
    filterLayout->addWidget(datePicker);
    filterLayout->addWidget(filterBtn);

    // --- TIMETABLE SECTION ---
    table = new QTableWidget(0, 5);
    table->setHorizontalHeaderLabels({"Time", "Doctor", "Department", "Patient / Status", "Action"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // --- ACTION BUTTONS ---
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *regBtn = new QPushButton("Register New Patient Profile");
    QPushButton *updateMobileBtn = new QPushButton("Update Patient Mobile Number");
    actionLayout->addWidget(regBtn);
    actionLayout->addWidget(updateMobileBtn);

    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(table);
    mainLayout->addLayout(actionLayout);
    setCentralWidget(centralWidget);

    // --- CONNECTIONS ---
    connect(filterBtn, &QPushButton::clicked, this, &MainWindow::displayTimetable);
    connect(regBtn, &QPushButton::clicked, this, &MainWindow::openPatientRegistration);

    connect(updateMobileBtn, &QPushButton::clicked, [=](){
        bool ok;
        QString id = QInputDialog::getText(this, "Update Record", "Enter Patient National ID:", QLineEdit::Normal, "", &ok);
        if(ok && !id.isEmpty()){
            Patient* p = manager->getPatientByID(id);
            if(p){
                QString newMob = QInputDialog::getText(this, "Update Mobile", "Enter New Number:", QLineEdit::Normal, p->getMobileNumber(), &ok);
                if(ok && !newMob.isEmpty()){
                    p->setMobileNumber(newMob);
                    manager->savePatientData();
                    QMessageBox::information(this, "Success", "Mobile number updated and saved!");
                    displayTimetable();
                }
            } else {
                QMessageBox::warning(this, "Error", "Patient ID not found in system.");
            }
        }
    });

    displayTimetable();
}

MainWindow::~MainWindow() {
    delete manager;
}

void MainWindow::displayTimetable() {
    table->setRowCount(0);

    QDate selectedDate = datePicker->date();
    QString nFilter = nameFilter->text().trimmed();
    QString dFilter = deptFilter->text().trimmed();

    // Get doctors matching Name filter
    QList<Doctor> doctorsToShow = manager->filterDoctorsByName(nFilter);

    for (const auto &doc : doctorsToShow) {
        // Apply Department Filter manually
        if (!dFilter.isEmpty() && !doc.getDepartment().contains(dFilter, Qt::CaseInsensitive)) {
            continue;
        }

        QList<TimeSlot> availableSlots = manager->generate20MinSlots(doc.getID(), selectedDate);

        for (auto &s : availableSlots) {
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(s.getStartTime().toString("hh:mm")));
            table->setItem(row, 1, new QTableWidgetItem(doc.getName()));
            table->setItem(row, 2, new QTableWidgetItem(doc.getDepartment()));

            QString statusText = "Available";
            bool isBooked = false;

            for(auto &app : manager->loadAllAppointmentsInternal()) {
                if(app.getDoctorID() == doc.getID() &&
                    app.getDate() == selectedDate &&
                    app.getStartTime() == s.getStartTime()) {
                    Patient* p = manager->getPatientByID(app.getPatientID());
                    statusText = (p) ? "Booked: " + p->getName() : "Booked";
                    isBooked = true;
                    break;
                }
            }

            table->setItem(row, 3, new QTableWidgetItem(statusText));

            if(!isBooked) {
                QPushButton *bookBtn = new QPushButton("Book Appointment");
                table->setCellWidget(row, 4, bookBtn);

                connect(bookBtn, &QPushButton::clicked, [=]() {
                    bool ok;
                    QString pID = QInputDialog::getText(this, "New Booking",
                                                        "Enter Patient National ID:", QLineEdit::Normal, "", &ok);

                    if (ok && !pID.isEmpty()) {
                        if(manager->getPatientByID(pID) == nullptr) {
                            QMessageBox::critical(this, "Error", "Patient ID not found! Register the patient first.");
                            return;
                        }

                        Appointment newApp(doc.getID(), pID, selectedDate, s);
                        if (manager->addAppointment(newApp)) {
                            QMessageBox::information(this, "Success", "Appointment Confirmed!");
                            displayTimetable();
                        }
                    }
                });
            } else {
                table->setItem(row, 4, new QTableWidgetItem("N/A"));
            }
        }
    }
}

void MainWindow::openPatientRegistration() {
    QDialog diag(this);
    diag.setWindowTitle("Patient Intake Form");
    QFormLayout form(&diag);

    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *idEdit = new QLineEdit();
    QDateEdit *dobEdit = new QDateEdit(QDate(2000, 1, 1));
    QComboBox *genderEdit = new QComboBox();
    genderEdit->addItems({"Male", "Female"});
    QLineEdit *mobileEdit = new QLineEdit();

    form.addRow("Patient Name:", nameEdit);
    form.addRow("National ID:", idEdit);
    form.addRow("Birthdate:", dobEdit);
    form.addRow("Gender:", genderEdit);
    form.addRow("Mobile Number:", mobileEdit);

    QPushButton *saveBtn = new QPushButton("Create Record");
    form.addRow(saveBtn);

    connect(saveBtn, &QPushButton::clicked, [&]() {
        // Enforce Required Fields
        if(nameEdit->text().trimmed().isEmpty() ||
            idEdit->text().trimmed().isEmpty() ||
            mobileEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&diag, "Required Fields", "Name, ID, and Mobile are required.");
            return;
        }

        Patient::Gender g = (genderEdit->currentIndex() == 0) ? Patient::Male : Patient::Female;
        Patient p(idEdit->text(), nameEdit->text(), dobEdit->date(), g, mobileEdit->text());

        if (manager->addPatient(p)) {
            QMessageBox::information(&diag, "Success", "Patient Registered!");
            diag.accept();
        } else {
            QMessageBox::warning(&diag, "Error", "National ID already exists!");
        }
    });

    diag.exec(); // Don't forget this!
}

void MainWindow::refreshTimetable() { displayTimetable(); }
void MainWindow::handleBookingClick() {}
void MainWindow::applyFilters(QString n, QString d) { displayTimetable(); }
