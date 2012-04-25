#ifndef MW_H
#define MW_H

#include <QtGui>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "charbar.h"
#include "header.h"
#include "pretreat.h"

class mainwindow:public QMainWindow
{
    Q_OBJECT
    public:
	mainwindow(QWidget *parent = 0);	
	~mainwindow();

    private:
	QImage *myimage;
	QString path;
	picture_T* pic_data;
	UINT8 * intervalue1;
	UINT8 * intervalue2;
	QString codec;
	QLineEdit *lineEdit;
	QLabel * label;

	void paintEvent(QPaintEvent *event);
	void capturingdata();
	void rgb8torgb32();
    private slots:
	void Graying();
	void Identify();
	void binary_pic();
	void openfile();
};
#endif
