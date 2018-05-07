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
class GraphicView;
}

class DicomViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DicomViewer(QWidget *parent = 0);

    double mouseX=0,mouseY=0;

    double mouseX_m=0.,mouseY_m=0;

    ~DicomViewer();

private:
    void dicomImgShow(int tempDicom);

    void dicomImgShow(int tempDicom,int dx,int dy,int scale);

    void dicomWindowAdj(int tempDicom);

    //dicom文件转换成Qimage,根据tempFileName的值对应给全局变量dicomMat1和2赋值
    void dicom2Cvmat(QString fileName,int tempFileName);


private:
    int winWidth=0,winCenter=0;

    int currentSize=512;

    cv::Mat dicomMat1,dicomMat2;

private slots:
    void on_FileOpenButton2_clicked();

    void on_FileOpenButton_clicked();

    void on_setLungWin_clicked();

    void on_setMedisatWin_clicked();



    void on_pushButton_clicked();

private:
    Ui::DicomViewer *ui;
};


#endif // DICOMVIEWER_H
