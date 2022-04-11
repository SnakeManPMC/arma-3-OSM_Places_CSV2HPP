#include "widget.h"
#include "ui_widget.h"

#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <QDebug>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	setWindowTitle("OSM Places CSV to HPP Converter v0.1.3 by PMC");
}

Widget::~Widget()
{
	delete ui;
}

// QGIS csv
void Widget::on_open_csv_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("CSV"), "",
	tr("CSV files (*.csv);;All Files (*)"));
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
    in.setCodec("UTF-8");

	// header
	QString line;
	line = in.readLine();

	bool ok;
	int nameDigit = 0;

	while (!in.atEnd())
	{
		line = in.readLine();
		QStringList list;
		list = line.split(",");

		//X,Y,fid,cat,osm_id,code,fclass,population,name
		//ui->textEdit->append("X: " + list[0] + "\nY: " + list[1] + "\nOSM_ID: " + list[2] + "\ncode: " + list[3] + "\nFCLASS: " + list[6] + "\npopulation: " + list[7] + "\nname: " + list[8]);
		// name
		QString placeName = list[8];
		// x
		float xCoord = list[0].toFloat(&ok);
		// subtract the terrain builder easting 200,000 coord shizzle
		xCoord = (xCoord - 200000);
		// y
		float yCoord = list[1].toFloat(&ok);
		// type
		tempPlace = list[6];
		armaType();
		QString placeType = armaPlace;
		// nameDigit
		nameDigit++;

		ui->textEdit->append("class place" + QString::number(nameDigit) + "\n{\n\tname = \"" + placeName +
				     "\";\n\tposition[] = { " + QString::number(xCoord) + ", " + QString::number(yCoord) +
				     " };\n\ttype = \"" + placeType + "\";\n\tradiusA = 200;\n\tradiusB = 200;\n\tangle = 0;\n};");
	}
}


// QGIS csv but with limited PMC format
void Widget::on_open_csv_pmc_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("CSV"), "",
	tr("CSV files (*.csv);;All Files (*)"));
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	in.setCodec("UTF-8");

	// header
	QString line;
	line = in.readLine();

	bool ok;
	int nameDigit = 0;

	while (!in.atEnd())
	{
		line = in.readLine();
		QStringList list;
		list = line.split(",");
		//ui->textEdit->append("X: " + list[0] + " Y: " + list[1] + " OSM_ID: " + list[2] + " code: " + list[3] + " FCLASS: " + list[4] + " population: " + list[5] + " name: " + list[6]);

		// name
		QString placeName = list[2];
		// x
		float xCoord = list[0].toFloat(&ok);
		// subtract the terrain builder easting 200,000 coord shizzle
		xCoord = (xCoord - 200000);
		// y
		float yCoord = list[1].toFloat(&ok);
		// type
		tempPlace = list[3];
		armaType();
		QString placeType = armaPlace;
		// nameDigit
		nameDigit++;

		ui->textEdit->append("class place" + QString::number(nameDigit) + "\n{\n\tname = \"" + placeName +
				     "\";\n\tposition[] = { " + QString::number(xCoord) + ", " + QString::number(yCoord) +
				     " };\n\ttype = \"" + placeType + "\";\n\tradiusA = 200;\n\tradiusB = 200;\n\tangle = 0;\n};");
	}
}


// Global Mapper csv
void Widget::on_GlobalMapper_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("CSV"), "",
	tr("CSV files (*.csv);;All Files (*)"));
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
    in.setCodec("UTF-8");

	// header
	QString line;
	line = in.readLine();
    QStringList list;

	bool ok;
	int nameDigit = 0;

	while (!in.atEnd())
	{
		line = in.readLine();
        qDebug() << "line is: " << line;
		list = line.split(",");
        qDebug() << "list.size: " << list.size();
        qDebug() << "list[0]: " << list[0];

		// name
        QString placeName = list[(list.size()-1)];
		// x
		float xCoord = list[0].toFloat(&ok);
		// subtract the terrain builder easting 200,000 coord shizzle
		xCoord = (xCoord - 200000);
		// y
		float yCoord = list[1].toFloat(&ok);
		// type
        qDebug() << list;
        tempPlace = list[1];
		armaType();
		QString placeType = armaPlace;
		// nameDigit
		nameDigit++;

		ui->textEdit->append("class place" + QString::number(nameDigit) + "\n{\n\tname = \"" + placeName +
				     "\";\n\tposition[] = { " + QString::number(xCoord) + ", " + QString::number(yCoord) +
				     " };\n\ttype = \"" + placeType + "\";\n\tradiusA = 200;\n\tradiusB = 200;\n\tangle = 0;\n};");
	}
}


// Global Mapper csv but with limited PMC format
void Widget::on_GlobalMapperPMC_clicked()
{
	// uhm, not used? :)
}


// sets cfgLocationType to nameLocal unless we find specific ones like city or village etc
void Widget::armaType()
{
	// set default arma3 cfgLocationType
	armaPlace = "nameLocal";

	if (!tempPlace.compare("village", Qt::CaseInsensitive))
	{
		armaPlace = "nameVillage";
	}

	if (!tempPlace.compare("town", Qt::CaseInsensitive))
	{
		armaPlace = "nameCity";
	}

	if (!tempPlace.compare("city", Qt::CaseInsensitive))
	{
		armaPlace = "nameCity";
	}

	if (!tempPlace.compare("peak", Qt::CaseInsensitive))
	{
		armaPlace = "Hill";
	}
}
