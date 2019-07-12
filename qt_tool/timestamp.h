#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <QDateTime>
#include <QObject>

class timestamp : public QObject
{
    Q_OBJECT
public:
    timestamp();
    ~timestamp();
    uint32_t get_time_now();
    QString get_current_time(uint32_t timestamp);
    QString get_utc_time(uint32_t timestamp);
private:
    uint32_t m_utc;
    QString m_currenttime;
    QString m_utctime;
};

#endif // TIMESTAMP_H
