#include "qtyunbi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
#include <QJsonDocument>
#include <QSslConfiguration>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextBlock>
#include <QTextCursor>
#include <QDateTime>

const int QtYunbi::UPDATE_INTERVAL = 10*1000;
#define DATETIMEFORMAT "yyyy-MM-dd hh:mm:ss"
QtYunbi::QtYunbi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QTimer::singleShot(UPDATE_INTERVAL, this, SLOT(OnUpdate()));
}

QtYunbi::~QtYunbi()
{

}

void QtYunbi::OnUpdate()
{
	QNetworkRequest networkRequest;
	networkRequest.setUrl(QString("https://yunbi.com:443//api/v2/tickers/ethcny.json"));
	QSslConfiguration config = networkRequest.sslConfiguration();
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	networkRequest.setSslConfiguration(config);
	QPointer<QNetworkReply> pNetworkReply = networkAccessManager.get(networkRequest);
	QEventLoop eventLoop;
	connect(pNetworkReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
	eventLoop.exec();
	if (!pNetworkReply.isNull())
	{
		bool is_error = pNetworkReply->error();
		if (is_error)
		{
			QNetworkReply::NetworkError error = pNetworkReply->error();
		}
		else
		{
			while (true)
			{
				QByteArray content = pNetworkReply->readAll();
				QString qstrContent;
				qstrContent.prepend(content);
				QJsonParseError json_error;
				QJsonDocument parse_doucment = QJsonDocument::fromJson(content, &json_error);
				if (json_error.error != QJsonParseError::NoError)
					break;

				if (!parse_doucment.isObject())
					break;
				QJsonObject obj = parse_doucment.object();
				if (!obj.contains("ticker"))
					break;
				QJsonValue ticker_value = obj.take("ticker");
				if (!ticker_value.isObject())
					break;
				QVariant var = ticker_value.toVariant();
				if (var.type() != QVariant::Map)
					break;
				QVariantMap varMap = var.toMap();
				double varLast = varMap.value("last").toString().toDouble();
				double dMin = ui.leMin->text().toDouble();
				double dMax = ui.leMax->text().toDouble();

				qint64 currTime = QDateTime::currentMSecsSinceEpoch();
				QString qstrTime = QDateTime::fromMSecsSinceEpoch(currTime).toString(DATETIMEFORMAT);
				QString qstrEth;
				qstrEth.append(qstrTime).append("    ").append(QString::number(varLast));
				ui.lbEth->setText(qstrEth);

				if (varLast < dMin)
				{
					QString info(qstrTime);
					info.append("    ").append(QString::number(varLast));
					InsertLine(info);
				}
				if (varLast > dMax)
				{
					QString info(qstrTime);
					info.append("    ").append(QString::number(varLast));
					InsertLine(info);
				}
			}

		}
		QTimer::singleShot(UPDATE_INTERVAL, this, SLOT(OnUpdate()));
	}
}

void QtYunbi::InsertLine(QString text)
{
	text.append("\r");
	ui.plainTextEdit->setFocus();

	const QTextBlock block = ui.plainTextEdit->document()->findBlockByLineNumber(0);
	if (block.isValid())
	{
		QTextCursor cursor = ui.plainTextEdit->textCursor();
		cursor.setPosition(0);
		ui.plainTextEdit->setTextCursor(cursor);
		ui.plainTextEdit->ensureCursorVisible();
		ui.plainTextEdit->insertPlainText(text);
	}
}