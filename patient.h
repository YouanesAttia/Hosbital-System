#include <QString>
#include <QDate>


class Patient
{
public:
	enum Gender { Male, Female };
private:
	QString nationalID;
	QString name;
	QString mobileNumber;
	QDate birthdate;
	Gender gender;
public:
	Patient(QString i, QString n, QDate b, Gender g, QString m) 
        : nationalID(i), name(n), birthdate(b), gender(g), mobileNumber(m) {}
	void setMobileNumber(QString m){mobileNumber = m;}
	QString getNationalID() const { return nationalID; }
	QString getName() const { return name; }
	QDate getBirthdate() const { return birthdate; }
	Gender getGender() const { return gender; }
	QString getMobileNumber() const{return mobileNumber;}
};

