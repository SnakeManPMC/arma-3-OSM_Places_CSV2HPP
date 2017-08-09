#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

private slots:
	void on_open_csv_clicked();
	void on_open_csv_pmc_clicked();

private:
	Ui::Widget *ui;
	QString tempPlace,armaPlace;
	void armaType();
};

#endif // WIDGET_H
