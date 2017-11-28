#include "mainwindow.h"
#include "ui_mainwindow.h"

QString namaFilenya,alamatFilenya,waktu,tipe,balasanSql;
QString alamatPortal,alamatSensor;
QTimer *timer1;
Mat streamKamera, capture;
VideoCapture kamera(0);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kameraStart();

    ui->progressBar->setVisible(false);
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));

    kbdprog = "onboard";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uploadFinished(QNetworkReply *reply)
{
    if (!reply->error())
    {
        m_file->close();
        m_file->deleteLater();
        reply->deleteLater();
        ui->progressBar->setVisible(false);
        if (balasanSql=="berhasil") ui->statusbar_e205->setText("Upload selesai !");
        else ui->statusbar_e205->setText("Upload Gagal (database)");
    } else {
        ui->statusbar_e205->setText("Upload Gagal (foto)");
    }
}

void MainWindow::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(100 * bytesSent/bytesTotal);
    ui->statusbar_e205->setText("Uploading.....");
}

void MainWindow::onfinishCoy(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    balasanSql=bts;
}

void MainWindow::cekSensor(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString balasan(bts);

    if (balasan=="jembatan_sensor_node"){
        ui->ipsensor->setStyleSheet("background-color: rgb(202, 202, 202);");
        ui->ipsensor->setReadOnly(true);
    } else {
        ui->ipsensor->setStyleSheet("background-color: rgb(255, 255, 255);");
        ui->ipsensor->setReadOnly(false);
    }
}

void MainWindow::cekPortal(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString balasan(bts);

    if (balasan=="jembatan_portal_node"){
        ui->ipportal->setStyleSheet("background-color: rgb(202, 202, 202);");
        ui->ipportal->setReadOnly(true);
    } else {
        ui->ipportal->setStyleSheet("background-color: rgb(255, 255, 255);");
        ui->ipportal->setReadOnly(false);
    }
}

void MainWindow::mintaSensor(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString balasan(bts);

    if (balasan=="baca"){
        ui->pushButton->setText("selesai");
    }
}

void MainWindow::terimaSensor(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString balasan(bts);

    if (balasan.length()>0){
        ui->statusbar_e205->setText("Terima data sensor (" + balasan + ")");
        ui->pushButton->setText("Baca Sensor");

        if (balasan.length()==7){
            QStringList pisah1 = balasan.split('a');
            QStringList pisah2 = pisah1[1].split('b');
            QString sensor1=pisah2[0];
            QStringList pisah3 = pisah2[1].split('c');
            QString sensor2=pisah3[0];
            QStringList pisah4 = pisah3[1].split('d');
            QString sensor3=pisah4[0];

            if (tipe=="Tipe1") {
                if (sensor1=="1") ui->isi_4->setText("Tilang");
                else ui->isi_4->setText("Lolos");
            }

            if (tipe=="Tipe2") {
                if (sensor2=="1") ui->isi_4->setText("Tilang");
                else ui->isi_4->setText("Lolos");
            }

            if (tipe=="Tipe3") {
                if (sensor3=="1") ui->isi_4->setText("Tilang");
                else ui->isi_4->setText("Lolos");
            }

            if (ui->isi_4->text()=="Tilang"){
                QNetworkAccessManager *mgr1 = new QNetworkAccessManager(this);
                connect(mgr1,SIGNAL(finished(QNetworkReply*)),mgr1,SLOT(deleteLater()));

                alamatSensor="http://192.168.1.101/tilang";
                mgr1->get(QNetworkRequest(QUrl(alamatSensor)));

                QNetworkAccessManager *mgr2 = new QNetworkAccessManager(this);
                connect(mgr1,SIGNAL(finished(QNetworkReply*)),mgr2,SLOT(deleteLater()));

                alamatSensor="http://192.168.1.100/tidak";
                mgr2->get(QNetworkRequest(QUrl(alamatSensor)));
            }

            if (ui->isi_4->text()=="Lolos"){
                QNetworkAccessManager *mgr1 = new QNetworkAccessManager(this);
                //connect(mgr1,SIGNAL(finished(QNetworkReply*)),this,SLOT(cekSensor(QNetworkReply*)));
                connect(mgr1,SIGNAL(finished(QNetworkReply*)),mgr1,SLOT(deleteLater()));

                alamatSensor="http://192.168.1.101/lolos";
                mgr1->get(QNetworkRequest(QUrl(alamatSensor)));

                QNetworkAccessManager *mgr2 = new QNetworkAccessManager(this);
                connect(mgr1,SIGNAL(finished(QNetworkReply*)),mgr2,SLOT(deleteLater()));

                alamatSensor="http://192.168.1.100/ya";
                mgr2->get(QNetworkRequest(QUrl(alamatSensor)));
            }

            QThread::msleep(3000);
            QNetworkAccessManager *mgr3 = new QNetworkAccessManager(this);
            connect(mgr3,SIGNAL(finished(QNetworkReply*)),mgr3,SLOT(deleteLater()));

            alamatSensor="http://192.168.1.100/reset";
            mgr3->get(QNetworkRequest(QUrl(alamatSensor)));


        }
    }
}


