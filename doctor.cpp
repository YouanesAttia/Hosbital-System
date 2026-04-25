include "Doctor.h"


Doctor::Doctor()
{
}


Doctor::Doctor(QString docID,
               QString docName,
               QString dept)
{
    id=docID;
    name=docName;
    department=dept;
}
QString Doctor::getID() const
{
    return id;
}

QString Doctor::getName() const
{
    return name;
}

QString Doctor::getDepartment() const
{
    return department;
}
