#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateEdit>
#include <QLineEdit>
#include "manager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Manager* manager;
    QLineEdit* nameFilter;
    QLineEdit* deptFilter;
    QDateEdit* datePicker;
    QTableWidget* table;

public:
     MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void displayTimetable();
    void openPatientRegistration();
    void handleBookingClick();
    void applyFilters(QString name, QString dept);
    void refreshTimetable();
};

#endif

