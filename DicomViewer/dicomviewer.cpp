#include "dicomviewer.h"
#include "ui_dicomviewer.h"
#include "libMyAdd.h"

#include <QFileDialog>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>



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

}

DicomViewer::~DicomViewer()
{
    delete ui;
    remove("1");
    remove("2");
}

//将dicom文件转换成cvmat,对dicommat1和dicommat2进行赋值
void DicomViewer::dicom2Cvmat(QString fileName,int tempFileName)
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

        if(tempFileName==TEMPDICOM1) dst.copyTo(dicomMat1);
        if(tempFileName==TEMPDICOM2) dst.copyTo(dicomMat2);
    }
}


//显示图像

void DicomViewer::dicomImgShow(int tempDicom)
{
    cv::Mat dst;
    if(tempDicom==TEMPDICOM1) dicomMat1.copyTo(dst);
    if(tempDicom==TEMPDICOM2) dicomMat2.copyTo(dst);

    QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

    QGraphicsScene *scene=new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(img));

    if(tempDicom==TEMPDICOM1)
    {
        ui->graphicsView1->resetTransform();
        ui->graphicsView1->setScene(scene);
        ui->graphicsView1->resize(img.width()+10,img.height()+10);
        ui->graphicsView1->show();
    }
    else if(tempDicom==TEMPDICOM2)
    {
        ui->graphicsView2->resetTransform();
        ui->graphicsView2->setScene(scene);
        ui->graphicsView2->resize(img.width()+10,img.height()+10);
        ui->graphicsView2->show();
    }
}

void  DicomViewer::dicomImgShow(int tempDicom,int dx,int dy,int scale)
{
    cv::Mat dst;
    if(tempDicom==TEMPDICOM1) dicomMat2.copyTo(dst);
    if(tempDicom==TEMPDICOM2) dicomMat2.copyTo(dst);

    QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);


    QGraphicsScene *scene=new QGraphicsScene;


    scene->addPixmap(QPixmap::fromImage(img));

    if(tempDicom==TEMPDICOM1)
    {
        ui->graphicsView1->setScene(scene);
        ui->graphicsView1->resize(img.width()+10,img.height()+10);
        ui->graphicsView1->centerOn(QPointF(dx,dy));
        ui->graphicsView1->scale(scale,scale);
        ui->graphicsView1->show();
    }
    else if(tempDicom==TEMPDICOM2)
    {
        ui->graphicsView2->resetTransform();
        ui->graphicsView2->setScene(scene);
        ui->graphicsView2->resize(img.width()+10,img.height()+10);
        ui->graphicsView1->centerOn(QPointF(dx,dy));
        ui->graphicsView1->scale(scale,scale);
        ui->graphicsView2->show();
    }
}



//刷新显示图像

void DicomViewer::dicomWindowAdj(int tempDicom)
{
      DcmFileFormat tempDicomFile;
      if(tempDicomFile.loadFile(to_string(tempDicom).c_str()).good()){
          DicomImage *tempdimage=new DicomImage(to_string(tempDicom).c_str());
          tempdimage->setWindow(winCenter,winWidth);

          Uint8 *pixel=(Uint8*)(tempdimage->getOutputData(8));

          cv::Mat dst(tempdimage->getWidth(),tempdimage->getHeight(),CV_8U,pixel);
          if(tempDicom==TEMPDICOM1) dst.copyTo(dicomMat1);
          if(tempDicom==TEMPDICOM2) dst.copyTo(dicomMat2);
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
    ui->graphicsView1->resetTransform();
    dicom2Cvmat(fileName,TEMPDICOM1);
    dicomImgShow(TEMPDICOM1);
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
    ui->graphicsView2->resetTransform();
    dicom2Cvmat(fileName,TEMPDICOM2);
    dicomImgShow(TEMPDICOM2);
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
    dicomWindowAdj(TEMPDICOM1);
    dicomWindowAdj(TEMPDICOM2);
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
    dicomWindowAdj(TEMPDICOM1);
    dicomWindowAdj(TEMPDICOM2);
    dicomImgShow(TEMPDICOM1);
    dicomImgShow(TEMPDICOM2);
}


mwArray Mat2mwArray(cv::Mat src)
{
    cv::Mat tempMat=src.clone();
    mwSize dims[2]={src.cols,src.rows};
    mwArray pMat(2,dims,mxUINT8_CLASS);

    UINT8* x=(UINT8*)tempMat.data;
    pMat.SetData(x,src.rows*src.cols);
    return pMat;
}

cv::Mat mwArry2Mat(mwArray src)
{
    mwArray dims=src.GetDimensions();



    int M=dims(1);
    int N=dims(2);
    cv::Mat image(M,N,CV_8UC1);

    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
            int index=src(i+1,j+1);
            image.row(j).col(i).data[0]=index;
        }
    }
    return image;
}

void DicomViewer::on_pushButton_clicked()
{
    if(libMyAddInitialize()){
        mwArray dicomArray2;
        c_matlab(1,dicomArray2,Mat2mwArray(dicomMat1));
        mwArry2Mat(dicomArray2).copyTo(dicomMat2);
        dicomImgShow(TEMPDICOM2);
        libMyAddTerminate();
    }
}
