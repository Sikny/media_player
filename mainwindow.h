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
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <fmod.h>
#include "graphicsarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void loadMedia(QString arg);
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
    void updateRenderArea();

    void on_actionNext_triggered();
    void on_actionPrevious_triggered();

private:
    Ui::MainWindow *ui;

    // CORE
    FMOD_SYSTEM* fmod_system;
    FMOD_CHANNEL* current_channel;
    FMOD_CHANNELGROUP* master_group;
    FMOD_SOUND* current_media;
    FMOD_DSP* dsp;
    QMediaPlaylist* loaded_files;
    QTimer* timer_progress;

    // OpenGL
    GraphicsArea * media_gView;

    // UI
    QListWidget * media_list;
    QSlider * media_progress;
    QLabel *media_time, *media_cur_time;
    QCheckBox *loopCheckbox;
};
#endif // MAINWINDOW_H
