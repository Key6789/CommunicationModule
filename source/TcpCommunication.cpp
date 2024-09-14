#include "../include/TcpCommunication.h"
namespace  CommunicationModule
{
	TcpCommunication::TcpCommunication(const QString& ip, quint16 port, QWidget* parent)
	{
		m_serverIP = ip;
		m_serverPort = port;
		connectToServer(m_serverIP, m_serverPort);

		// 处理QTcpSocket连接状态变化的信号槽连接
		connect(this, &QTcpSocket::connected, [=]() {emit connectStatusChanged(true); });
		connect(this, &QTcpSocket::disconnected, [=]() {emit connectStatusChanged(false); });

		// 处理QTcpSocket读取数据的信号槽连接
		//connect(this, &QTcpSocket::readyRead, [=]() {emit readAll().isEmpty() ? 1 : emit receiveSendData(readAll()); });

		// 处理QTcpSocket错误信号的信号槽连接
		connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpCommunication::handleError);

		// 初始化并配置重连定时器
		m_reconnectTimer = new QTimer(this);
		m_reconnectTimer->setInterval(m_reconnectTime);
		connect(m_reconnectTimer, &QTimer::timeout, this, &TcpCommunication::startReconnect);
		m_reconnectTimer->start();


	}
	bool TcpCommunication::connectToServer(const QString& ip, quint16 port)
	{
		// 通讯连接
		connectToHost(ip, port);

		// 等待连接成功
		if (waitForConnected(m_waitTime))
		{
			// 连接成功状态
			emit connectStatusChanged(true);
			// 连接成功
			return true;
		}
		// 连接失败
		emit connectStatusChanged(false);
		return false;


	}
	void TcpCommunication::sendCommand(const QByteArray& data)
	{
		// 发送数据
		write(data);

		// 刷新缓冲区
		flush();
		
		emit showSendDataValue(data);
		emit sendDataFinished(true);
	}
	void TcpCommunication::receiveCommand(QByteArray& data)
	{
		// 接收数据
		if (data.size() > 0)
		{
			// 接收成功
			emit receiveCommand(data);
			return;
		}
	}
	void TcpCommunication::closeConnection()
	{
		// 关闭连接
		disconnectFromHost();
	}
	bool TcpCommunication::isConnected() const
	{
		// 判断是否连接成功
		return state() == QAbstractSocket::ConnectedState;
	}
	void TcpCommunication::setReconnectTime(int time)
	{
		if (!m_reconnectTimer)
		{
			m_reconnectTimer = new QTimer(this);
		}
		// 设置重新连接时间
		if (m_reconnectTimer->isActive())
		{
			m_reconnectTimer->stop();
		}
		m_reconnectTimer->setInterval(time);
		m_reconnectTimer->start();
	}
	void TcpCommunication::setWaitTime(int time)
	{
		// 设置等待连接时间
		m_waitTime = time;
	}
	void TcpCommunication::setReconnectCount(int count)
	{
		// 设置重连次数
		m_reconnectCount = count;
	}
	QWidget* TcpCommunication::showConnectStatus(QWidget* parent)
	{
		QWidget* widget = new QWidget(parent);
		QHBoxLayout* layout = new QHBoxLayout(widget);
		QLabel* label = new QLabel(widget);
		label->setText("连接状态：");
		layout->addWidget(label);
		connect(this, &TcpCommunication::connectStatusChanged, [=](bool status) {
			QString statusText = status ? "已连接" : "未连接"; 
			label->setText(QString("连接状态：" + statusText)); 
			status ? label->setStyleSheet("color: green") : label->setStyleSheet("color: red");
			});
		return widget;
	}
	QWidget* TcpCommunication::showSendData(QWidget* parent)
	{
		// 显示已发送数据
		QWidget* widget = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(widget);
		QLabel* label = new QLabel(widget);
		label->setText("已发送数据：");

		// 显示已发送数据
		QTextEdit* textEdit = new QTextEdit(widget);

		textEdit->setReadOnly(true);
		layout->addWidget(label);
		layout->addWidget(textEdit);
		connect(this, &TcpCommunication::showSendDataValue, [=](const QByteArray& data) {
			textEdit->append(QString(data));
			});
		layout->addWidget(label, 0, Qt::AlignLeft);
		layout->addWidget(textEdit, 1, Qt::AlignRight);

		// 清空操作
		QPushButton* clearButton = new QPushButton(widget);
		clearButton->setText("清空");
		layout->addWidget(clearButton);
		connect(clearButton, &QPushButton::clicked, [=]() {
			textEdit->clear();
			});

		return widget;
	}

	QWidget* TcpCommunication::showReceiveData(QWidget* parent)
	{
		// 显示已接收数据
		QWidget* widget = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(widget);
		QLabel* label = new QLabel(widget);
		label->setText("已接收数据：");

		// 显示已接收数据
		QTextEdit* textEdit = new QTextEdit(widget);
		textEdit->setReadOnly(true);
		layout->addWidget(label);
		layout->addWidget(textEdit);
		connect(this, &TcpCommunication::receiveSendData, [=](const QByteArray& data) {
			textEdit->append(QString(data));
			});
		layout->addWidget(label, 0, Qt::AlignLeft);
		layout->addWidget(textEdit, 1, Qt::AlignRight);

		// 清空操作
		QPushButton* clearButton = new QPushButton(widget);
		clearButton->setText("清空");
		layout->addWidget(clearButton);
		connect(clearButton, &QPushButton::clicked, [=]() {
			textEdit->clear();
			});

		return widget;
	}
	QWidget* TcpCommunication::showError(QWidget* parent)
	{
		// 显示错误信息
		QWidget* widget = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(widget);
		QLabel* label = new QLabel(widget);
		label->setText("错误信息：");

		// 显示错误信息
		QTextEdit* textEdit = new QTextEdit(widget);
		textEdit->setReadOnly(true);
		layout->addWidget(label);
		layout->addWidget(textEdit);
		connect(this, &TcpCommunication::sendError, [=](const QString& error) {
			textEdit->append(error);
			});
		layout->addWidget(label, 0, Qt::AlignLeft);
		layout->addWidget(textEdit, 1, Qt::AlignRight);

		// 清空操作
		QPushButton* clearButton = new QPushButton(widget);
		clearButton->setText("清空");
		layout->addWidget(clearButton);
		connect(clearButton, &QPushButton::clicked, [=]() {
			textEdit->clear();
			});

		return widget;
	}
	QWidget* TcpCommunication::showReconnect(QWidget* parent)
	{
		// 显示重连信息
		QWidget* widget = new QWidget(parent);
		QHBoxLayout* layout = new QHBoxLayout(widget);
		QLabel* label = new QLabel(widget);
		label->setText("重连状态：");
		layout->addWidget(label);
		connect(this, &TcpCommunication::reConnectStatusChanged, [=](bool status) {
			QString statusText = status ? "已重连" : "重连失败";
			label->setText(QString("重连状态：" + statusText));
			status ? label->setStyleSheet("color: green") : label->setStyleSheet("color: red");
			});
		// 显示重连次数
		QLabel* countLabel = new QLabel(widget);
		countLabel->setText(QString("重连次数：" + QString::number(m_reconnectCount)));
		layout->addWidget(countLabel);

		// 设置重连次数
		QLabel* reconnectCountLabel = new QLabel(widget);
		reconnectCountLabel->setText("重连次数：");
		layout->addWidget(reconnectCountLabel);
		QSpinBox* reconnectCountSpinBox = new QSpinBox(widget);
		reconnectCountSpinBox->setMinimum(0);
		reconnectCountSpinBox->setMaximum(INT_MAX);
		reconnectCountSpinBox->setValue(m_reconnectCount);
		layout->addWidget(reconnectCountSpinBox);
		connect(reconnectCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
			setReconnectCount(value);
			countLabel->setText(QString("重连次数：" + QString::number(m_reconnectCount)));
			});

		// 显示重连按钮
		QPushButton* reconnectButton = new QPushButton(widget);
		reconnectButton->setText("重连");
		layout->addWidget(reconnectButton);
		connect(reconnectButton, &QPushButton::clicked, [=]() {
			startReconnect();
			});

		

		return widget;
	}
	void TcpCommunication::startReconnect()
	{
		// 停止重连定时器
		m_reconnectTimer->stop();
		// 重新连接
		connectToServer(m_serverIP, m_serverPort);

		// 处理重连逻辑的代码块
		if (m_reconnectCount > 0)
		{
			// 重连次数大于0，继续重连
			m_reconnectCount = m_reconnectCount - 1;
			m_reconnectTimer->start();
			emit reConnectStatusChanged(true);
		}
		else
		{
			// 重连次数为0，停止重连
			m_reconnectTimer->stop();
			emit reConnectStatusChanged(false);
		}

	}
	void TcpCommunication::handleError(QAbstractSocket::SocketError socketError)
	{
		emit sendError(errorString()); // 发出错误信号
	}
}

