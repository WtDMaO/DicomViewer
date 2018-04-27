#include "dicomviewer.h"
#include "ui_dicomviewer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <dcmtk\dcmjpeg\djdecode.h>
#include <dcmtk\dcmimgle\dcmimage.h>

using namespace std;

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

//refresh the img show
void DicomViewer::dicomImgShow(QString fileName,int tempDicom){

    string fileNamestr = fileName.toStdString();

    DJDecoderRegistration::registerCodecs();
    DcmFileFormat Dicomfile;

    if(Dicomfile.loadFile(fileNamestr.c_str()).good()){
        DcmDataset *dataset=Dicomfile.getDataset();
        dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
        if (dataset->canWriteXfer(EXS_LittleEndianExplicit))
        {
            Dicomfile.saveFile(to_string(tempDicom).c_str(), EXS_LittleEndianExplicit);
        }
        DJDecoderRegistration::cleanup();

        DicomImage *dicomFile=new DicomImage(to_string(tempDicom).c_str());

        dicomFile->setWindow(winCenter,winWidth);

        Uint8 *pixel=(Uint8*)(dicomFile->getOutputData(8));

        cv::Mat dst(dicomFile->getWidth(),dicomFile->getHeight(),CV_8U,pixel);

        QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

        QImage img_scaled=img.scaled(img.size(),Qt::IgnoreAspectRatio);

        if(tempDicom==TEMPDICOM1) ui->DicomShow1->setPixmap(QPixmap::fromImage(img_scaled));
        else if(tempDicom==TEMPDICOM2) ui->DicomShow2->setPixmap(QPixmap::fromImage(img_scaled));
    }


}
const char* itoa2(int val)
{
    static char result[sizeof(int)<<3+2];
    sprintf(result,"%d",val);
    return result;
}

void DicomViewer::dicomTempImgShow(int tempDicom){
    DcmFileFormat dicomTempFile;
    if(dicomTempFile.loadFile(itoa2(tempDicom)).good()){
        DicomImage *dicomTempFile=new DicomImage(itoa2(tempDicom));

        dicomTempFile->setWindow(winCenter,winWidth);

        Uint8 *pixel=(Uint8*)(dicomTempFile->getOutputData(8));

        cv::Mat dst(dicomTempFile->getWidth(),dicomTempFile->getHeight(),CV_8U,pixel);

        QImage img=QImage((const uchar*)dst.data,dst.cols,dst.rows,dst.cols*dst.channels(),QImage::Format_Indexed8);

        QImage img_scaled=img.scaled(img.size(),Qt::IgnoreAspectRatio);

        if(tempDicom==TEMPDICOM1) ui->DicomShow1->setPixmap(QPixmap::fromImage(img_scaled));
        else if(tempDicom==TEMPDICOM2) ui->DicomShow2->setPixmap(QPixmap::fromImage(img_scaled));
    }
}


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
    dicomTempImgShow(TEMPDICOM1);
    dicomTempImgShow(TEMPDICOM2);
}

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
    dicomTempImgShow(TEMPDICOM1);
    dicomTempImgShow(TEMPDICOM2);
}

