#include "qtyunbi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtYunbi w;
	w.show();
	return a.exec();
}
