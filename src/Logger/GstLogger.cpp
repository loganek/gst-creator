#include "GstLogger.h"
#include "GstLoggerProperties.h"
#include "ui_GstLogger.h"

GstLogger::GstLogger(QWidget *parent)
: QWidget(parent),
  ui(new Ui::GstLogger)
{
	ui->setupUi(this);

	connect(ui->propertiesPushButton, &QPushButton::clicked, [](bool checked){
		GstLoggerProperties log_prop;
		if (log_prop.exec())
		{
			if (log_prop.is_default_level())
				gst_debug_set_default_threshold(log_prop.get_default_level());
			else
			{
				auto levels = log_prop.get_speciefied_levels();

				for (auto l : levels)
					gst_debug_category_set_threshold(l.first, l.second);
			}
		}
	});
}

GstLogger::~GstLogger()
{
	delete ui;
}
