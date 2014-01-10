#include "GstLogger.h"
#include "GstLoggerProperties.h"
#include "ui_GstLogger.h"
#include <functional>

#define MAX_LOGS 3000

void log_function(GstDebugCategory * category,
		GstDebugLevel      level,
		const gchar      * file,
		const gchar      * function,
		gint               line,
		GObject          * object,
		GstDebugMessage  * message,
		gpointer           user_data)
{
	GstLogger* logger = static_cast<GstLogger*>(user_data);

	logger->add_log(category, level, file, function, line, object, message);
}


GstLogger::GstLogger(QWidget *parent)
: QWidget(parent),
  ui(new Ui::GstLogger)
{
	ui->setupUi(this);
	config_checkboxes = {
		ui->categoryCheckBox,
		ui->levelCheckBox,
		ui->fileCheckBox,
		ui->functionCheckBox,
		ui->lineCheckBox,
		ui->messageCheckBox
	};

	for (int i = 0; i < 6; i++)
		connect(config_checkboxes[i], &QCheckBox::stateChanged, [this, i](int state){
			ui->logsTableWidget->setColumnHidden(i, state == 0);
	});

	typedef QVector<int> intvect;
	qRegisterMetaType<intvect>("intvect");
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

	ui->logsTableWidget->setColumnHidden(3, true);

	gst_debug_remove_log_function(gst_debug_log_default);
	gst_debug_add_log_function(log_function, this, nullptr);

	ui->logsTableWidget->setHorizontalHeaderLabels(
			{"Category", "Level", "File", "Function", "Line", "Message"});
}

void GstLogger::add_log(GstDebugCategory * category,
		GstDebugLevel      level,
		const gchar      * file,
		const gchar      * function,
		gint               line,
		GObject          * object,
		GstDebugMessage  * message)
{
	widget_locker.lock();

	int row = ui->logsTableWidget->rowCount();
	if (row > MAX_LOGS)
	{
		ui->logsTableWidget->removeRow(0);
		row--;
	}
	ui->logsTableWidget->insertRow(row);

	set_if_checked(row, 0, gst_debug_category_get_name(category));
	set_if_checked(row, 1, gst_debug_level_get_name(level));
	set_if_checked(row, 2, file);
	set_if_checked(row, 3, function);
	set_if_checked(row, 4, QString::number(line));
	set_if_checked(row, 5, gst_debug_message_get(message));
	widget_locker.unlock();
}

void GstLogger::set_if_checked(int row, int col, const QString& value)
{
	if (config_checkboxes[col]->isChecked())
		ui->logsTableWidget->setItem(row, col, new QTableWidgetItem(value));
}

GstLogger::~GstLogger()
{
	delete ui;
}
