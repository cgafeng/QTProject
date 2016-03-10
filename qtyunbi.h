#ifndef QTYUNBI_H
#define QTYUNBI_H

#include <QtWidgets/QMainWindow>
#include "ui_qtyunbi.h"
#include <QTimer>
#include <QNetworkAccessManager>

class QtYunbi : public QMainWindow
{
	Q_OBJECT

public:
	QtYunbi(QWidget *parent = 0);
	~QtYunbi();

protected slots :
	void OnUpdate();

private:
	void InsertLine(QString text);
	Ui::QtYunbiClass ui;
	QTimer timerUpdate;
	QNetworkAccessManager networkAccessManager;

	static const int UPDATE_INTERVAL;
};

#endif // QTYUNBI_H
