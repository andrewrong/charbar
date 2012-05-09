#ifndef MW_H
#define MW_H

#include <QtGui>
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
	QImage*	    myimage;
	QImage*	    pic_hist;
	QString	    path;
	picture_T*  photo;
	UINT8*	    rgb24;
	QString	    codec;
	QLineEdit*  lineEdit;
	QLabel*	    piclabel;
	QLabel*	    binlabel;
	histogram*  hist;
	int	    threshold;

//	void paintEvent(QPaintEvent *event);
	void capturingdata();
	void rgb8torgb32();
    private slots:
	void filtering(); 
	void Graying();
	void Identify();
	void binary_pic();
	void openfile();
	void get_histogram();
	void otsuThreadmethod();
	void charbar();
};
#endif
