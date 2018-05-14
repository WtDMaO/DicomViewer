#include "dcmenhance.h"
#include "ui_dcmenhance.h"
#include "libMyAdd.h"

#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>



#include <opencv2/highgui/highgui.hpp>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <dcmtk\dcmjpeg\djdecode.h>
#include <dcmtk\dcmimgle\dcmimage.h>

using namespace std;
//工具函数实现

//int转换为字符数组
const char* itoa2(int val)
{
    static char result[(sizeof(int)<<3)+2];
    sprintf(result,"%d",val);
    return result;
}

//cvMat转换为mwArray
mwArray Mat2mwArray(cv::Mat src)
{
    cv::Mat tempMat=src.clone();
    mwSize dims[2]={src.cols,src.rows};
    mwArray pMat(2,dims,mxUINT16_CLASS);

    UINT16* x=(UINT16*)tempMat.data;
    pMat.SetData(x,src.rows*src.cols);
    return pMat;
}

//mwArray转换为cvMat
cv::Mat mwArry2Mat(mwArray src)
{
    mwArray dims=src.GetDimensions();



    int M=dims(1);
    int N=dims(2);
    cv::Mat image(M,N,CV_16UC1);

    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
            int index=src(i+1,j+1);
            //if(index>255) index=255;
            image.at<Uint16>(j,i)=index;
        }
    }
    return image;
}

//cvMat保存为txt
void cvmat2Txt(cv::Mat m,string fileName)
{
    ofstream fout(fileName,ios::binary);


    for(int i=0;i<m.cols;i++){
        for(int j=0;j<m.rows;j++){
            fout<<(int)m.at<Uint16>(i,j)<<" ";
          //fout<<m.at<Uint16>(i,j)<<" ";
        }
        fout<<endl;
    }

}

//mwArray保存为txt
void mwarray2Txt(mwArray mw,string fileName)
{
    ofstream fout(fileName);
    for(int i=0;i<512;++i)
    {
        for(int j=0;j<512;++j)
        {
            fout<<mw(i+1,j+1)<<" ";
        }
        fout<<endl;
    }

    fout.close();
}

cv::Mat Mat16toMat8(cv::Mat Mat16)
{
    cv::Mat Mat8(Mat16.rows,Mat16.cols,CV_8UC1);
    for(int i=0;i<Mat16.rows;++i)
    {
        for(int j=0;j<Mat16.cols;++j)
        {
            Mat8.at<Uint8>(i,j)=Mat16.at<Uint16>(i,j)/256;
        }
    }
    return Mat8;
}

//主体功能实现 *******************************************************************


DcmEnhance::DcmEnhance(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::DcmEnhance)
{
    ui->setupUi(this);
}

DcmEnhance::~DcmEnhance()
{
    delete ui;
    remove("1");
    remove("2");

}

//将dicom文件转换成cvmat,对dicommat1和dicommat2进行赋值
void DcmEnhance::dicom2Cvmat(QString fileName,int tempFileName)
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

        Uint16 *pixel=(Uint16*)(dicomFile->getOutputData(16));

        cv::Mat dst(dicomFile->getWidth(),dicomFile->getHeight(),CV_16U,pixel);


        if(tempFileName==TEMPDICOM1) dst.copyTo(dicomMat1);
        if(tempFileName==TEMPDICOM2) dst.copyTo(dicomMat2);
    }
}


//显示图像

void DcmEnhance::dicomImgShow(int tempDicom)
{
    cv::Mat dst;
    if(tempDicom==TEMPDICOM1) dst=Mat16toMat8(dicomMat1).clone();
    if(tempDicom==TEMPDICOM2) dst=Mat16toMat8(dicomMat2).clone();


    QImage img=QImage((Uint8*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

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
// 带有缩放等功能的现实函数，目前尚未实现
/*
void  DcmEnhance::dicomImgShow(int tempDicom,int dx,int dy,int scale)
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
*/


//刷新显示图像

void DcmEnhance::dicomWindowAdj(int tempDicom)
{
      DcmFileFormat tempDicomFile;
      if(tempDicomFile.loadFile(to_string(tempDicom).c_str()).good()){
          DicomImage *tempdimage=new DicomImage(to_string(tempDicom).c_str());
          tempdimage->setWindow(winCenter,winWidth);

          Uint16 *pixel=(Uint16*)(tempdimage->getOutputData(16));

          cv::Mat dst(tempdimage->getWidth(),tempdimage->getHeight(),CV_16U,pixel);
          if(tempDicom==TEMPDICOM1) dst.copyTo(dicomMat1);
          if(tempDicom==TEMPDICOM2) dst.copyTo(dicomMat2);
      }
}


//打开文件1
void DcmEnhance::on_FileOpenButton_clicked()
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
void DcmEnhance::on_FileOpenButton2_clicked()
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
void DcmEnhance::on_setLungWin_clicked()
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
void DcmEnhance::on_setMedisatWin_clicked()
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



void DcmEnhance::on_pushButton_clicked()
{
    if(libMyAddInitialize()){
        mwArray dicomArray2;
        c_matlab(1,dicomArray2,Mat2mwArray(dicomMat1));
        mwArry2Mat(dicomArray2).copyTo(dicomMat2);
        dicomImgShow(TEMPDICOM2);
        //cvmat2Txt(dicomMat2,"outputmat");
        //mwarray2Txt(dicomArray2,"mwoutput");
        libMyAddTerminate();
    }
}
