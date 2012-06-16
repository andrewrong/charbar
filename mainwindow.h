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
	QImage*	    image1;//pic1
	QImage*	    image2;//pic2
	QString	    path;
	picture_T*  photo;
	UINT8*	    rgb24;
	//UINT8*	    r_gray_data;
	//UINT8*	    p_gray_data;
	//关于这个old_rgb24在最后的完善过程会好好的进行修改
	UINT8*	    old_rgb24;
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
	void mdelshine();
	void mbilinear();
};
#endif
