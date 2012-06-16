#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent):QMainWindow(parent)
{
    QAction*	gray_button = new QAction(tr("gray"),this);
    QAction*	bin_button = new QAction(tr("binary"),this);
    QAction*	open = new QAction(tr("open"),this);
    QAction*	action_identify = new QAction(tr("identity"),this);
    QAction*	filter_button = new QAction(tr("median filter"),this);
    QAction*	shine = new QAction(tr("delshine"),this);
    QAction*	hist_button = new QAction(tr("histogram"),this);
    QAction*	otsu_button = new QAction(tr("Otsu"),this);
    QAction*	sum_button = new QAction(tr("charbar"),this);
    QAction*	zoom_button = new QAction(tr("zoom"),this);

    QMenu*	file = menuBar()->addMenu(tr("file"));
    file->addAction(open); 
    QMenu*	pro = menuBar()->addMenu(tr("Process"));
    pro->addAction(gray_button);
    pro->addAction(zoom_button);
    pro->addAction(hist_button);
    pro->addAction(shine);
    pro->addAction(filter_button);
    pro->addAction(otsu_button);
    pro->addAction(bin_button);
    pro->addAction(action_identify);
    pro->addAction(sum_button);

    connect(gray_button,SIGNAL(triggered()),this,SLOT(Graying()));
    connect(filter_button,SIGNAL(triggered()),this,SLOT(filtering()));
    connect(hist_button,SIGNAL(triggered()),this,SLOT(get_histogram()));
    connect(open,SIGNAL(triggered()),this,SLOT(openfile()));
    connect(action_identify,SIGNAL(triggered()),this,SLOT(Identify()));
    connect(bin_button,SIGNAL(triggered()),this,SLOT(binary_pic()));
    connect(sum_button,SIGNAL(triggered()),this,SLOT(charbar()));
    connect(otsu_button,SIGNAL(triggered()),this,SLOT(otsuThreadmethod()));
    connect(shine,SIGNAL(triggered()),this,SLOT(mdelshine()));
    connect(zoom_button,SIGNAL(triggered()),this,SLOT(mbilinear()));

    this->lineEdit = new QLineEdit;
    this->piclabel = new QLabel("display picture");
    this->binlabel = new QLabel("a histogram of the image");
    this->image1  = NULL;
    this->image2  = NULL;
    this->rgb24	   = NULL;
    this->hist	   = NULL;
    this->threshold = 0;

    this-> photo = (picture_T*)malloc(sizeof(picture_T));
    assert(photo);
    photo->width = 0;
    photo->height = 0;
    photo->bpp = 0;
    photo->data = NULL;

    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;
    h->addWidget(piclabel);
    h->addWidget(binlabel);
    v->addLayout(h);
    v->addWidget(lineEdit);

    setLayout(v);
    QWidget * widget = new QWidget;
    widget->setLayout(v);
    setCentralWidget(widget);

    setWindowTitle(tr("charbar"));

    this->path = "";
    this->codec = "";
    this->resize(1024,800);
}

/*
 * 这里photo->data在加的过程中将这个值也变化了,这样就导致了后面的图片的严重偏移
 * */
void mainwindow::capturingdata()
{
    int j = 0;
    UINT8* tmp1 = photo->data; 

    for(j = 0; j <image1->height();j++)
    {
	memcpy(photo->data,image1->scanLine(j),image1->width() * (image1->depth() >> 3));
	photo->data += (image1->width() * (image1->depth() >> 3));
    }
    photo->data = tmp1;
}


