#include "GstLoggerProperties.h"
#include "ui_GstLoggerProperties.h"

GstLoggerProperties::GstLoggerProperties(QWidget *parent)
: QDialog(parent),
  ui(new Ui::GstLoggerProperties)
{
	ui->setupUi(this);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	ui->tableWidget->setHorizontalHeaderLabels({"Category", "Debug Level"});

	QStringList debug_levels = {
			"none", "error", "warning", "fixme", "info", "debug", "log", "trace"
	};

	ui->defaultDebugLevelComboBox->addItems(debug_levels);
	ui->defaultDebugLevelComboBox->setCurrentIndex(gst_debug_get_default_threshold());

	connect(ui->defaultRadioButton, &QRadioButton::toggled, [this](bool checked){
		ui->defaultDebugLevelComboBox->setEnabled(checked);
		ui->tableWidget->setEnabled(!checked);
	});
	connect(ui->specifiedRadioButton, &QRadioButton::toggled, [this](bool checked){
		ui->defaultDebugLevelComboBox->setEnabled(!checked);
		ui->tableWidget->setEnabled(checked);
	});

	GSList* categories = gst_debug_get_all_categories();

	while (categories)
	{
		GstDebugCategory* category = static_cast<GstDebugCategory*>(categories->data);
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 0,
				new QLineEdit(gst_debug_category_get_name(category)));

		auto cb = new QComboBox();
		cb->addItems(debug_levels);
		cb->setCurrentIndex(category->threshold);
		ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 1, cb);
		categories = categories->next;
	}

	g_slist_free(categories);
}

GstLoggerProperties::~GstLoggerProperties()
{
	delete ui;
}

bool GstLoggerProperties::is_default_level() const
{
	return ui->defaultRadioButton->isChecked();
}

GstDebugLevel GstLoggerProperties::get_default_level() const
{
	return (GstDebugLevel) ui->defaultDebugLevelComboBox->currentIndex();
}

std::map<GstDebugCategory*, GstDebugLevel> GstLoggerProperties::get_speciefied_levels() const
{
	std::map<GstDebugCategory*, GstDebugLevel> ret_map;

	for (int i = 0; i < ui->tableWidget->rowCount(); i++)
	{
		GstDebugCategory* cat = _gst_debug_get_category(
				static_cast<QLineEdit*>(ui->tableWidget->cellWidget(i, 0))->
				text().toStdString().c_str()
		);
		if (cat != nullptr)
			ret_map[cat] = (GstDebugLevel)static_cast<QComboBox*>(
					ui->tableWidget->cellWidget(i, 1))->currentIndex();
	}

	return ret_map;
}
