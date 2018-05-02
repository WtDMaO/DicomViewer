#ifndef DICOMVIEWER_H
#define DICOMVIEWER_H

#define TEMPDICOM1 1
#define TEMPDICOM2 2
#define MAXLENGTH 260

#define SCALED_ON 1
#define SCALED_OFF 0

#include <QMainWindow>
#include <QMouseEvent>
#include <QImage>

#include <opencv2/core/core.hpp>

namespace Ui {
class DicomViewer;
}

class DicomViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DicomViewer(QWidget *parent = 0);



    ~DicomViewer();

private:
    void dicomImgShow(QString loadFileName,int tempDicom);

    void dicomImgShow(int tempDicom);

    cv::Mat dicomWindowAdj(int tempDicom);

    void mouseMoveEvent(QMouseEvent *e);

    void mousePressEvent(QMouseEvent *e);

    //dicom文件转换成Qimage,根据tempFileName的值对应给全局变量dicomMat1和2赋值
    cv::Mat dicom2Cvmat(QString fileName,int tempFileName);


private:
    int winWidth=0,winCenter=0;

    int currentSize=512;

    double mouseX=0,mouseY=0;

    double mouseX_m=0.,mouseY_m=0;

    cv::Mat dicomMat1,dicomMat2;

private slots:
    void on_FileOpenButton2_clicked();

    void on_FileOpenButton_clicked();

    void on_setLungWin_clicked();

    void on_setMedisatWin_clicked();


private:
    Ui::DicomViewer *ui;
};

#endif // DICOMVIEWER_H
