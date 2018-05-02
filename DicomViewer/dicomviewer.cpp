#include "dicomviewer.h"
#include "ui_dicomviewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <QFile>

#include <opencv2/imgproc/imgproc.hpp>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <dcmtk\dcmjpeg\djdecode.h>
#include <dcmtk\dcmimgle\dcmimage.h>

using namespace std;

const char* itoa2(int val)
{
    static char result[(sizeof(int)<<3)+2];
    sprintf(result,"%d",val);
    return result;
}


DicomViewer::DicomViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DicomViewer)
{
    ui->setupUi(this);
    setMouseTracking(true); //全局监控鼠标事件
}

DicomViewer::~DicomViewer()
{
    delete ui;
    remove("1");
    remove("2");
}

//将dicom文件转换成cvmat,对dicommat1和dicommat2进行赋值
cv::Mat DicomViewer::dicom2Cvmat(QString fileName,int tempFileName)
{
    //读取文件名和保存解压后的临时文件名
    string fileNamestr=fileName.toStdString();

    //解压dicom文件
    DJDecoderRegistration::registerCodecs();

    DcmFileFormat Dicomfile;

    if(Dicomfile.loadFile(fileNamestr.c_str()).good())
    {
        DcmDataset *dataset=Dicomfile.getDataset();

        dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);

        if (dataset->canWriteXfer(EXS_LittleEndianExplicit))
        {
            Dicomfile.saveFile(to_string(tempFileName).c_str(), EXS_LittleEndianExplicit);
        }

        DJDecoderRegistration::cleanup();
        //提取解压后的dicom文件的图像信息，并进行处理。
        DicomImage *dicomFile=new DicomImage(to_string(tempFileName).c_str());

        dicomFile->setWindow(winCenter,winWidth);

        Uint8 *pixel=(Uint8*)(dicomFile->getOutputData(8));

        cv::Mat dst(dicomFile->getWidth(),dicomFile->getHeight(),CV_8U,pixel);

        if(tempFileName==TEMPDICOM1) dicomMat1==dst.clone();
        if(tempFileName==TEMPDICOM2) dicomMat2==dst.clone();

        return dst;
    }
}


//显示图像

void DicomViewer::dicomImgShow(QString fileName,int tempDicom)
{
    cv::Mat dst=dicom2Cvmat(fileName,tempDicom);

    QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

    //QImage img_scaled=img.scaled(ui->graphicsView1->size(),Qt::IgnoreAspectRatio);

    QGraphicsScene *scene=new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(img));

    if(tempDicom==TEMPDICOM1)
    {
        ui->graphicsView1->setScene(scene);
        ui->graphicsView1->resize(img.width()+10,img.height()+10);
        ui->graphicsView1->show();
    }
    else if(tempDicom==TEMPDICOM2)
    {
        ui->graphicsView2->setScene(scene);
        ui->graphicsView2->resize(img.width()+10,img.height()+10);
        ui->graphicsView2->show();
    }
    scene->destroyed();
}

void DicomViewer::dicomImgShow(int tempDicom)
{
    cv::Mat dst=dicomWindowAdj(tempDicom);

    QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

    //QImage img_scaled=img.scaled(ui->graphicsView1->size(),Qt::IgnoreAspectRatio);

    QGraphicsScene *scene=new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(img));

    if(tempDicom==TEMPDICOM1)
    {
        ui->graphicsView1->setScene(scene);
        ui->graphicsView1->resize(img.width()+10,img.height()+10);
        qDebug()<<img.width();
        ui->graphicsView1->show();
    }
    else if(tempDicom==TEMPDICOM2)
    {
        ui->graphicsView2->setScene(scene);
        ui->graphicsView2->resize(img.width()+10,img.height()+10);
        ui->graphicsView2->show();
    }
    scene->destroyed();
}



//刷新显示图像

cv::Mat DicomViewer::dicomWindowAdj(int tempDicom)
{
      DcmFileFormat tempDicomFile;
      if(tempDicomFile.loadFile(to_string(tempDicom).c_str()).good()){
          DicomImage *tempdimage=new DicomImage(to_string(tempDicom).c_str());
          tempdimage->setWindow(winCenter,winWidth);

          Uint8 *pixel=(Uint8*)(tempdimage->getOutputData(8));

          cv::Mat dst(tempdimage->getWidth(),tempdimage->getHeight(),CV_8U,pixel);
          return dst;
      }
}


//打开文件1
void DicomViewer::on_FileOpenButton_clicked()
{
    QString fileName;

    fileName=QFileDialog::getOpenFileName(this,"选择dicom图像","","Images (*.*)");

    if(fileName.isEmpty())
    {
        return;
    }

    dicomImgShow(fileName,TEMPDICOM1);
}

//打开文件2
void DicomViewer::on_FileOpenButton2_clicked()
{
    QString fileName;

    fileName=QFileDialog::getOpenFileName(this,"选择dicom图像","","Images (*.*)");

    if(fileName.isEmpty())
    {
        return;
    }

    dicomImgShow(fileName,TEMPDICOM2);
}

//设置肺窗
void DicomViewer::on_setLungWin_clicked()
{
    winWidth=1600;
    winCenter=-600;
    string curCenter="当前窗位:";
    curCenter=curCenter+to_string(winCenter);
    string curWin="当前窗宽:";
    curWin=curWin+to_string(winWidth);
    ui->cur_WinCen->setText(QString::fromStdString(curCenter));
    ui->cur_WinWidth->setText(QString::fromStdString(curWin));
    dicomImgShow(TEMPDICOM1);
    dicomImgShow(TEMPDICOM2);
}

//设置纵膈窗
void DicomViewer::on_setMedisatWin_clicked()
{
    winWidth=400;
    winCenter=40;
    string curCenter="当前窗位:";
    curCenter=curCenter+to_string(winCenter);
    string curWin="当前窗宽:";
    curWin=curWin+to_string(winWidth);
    ui->cur_WinCen->setText(QString::fromStdString(curCenter));
    ui->cur_WinWidth->setText(QString::fromStdString(curWin));
    dicomImgShow(TEMPDICOM1);
    dicomImgShow(TEMPDICOM2);
}

// 鼠标点击事件
void DicomViewer::mousePressEvent(QMouseEvent *e)
{
    if(Qt::LeftButton==e->button())
    {
        mouseX=e->x();
        mouseY=e->y();
    }
}

//鼠标移动事件
void DicomViewer::mouseMoveEvent(QMouseEvent *e)
{
    mouseX_m=e->x();
    mouseY_m=e->y();
}
