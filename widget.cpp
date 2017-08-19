#include "widget.h"
#include "ui_widget.h"

#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	setWindowTitle("OSM Places CSV to HPP Converter");
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_open_csv_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("CSV"), "",
	tr("CSV files (*.csv);;All Files (*)"));
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);

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
		//ui->textEdit->append("X: " + list[0] + "\nY: " + list[1] + "\nOSM_ID: " + list[2] + "\ncode: " + list[3] + "\nFCLASS: " + list[4] + "\nname: " + list[5] + "\npopulation: " + list[6]);

		// name
		QString placeName = list[5];
		// x
		float xCoord = list[0].toFloat(&ok);
		// subtract the terrain builder easting 200,000 coord shizzle
		xCoord = (xCoord - 200000);
		// y
		float yCoord = list[1].toFloat(&ok);
		// type
		tempPlace = list[4];
		armaType();
		QString placeType = armaPlace;
		// nameDigit
		nameDigit++;

		ui->textEdit->append("class place" + QString::number(nameDigit) + "\n{\n\tname = \"" + placeName +
				     "\";\n\tposition[] = { " + QString::number(xCoord) + ", " + QString::number(yCoord) +
				     " };\n\ttype = \"" + placeType + "\";\n\tradiusA = 200;\n\tradiusB = 200;\n\tangle = 0;\n};");
	}
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


void Widget::on_open_csv_pmc_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("CSV"), "",
	tr("CSV files (*.csv);;All Files (*)"));
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);

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

/*
official OSM place format:
X,Y,osm_id,code,fclass,population,name

PMC manually created shapefile format:
X,Y,NAME,FCLASS

class kerala
{
	name="Kerala";
	position[]={26577.84,35438.98};
	type="NameVillage";

	radiusA=200.00;
	radiusB=200.00;
	angle=0.000;
};

http://wiki.openstreetmap.org/wiki/Key:place
city
national_capital
town
village
county
hamlet
locality
suburb

08-19-17: came across "peak" for mountain / hill.

arma3 cfgLocationTypes https://community.bistudio.com/wiki/Location
NameCity
NameCityCapital
NameMarine
NameVillage
NameLocal (Will return names like Airport)
Hill
Mount
Airport (On Tanoa only. Tanoan airports have their own location type. On Altis and Stratis, airports are NameLocal)
*/
