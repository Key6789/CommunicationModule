#pragma once

#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H


#include "TcpCommunication_global.h"
#include <QTcpSocket>
#include <QTimer>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QMutex>

// 导出宏，用于声明接口 如果在类外定义接口，则需要在前面加上COMMUNICATIONMODULE_EXPORT
#define TCPCOMMUNICATION_NAME			"CommunicationModule::TcpCommunication"
#define TCPCOMMUNICATION_TYPE			CommunicationModule::TcpCommunication

/// @brief 通信模块的Tcp通信类
/// @details 继承自QTcpSocket类，实现Tcp通信功能
/// @note 该类是通信模块的核心类，负责实现Tcp通信功能
/// @details 该类提供了连接服务器、发送数据、接收数据、关闭连接、重连机制
/// @author hmy
/// @date 2024/9/12
namespace CommunicationModule
{
	class COMMUNICATIONMODULE_EXPORT TcpCommunication : public QTcpSocket
	{
		Q_OBJECT
	public:
		TcpCommunication(const QString& ip, quint16 port, QWidget* parent = nullptr);

		//@brief 连接服务器
		//@param ip 服务器IP地址
		//@param port 服务器端口号
		virtual bool connectToServer(const QString& ip, quint16 port);

		//@brief 发送数据
		//@param data 待发送数据
		virtual void sendCommand(const QByteArray& data);

		//@brief 接收数据
		//@return 接收到的数据
		virtual void receiveCommand(QByteArray& data);

		//@brief 关闭连接
		virtual void closeConnection();

		//@brief 是否连接成功
		//@return 连接成功返回true，否则返回false
		bool isConnected() const;

		//@brief 重连时间
		//@param time 重连时间，单位为毫秒
		void setReconnectTime(int time);

		//@brief 等待时间
		//@param time 等待时间，单位为毫秒
		void setWaitTime(int time);

		//@brief 重连次数
		//@param count 重连次数
		void setReconnectCount(int count);

		void setConnectIndex(const int& index);
		int getConnectIndex();

		/********************* 界面显示部分*************************************/
	public:
		// 显示连接状态的窗口
		QWidget* showConnectStatus(QWidget* parent);

		// 显示发送数据的窗口
		QWidget* showSendData(QWidget* parent);

		// 显示接收数据的窗口
		QWidget* showReceiveData(QWidget* parent);

		// 显示错误信息的窗口
		QWidget* showError(QWidget* parent);

		// 显示重连的窗口
		QWidget* showReconnect(QWidget* parent);

		// 显示等待的窗口
		QWidget* showSetSendCommand(QWidget* parent);



	Q_SIGNALS:
		//@brief 连接成功信号
		void connectStatusChanged(bool isConnected);
		//@brief 接收到数据信号
		void receiveSendData(QByteArray data);

		void showSendDataValue(QByteArray data);
		//@brief 发送数据完成信号
		void sendDataFinished(bool isSuccess);

		//@brief 错误信号
		void sendError(QString errorMsg);

		//@brief 重连信号
		void reConnectStatusChanged(bool isReconnect);

	protected slots:
		//@brief 错误信息槽函数
		virtual void handleError(QAbstractSocket::SocketError socketError);

		//@brief 开启重连机制
		virtual void startReconnect();




	private:
		//@brief 服务器IP地址
		QString m_serverIP = __TcpString("127.0.0.1");

		//@brief 服务器端口号
		quint16 m_serverPort = 8080;

		//@brief 重连定时器
		QTimer* m_reconnectTimer = nullptr;

		//@brief 重连时间 ms
		int m_reconnectTime = 1000;

		//@brief 等待时间 ms
		int m_waitTime = 20;

		//@brief 重连次数
		int m_reconnectCount = INT_MAX;

		int m_connectIndex = 0;

	};
}
#endif