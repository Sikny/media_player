#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sstream>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QBoxLayout>
#include <QListWidget>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QToolBar>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <fmod.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void hasMedia(bool);
    void mediaInserted(int, int);

private slots:
    void on_actionAbout_triggered();
    void on_actionLoad_media_triggered();
    void updateQueue(int, int);
    void enableButtons(bool);

    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

    void updateProgressTimer();
    void updateMedia();

    void on_actionNext_triggered();

private:
    void setPixel(int x, int y);

    Ui::MainWindow *ui;

    // CORE
    FMOD_SYSTEM* fmod_system;
    FMOD_CHANNEL* current_channel;
    FMOD_SOUND* current_media;
    QMediaPlaylist* loaded_files;
    QGraphicsScene * media_gScene;
    QTimer* timer_progress;

    // UI
    QListWidget * media_list;
    QGraphicsView * media_gView;
    QSlider * media_progress;
    QLabel *media_time, *media_cur_time;
};
#endif // MAINWINDOW_H
