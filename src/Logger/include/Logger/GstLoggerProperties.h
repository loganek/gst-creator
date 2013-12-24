#ifndef GSTLOGGERPROPERTIES_H
#define GSTLOGGERPROPERTIES_H

#include <QtWidgets>
#include <gst/gst.h>
#include <map>

namespace Ui {
class GstLoggerProperties;
}

class GstLoggerProperties : public QDialog
{
	Q_OBJECT

public:
	explicit GstLoggerProperties(QWidget *parent = 0);
	~GstLoggerProperties();

	bool is_default_level() const;
	GstDebugLevel get_default_level() const;
	std::map<GstDebugCategory*, GstDebugLevel> get_speciefied_levels() const;


private:
	Ui::GstLoggerProperties *ui;
};

#endif // GSTLOGGERPROPERTIES_H
