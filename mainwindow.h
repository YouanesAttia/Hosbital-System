#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QInputDialog>

#include "manager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Manager manager;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void displayTimetable();
    void openPatientRegistration();
    void handleBookingClick();
    void applyFilters();
    void refreshTimetable();

private slots:
    void on_bookButton_clicked();
    void on_registerButton_clicked();
    void on_doctorComboBox_currentIndexChanged(int index);
};

#endif
