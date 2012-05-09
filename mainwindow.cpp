#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent):QMainWindow(parent)
{
    QAction*	gray_button = new QAction(tr("gray"),this);
    QAction*	bin_button = new QAction(tr("binary"),this);
    QAction*	open = new QAction(tr("open"),this);
    QAction*	action_identify = new QAction(tr("identity"),this);
    QAction*	hist_button = new QAction(tr("histogram"),this);
    QAction*	filter_button = new QAction(tr("median filter"),this);
    QAction*	otsu_button = new QAction(tr("Otsu"),this);
    QAction*	sum_button = new QAction(tr("charbar"),this);

    QMenu*	file = menuBar()->addMenu(tr("file"));
    file->addAction(open); 
    QMenu*	pro = menuBar()->addMenu(tr("Process"));
    pro->addAction(gray_button);
    pro->addAction(filter_button);
    pro->addAction(hist_button);
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

    this->lineEdit = new QLineEdit;
    this->piclabel = new QLabel("display picture");
    this->binlabel = new QLabel("a histogram of the image");
    this->myimage  = NULL;
    this->pic_hist = NULL;
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

    for(j = 0; j < myimage->height();j++)
    {
	memcpy(photo->data,myimage->scanLine(j),myimage->width() * (myimage->depth() >> 3));
	photo->data += (myimage->width() * (myimage->depth() >> 3));
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
	if(this->myimage)
    	{
    	    delete this->myimage;
	    this->myimage = NULL;
    	}
	
	/*
	 * 对打开后面的图片时要释放掉前面图片申请的空间
	 * */
    	if(this->rgb24)
    	{
    	    free (this->rgb24);
	    this->rgb24 = NULL;
    	}

    	myimage = new QImage(this->path);
    	assert(myimage);

    	this->piclabel->setPixmap(QPixmap::fromImage(*myimage));

    	if(photo->data)
    	{
    	    free(photo->data);
	    photo->data = NULL;
    	}

    	this->photo->width = myimage->width();
    	this->photo->height = myimage->height();
    	this->photo->data = (UINT8*)malloc((myimage->depth() >> 3) * myimage->width() * myimage->height());
    	this->photo->bpp = myimage->depth();
    	
    	this->rgb24 = (UINT8*)malloc(photo->width * photo->height * 3);

    	assert(this->rgb24);
    	assert(photo->data);

    	capturingdata();

    	printf("width is %d,heigth is %d,depth is %d\n",myimage->width(),myimage->height(),myimage->depth());
    	update();
    }
}


void mainwindow::charbar()
{
    Graying();
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
   binlabel->setPixmap(QPixmap::fromImage(*myimage));
   update();
}

void mainwindow::filtering()
{
    filter(this->photo);
    rgb8torgb32();
    binlabel->setPixmap(QPixmap::fromImage(*myimage));
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
    binlabel->setPixmap(QPixmap::fromImage(*myimage));
    update();
}

void mainwindow::otsuThreadmethod()
{
    this->threshold = get_histogram_value2(this->hist);
    //this->threshold = otsu(this->hist);
    this->threshold = 80;
    printf("阈值：%d\n",this->threshold);
}

void mainwindow::Identify()
{
    char* restr = (char*)Iden_charbar(this->photo);
    codec = QString(restr);
    
    lineEdit->setText(codec);
    lineEdit->setReadOnly(1);
}
#if 0
void mainwindow::paintEvent(QPaintEvent *event)
{
    QPainter *paint = new QPainter(this);
    if(this->myimage != NULL)
    {
	paint->drawImage(QPoint(50,50),*(this->myimage));	
    }
}
#endif
mainwindow::~mainwindow()
{
    free(photo->data);
    free(photo);
    free(this->rgb24);
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

    if(this->myimage)
    {
	delete this->myimage;
	this->myimage = NULL;
    }

    this->myimage = new QImage(this->rgb24,photo->width,photo->height,3*photo->width,QImage::Format_RGB888);
    assert(this->myimage);
}


