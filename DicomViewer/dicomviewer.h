#ifndef DICOMVIEWER_H
#define DICOMVIEWER_H

#define TEMPDICOM1 1
#define TEMPDICOM2 2
#define MAXLENGTH 260

#include <QMainWindow>


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

private:
    int winWidth=0,winCenter=0;

private slots:
    void on_FileOpenButton2_clicked();

    void on_FileOpenButton_clicked();

    void on_setLungWin_clicked();

    void on_setMedisatWin_clicked();


private:
    Ui::DicomViewer *ui;
};

#endif // DICOMVIEWER_H
