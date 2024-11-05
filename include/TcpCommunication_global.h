#pragma once

#ifndef COMMUNICATIONMODULE_GLOBAL_H
#define COMMUNICATIONMODULE_GLOBAL_H



#include <QtCore/qglobal.h>
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
#include <QSpinBox>

#if defined(COMMUNICATIONMODULE_LIBRARY)
#define COMMUNICATIONMODULE_EXPORT   Q_DECL_EXPORT //extern "C"
#else
#define COMMUNICATIONMODULE_EXPORT    Q_DECL_IMPORT // extern "C"
#endif

//#define COMMUNICATIONMODULE_EXPORT   Q_DECL_EXPORT //extern "C"
#define __TcpString(str) QString::fromLocal8Bit(str)

#endif // !