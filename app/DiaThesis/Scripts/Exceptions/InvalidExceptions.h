///-----------------------------------------------------------------
/// Namespace:
/// Class:              InvalidDateTimeFormate, InvalidUser
/// Description:        Exception for ever invalid action
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:
///                     to ISO 8601.
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef INVALIDEXCEPTIONS_H
#define INVALIDEXCEPTIONS_H

#include <QException>

class InvalidDateTimeFormate : public QException
{
private:
    const char *m_message;
public:
    void raise() const override { throw *this; }
    InvalidDateTimeFormate *clone() const override { return new InvalidDateTimeFormate(*this); }
    InvalidDateTimeFormate(const char *desc) {m_message = desc; }

    QString getMessage(void) const {return m_message;}
};

class InvalidDateTimeRange : public QException
{
private:
    const char *m_message;
public:
    void raise() const override { throw *this; }
    InvalidDateTimeRange *clone() const override { return new InvalidDateTimeRange(*this); }
    InvalidDateTimeRange(const char *desc) {m_message = desc; }

    QString getMessage(void) const {return m_message;}
};

class InvalidUser : public QException
{
private:
    const char *m_message;
public:
    void raise() const override { throw *this; }
    InvalidUser *clone() const override { return new InvalidUser(*this); }
    InvalidUser(const char *desc) {m_message = desc; }

    QString getMessage(void) const {return m_message;}
};

class UserNotFound : public QException
{
private:
    const char *m_message;
public:
    void raise() const override { throw *this; }
    UserNotFound *clone() const override { return new UserNotFound(*this); }
    UserNotFound(const char *desc) {m_message = desc; }

    QString getMessage(void) const {return m_message;}
};

class SqlError : public QException
{
private:
    QString m_message;
public:
    void raise() const override { throw *this; }
    SqlError *clone() const override { return new SqlError(*this); }
    SqlError(const QString desc) {m_message = desc; }

    QString getMessage(void) const {return m_message;}
};

#endif // INVALIDEXCEPTIONS_H
