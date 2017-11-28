#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QWidget>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextDocument>
#include <QThread>
#include <QProcess>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void uploadFinished(QNetworkReply *reply);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

    void onfinishCoy(QNetworkReply *rep);
    void cekSensor(QNetworkReply *rep);
    void cekPortal(QNetworkReply *rep);
    void mintaSensor(QNetworkReply *rep);
    void terimaSensor(QNetworkReply *rep);

    void kameraStart();
    void kameraUpdate();
    QImage putImage(const Mat& mat);

    void showOnboard(void);

    void on_pushButton_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

    void on_radioButton_pressed();

    void on_radioButton_2_pressed();

    void on_radioButton_3_pressed();

    void on_koneksi_pressed();

    void on_isi_selectionChanged();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *m_manager;
    QString m_fileName;
    QFile *m_file;
    QString kbdprog;
};

#endif // MAINWINDOW_H
