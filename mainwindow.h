#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sstream>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QBoxLayout>
#include <QListWidget>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QDebug>
#include <QLabel>
#include <QTimer>

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

private:
    Ui::MainWindow *ui;

    // CORE
    QMediaPlayer * media_player;
    QMediaPlaylist * loaded_files;
    QGraphicsScene media_gScene;
    QTimer* timer_progress;

    // UI
    QListWidget * media_list;
    QGraphicsView media_gView;
    QSlider * media_progress;
    QLabel *media_time, *media_cur_time;
};
#endif // MAINWINDOW_H
