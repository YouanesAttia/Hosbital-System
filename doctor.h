#include <QString>

class Doctor
{
private:
	QString id, name, department;
public:
	Doctor (QString i, QString n, QString d): id(i), name(n), department(d){}
	QString getID() const {return id;}
	QString getDepartment() const {return department;}
	QString getName() const {return name;}
};