void MainWindow::kameraStart()
{
    if (!kamera.isOpened() )
    {
        qDebug() << "Cannot open the web cam" << endl;
    }

    timer1=new QTimer(this);
    //timer1->start(100);
    connect(timer1,SIGNAL(timeout()),this,SLOT(kameraUpdate()));
}

void MainWindow::kameraUpdate()
{
    bool berhasil = kamera.read(streamKamera);
    if (!berhasil){
        qDebug() << "Cannot read a frame from video stream" << endl;
    } else {
        ui->kamera->setPixmap(QPixmap::fromImage(putImage(streamKamera)));
    }
}

QImage MainWindow::putImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
        if(mat.type()==CV_8UC1)
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for (int i=0; i<256; i++)
                colorTable.push_back(qRgb(i,i,i));
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            img.setColorTable(colorTable);
            return img;
        }
        // 8-bits unsigned, NO. OF CHANNELS=3
        if(mat.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        else
        {
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            return QImage();
        }
}



void MainWindow::on_pushButton_pressed()
{
    if (ui->pushButton->text()=="Baca Sensor"){
        QNetworkAccessManager *mgr3 = new QNetworkAccessManager(this);
        connect(mgr3,SIGNAL(finished(QNetworkReply*)),this,SLOT(mintaSensor(QNetworkReply*)));
        connect(mgr3,SIGNAL(finished(QNetworkReply*)),mgr3,SLOT(deleteLater()));

        alamatSensor="http://"+ui->ipsensor->text()+"/baca";
        mgr3->get(QNetworkRequest(QUrl(alamatSensor)));
    } else if (ui->pushButton->text()=="selesai"){
        QNetworkAccessManager *mgr3 = new QNetworkAccessManager(this);
        connect(mgr3,SIGNAL(finished(QNetworkReply*)),this,SLOT(terimaSensor(QNetworkReply*)));
        connect(mgr3,SIGNAL(finished(QNetworkReply*)),mgr3,SLOT(deleteLater()));

        alamatSensor="http://"+ui->ipsensor->text()+"/sensor";
        mgr3->get(QNetworkRequest(QUrl(alamatSensor)));
    }


}

void MainWindow::on_pushButton_2_pressed()
{
    QStringList nama=QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    //qDebug()<<nama[0];
    QImage foto=putImage(streamKamera);
    namaFilenya=QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + ".jpg";
    waktu=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    alamatFilenya=nama[0] + "/Foto/";
    foto.save(alamatFilenya + namaFilenya,"JPG");
    ui->namaFoto_->setText(namaFilenya);
    //capture = streamKamera;
    //imwrite(nama[0] + "/Foto/test.jpg", capture);
}

