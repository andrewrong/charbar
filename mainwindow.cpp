#include "mainwindow.h"
#include <assert.h>
#include <stdio.h>

mainwindow::mainwindow(QWidget *parent):QMainWindow(parent)
{
    QAction *equaliation = new QAction(tr("gray"),this);
    QAction *binary = new QAction(tr("binary"),this);
    QAction *open = new QAction(tr("open"),this);
    QAction *action_identify = new QAction(tr("identity"),this);
    lineEdit = new QLineEdit;
    label = new QLabel;

    QMenu *file = menuBar()->addMenu(tr("file"));
    file->addAction(open); 
    QMenu *pro = menuBar()->addMenu(tr("Process"));
    pro->addAction(equaliation);
    pro->addAction(binary);
    pro->addAction(action_identify);

    connect(equaliation,SIGNAL(triggered()),this,SLOT(Graying()));
    connect(open,SIGNAL(triggered()),this,SLOT(openfile()));
    connect(action_identify,SIGNAL(triggered()),this,SLOT(Identify()));
    connect(binary,SIGNAL(triggered()),this,SLOT(binary_pic()));

    myimage = NULL;
    intervalue1 = NULL;
    intervalue2 = NULL;
    pic_data = (picture_T*)malloc(sizeof(picture_T));
    assert(pic_data);
    pic_data->width = 0;
    pic_data->height = 0;
    pic_data->bpp = 0;
    pic_data->data = NULL;
    QVBoxLayout *h = new QVBoxLayout;
    h->addWidget(label);
    h->addWidget(lineEdit);

    setLayout(h);
    QWidget * widget = new QWidget;
    widget->setLayout(h);
    setCentralWidget(widget);

    setWindowTitle(tr("charbar"));

    path = "";
    codec = "";
    this->resize(1024,800);
}

/*
 * 这里pic_data->data在加的过程中将这个值也变化了,这样就导致了后面的图片的严重偏移
 * */
void mainwindow::capturingdata()
{
    int j = 0;
    UINT8* tmp1 = pic_data->data; 
    for(j = 0; j < myimage->height();j++)
    {
	memcpy(pic_data->data,myimage->scanLine(j),myimage->width() * (myimage->depth() >> 3));
	pic_data->data += (myimage->width() * (myimage->depth() >> 3));
    }
    pic_data->data = tmp1;
}


void mainwindow::openfile()
{

    this->path = QFileDialog::getOpenFileName(this,tr("open Image"),".",tr("Image file(*.bmp *.jpg *.png *.gif)"));
    
    if(path.length() == 0)
    {
	QMessageBox::warning(NULL,tr("warning"),tr("this path is empty"),QMessageBox::Yes);
    }
    
    if(this->myimage)
    {
	delete this->myimage;
    }

    if(this->intervalue1)
    {
	free(this->intervalue1);
    }
    
    if(this->intervalue2)
    {
	free (this->intervalue2);
    }

    myimage = new QImage(this->path);
    assert(myimage);
    label->setPixmap(QPixmap::fromImage(*myimage));
    pic_data->width = myimage->width();
    pic_data->height = myimage->height();
    if(pic_data->data)
    {
	free(pic_data->data);
    }

    pic_data->data = (unsigned char*)malloc((myimage->depth() >> 3) * myimage->width() * myimage->height());
    pic_data->bpp = (myimage->depth()) >> 3;
    
    this->intervalue2 = (UINT8*)malloc(pic_data->width * pic_data->height * 3);

    assert(this->intervalue2);
    assert(pic_data->data);

    capturingdata();

    printf("width is %d,heigth is %d,depth is %d\n",myimage->width(),myimage->height(),myimage->depth());

    if(myimage == NULL)
    {
	QMessageBox::warning(NULL,tr("error"),tr("create image is failure,this error is %1").arg(strerror(errno)));
	exit(-1);
    }
    update();
}

void mainwindow::Graying()
{
   this->intervalue1 = graying(pic_data); 
   rgb8torgb32();
   update();
}

void mainwindow::binary_pic()
{
    binarization(this->intervalue1,pic_data->width,pic_data->height);
    rgb8torgb32();
    update();
}

void mainwindow::Identify()
{
    char* ch = "barcode is invaild";
    char* restr = (char*)Identify_charbar(this->intervalue1,pic_data->width,pic_data->height);
    if(restr == NULL)
    {
	codec = QString(ch);
    }
    else
    {
	codec = QString(restr);
    }
    
    lineEdit->setText(codec);
    lineEdit->setReadOnly(1);
}

void mainwindow::paintEvent(QPaintEvent *event)
{
    QPainter *paint = new QPainter(this);
    if(this->myimage != NULL)
    {
	paint->drawImage(QPoint(50,50),*(this->myimage));	
    }
}

mainwindow::~mainwindow()
{
    free(pic_data->data);
    free(pic_data);
    free(this->intervalue2);
    free(this->intervalue1);
}

void mainwindow::rgb8torgb32()
{
    int i = 0;
    int j = 0;

    assert(this->intervalue2);

    for(i = 0; i < pic_data->height; i++)
    {
	for(j = 0;j < pic_data->width;j++)
	{
	    UINT8 a = this->intervalue1[i*pic_data->width+j];
	    this->intervalue2[i*pic_data->width*3+j*3] = a;
	    this->intervalue2[i*pic_data->width*3+j*3 + 1] = a;
	    this->intervalue2[i*pic_data->width*3+j*3 + 2] = a;
	}
    }

    if(this->myimage)
    {
	delete this->myimage;
    }

    this->myimage = new QImage(this->intervalue2,pic_data->width,pic_data->height,3*pic_data->width,QImage::Format_RGB888);
    assert(this->myimage);
}
