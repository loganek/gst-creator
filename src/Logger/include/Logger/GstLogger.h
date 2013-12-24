#ifndef GSTLOGGER_H
#define GSTLOGGER_H

#include <QWidget>

namespace Ui {
class GstLogger;
}

class GstLogger : public QWidget
{
    Q_OBJECT

public:
    explicit GstLogger(QWidget *parent = 0);
    ~GstLogger();

private:
    Ui::GstLogger *ui;
};

#endif // GSTLOGGER_H