void MainWindow::on_pushButton_3_pressed()
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinishCoy(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    QString alamatLengkap="http://jembatan.e205.org/daftar.php?&waktu="+waktu+"&nopol="+ui->isi->text()+"&tipe="+tipe+"&batas="+ui->isi_3->text()+"&ket="+ui->isi_4->text()+"&foto=foto/"+namaFilenya;
    mgr->get(QNetworkRequest(QUrl(alamatLengkap)));


    m_file = new QFile(alamatFilenya + namaFilenya);

    QUrl url("ftp://ftp.e205.org/jembatan.e205.org/foto/" + namaFilenya);
    url.setUserName("e205");
    url.setPassword("10.124.20.205");
    url.setScheme("ftp");
    url.setPort(21);

    if (m_file->open(QIODevice::ReadOnly))
    {
        QNetworkReply *reply = m_manager->put(QNetworkRequest(url), m_file);
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    }

    //ui->plainTextEdit->setPlainText("====================\nJembatan Timbang\n "+waktu+"\n====================\nNoPol: "+ui->isi->text()+"\nTipe : "+tipe+"\nBatas: "+ui->isi_3->text()+"\nKet  : "+ui->isi_4->text()+"\n====================");

    QString datanyaa = "====================\nJembatan Timbang\n "+waktu+"\n====================\nNoPol: "+ui->isi->text()+"\nTipe : "+tipe+"\nBatas: "+ui->isi_3->text()+"\nKet  : "+ui->isi_4->text()+"\n====================";

    QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document");

        if (dialog->exec() != QDialog::Accepted)
            dialog->close();

        QPainter painter;
        painter.begin(&printer);
        painter.drawText(100, 100, 500, 500, Qt::AlignLeft|Qt::AlignTop, datanyaa);
        painter.end();

        QNetworkAccessManager *mgr4 = new QNetworkAccessManager(this);
        connect(mgr4,SIGNAL(finished(QNetworkReply*)),mgr4,SLOT(deleteLater()));

        alamatSensor="http://192.168.1.101/reset";
        mgr4->get(QNetworkRequest(QUrl(alamatSensor)));
}



void MainWindow::on_radioButton_pressed()
{
    tipe="Tipe1";
    ui->isi_3->setText("14");
}

void MainWindow::on_radioButton_2_pressed()
{
    tipe="Tipe2";
    ui->isi_3->setText("28");
}

void MainWindow::on_radioButton_3_pressed()
{
    tipe="Tipe3";
    ui->isi_3->setText("42");
}

int status=0;
void MainWindow::on_koneksi_pressed()
{
    if (status==0){
        kamera.open(0);
        timer1->start(50);

        QNetworkAccessManager *mgr1 = new QNetworkAccessManager(this);
        connect(mgr1,SIGNAL(finished(QNetworkReply*)),this,SLOT(cekSensor(QNetworkReply*)));
        connect(mgr1,SIGNAL(finished(QNetworkReply*)),mgr1,SLOT(deleteLater()));

        alamatSensor="http://"+ui->ipsensor->text();
        mgr1->get(QNetworkRequest(QUrl(alamatSensor)));

        QNetworkAccessManager *mgr2 = new QNetworkAccessManager(this);
        connect(mgr2,SIGNAL(finished(QNetworkReply*)),this,SLOT(cekPortal(QNetworkReply*)));
        connect(mgr2,SIGNAL(finished(QNetworkReply*)),mgr2,SLOT(deleteLater()));

        alamatPortal="http://"+ui->ipportal->text();
        mgr2->get(QNetworkRequest(QUrl(alamatPortal)));

        ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/conect.png);");
        ui->statusbar_e205->setText("Memulai integrasi sistem");
        status=1;
    } else {
        kamera.release();
        timer1->stop();

        ui->ipsensor->setStyleSheet("background-color: rgb(255, 255, 255);");
        ui->ipsensor->setReadOnly(false);
        ui->ipportal->setStyleSheet("background-color: rgb(255, 255, 255);");
        ui->ipportal->setReadOnly(false);

        ui->kamera->clear();
        ui->kamera->setText("Tidak Terhubung Kamera");

        ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/disconect.png);");
        ui->statusbar_e205->setText("Koneksi terputus");
        status=0;
    }
}

void MainWindow::showOnboard(){
    QProcess *kbdproc = new QProcess(this);
    kbdproc->start(kbdprog);
}

void MainWindow::on_isi_selectionChanged()
{
    showOnboard();
}