void mainwindow::openfile()
{

    this->path = QFileDialog::getOpenFileName(this,tr("open Image"),".",tr("Image file(*.bmp *.jpg *.png *.gif)"));
    
    if(path.length() == 0)
    {
	QMessageBox::warning(NULL,tr("warning"),tr("this path is empty"),QMessageBox::Yes);
    }
    else
    {
	if(this->image1)
    	{
    	    delete this->image1;
	    this->image1 = NULL;
    	}
	
	/*
	 * 对打开后面的图片时要释放掉前面图片申请的空间
	 * */
	if(this->rgb24)
    	{
    	    free (this->rgb24);
	    this->rgb24 = NULL;
    	}

    	image1 = new QImage(this->path);
    	assert(image1);

    	this->piclabel->setPixmap(QPixmap::fromImage(*image1));

    	if(photo->data)
    	{
    	    free(photo->data);
	    photo->data = NULL;
    	}

    	this->photo->width = image1->width();
    	this->photo->height = image1->height();
    	this->photo->data = (UINT8*)malloc((image1->depth() >> 3) * image1->width() * image1->height());
    	this->photo->bpp = image1->depth();
    	
    	this->rgb24 = (UINT8*)malloc(photo->width * photo->height * 3);

    	assert(this->rgb24);
    	assert(photo->data);

    	capturingdata();

    	printf("width is %d,heigth is %d,depth is %d\n",image1->width(),image1->height(),image1->depth());
    	update();
    }
}


void mainwindow::charbar()
{
    Graying();
    mbilinear(); 
    filtering();
    get_histogram();
    otsuThreadmethod();
    binary_pic();

    Identify();
}
void mainwindow::Graying()
{
   graying(this->photo);
   rgb8torgb32();
   binlabel->setPixmap(QPixmap::fromImage(*image2));
   update();
}

void mainwindow::mbilinear()
{
    bilinear(photo);
    this->old_rgb24 = this->rgb24;
    printf("%d,%d\n",photo->width,photo->height);
    this->rgb24 = (UINT8*)calloc(photo->width*photo->height*3,sizeof(UINT8));
    rgb8torgb32();
    binlabel->setPixmap(QPixmap::fromImage(*image2));
    update();
}


void mainwindow::filtering()
{
    filter(this->photo);
    rgb8torgb32();
    binlabel->setPixmap(QPixmap::fromImage(*image2));
    update();
}

void mainwindow::get_histogram()
{
    //直方图
    this->hist = (histogram*)malloc(sizeof(histogram));
    histogram_func(this->photo,hist);
}

void mainwindow::binary_pic()
{
    binarization(this->photo,this->threshold);
    rgb8torgb32();
    binlabel->setPixmap(QPixmap::fromImage(*image2));
    update();
}

void mainwindow::otsuThreadmethod()
{
    //this->threshold = get_histogram_value2(this->hist);
    this->threshold = otsu(this->hist);
    printf("阈值：%d\n",this->threshold);
}

void mainwindow::Identify()
{
    char* restr = (char*)Iden_charbar(this->photo);
//    char* restr = (char*)sobel(this->photo);
    codec = QString(restr);
    
    lineEdit->setText(codec);
    lineEdit->setReadOnly(1);
    //rgb8torgb32();
    //binlabel->setPixmap(QPixmap::fromImage(*image2));
    update();

}

mainwindow::~mainwindow()
{
    free(photo->data);
    free(photo);
    free(this->rgb24);
    //free(r_gray_data);
    //free(p_gray_data);
    free(old_rgb24);
    free(hist);
}

void mainwindow::rgb8torgb32()
{
    int i = 0;
    int j = 0;

    assert(this->rgb24);

    for(i = 0; i < photo->height; i++)
    { 
	for(j = 0;j < photo->width;j++) 
	{ 
	    UINT8 a = this->photo->data[i*photo->width+j]; 
	    this->rgb24[i*photo->width*3+j*3] = a; 
	    this->rgb24[i*photo->width*3+j*3 + 1] = a;
	    this->rgb24[i*photo->width*3+j*3 + 2] = a;
	}
    }

    if(this->image2)
    {
	delete this->image2;
	this->image2 = NULL;
    }

    this->image2 = new QImage(this->rgb24,photo->width,photo->height,3*photo->width,QImage::Format_RGB888);
    assert(this->image2);
}

void mainwindow::mdelshine()
{
	delshine2(this->photo,this->hist);
	rgb8torgb32();
	binlabel->setPixmap(QPixmap::fromImage(*image2));
	update();
}
