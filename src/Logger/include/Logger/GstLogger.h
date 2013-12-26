#ifndef GSTLOGGER_H
#define GSTLOGGER_H

#include <QtWidgets>
#include <gstreamermm.h>
#include <mutex>

namespace Ui {
class GstLogger;
}

class GstLogger : public QWidget
{
	Q_OBJECT

public:
	explicit GstLogger(QWidget *parent = 0);
	~GstLogger();
	void add_log(GstDebugCategory * category,
			GstDebugLevel      level,
			const gchar      * file,
			const gchar      * function,
			gint               line,
			GObject          * object,
			GstDebugMessage  * message);

private:
	Ui::GstLogger *ui;
	std::mutex widget_locker;
	std::vector<QCheckBox*> config_checkboxes;

	void set_if_checked(int row, int col, const QString& value);
};

#endif // GSTLOGGER_H
