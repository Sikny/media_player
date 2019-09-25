#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->media_player = new QMediaPlayer(this);
    this->loaded_files = new QMediaPlaylist(media_player);
    media_player->setPlaylist(loaded_files);

    QHBoxLayout *mainLayout = new QHBoxLayout();
        QGroupBox * grpMediaList = new QGroupBox(tr("Current queue"), centralWidget());
            QVBoxLayout *boxLayout = new QVBoxLayout(grpMediaList);
            media_list = new QListWidget(centralWidget());
            boxLayout->addWidget(media_list);
        grpMediaList->setLayout(boxLayout);
        mainLayout->addWidget(grpMediaList);
        QVBoxLayout *rightLayout = new QVBoxLayout();
            rightLayout->addSpacing(25);
            media_gView.setScene(&media_gScene);
            rightLayout->addWidget(&media_gView);
            media_gView.setFixedSize(300, 200);
            media_progress = new QSlider(Qt::Horizontal, centralWidget());
            media_progress->setFixedSize(265, 20);
            media_time = new QLabel("00:00", centralWidget());
            media_time->setFixedSize(35, 16);
            QHBoxLayout * progressLayout = new QHBoxLayout();
                progressLayout->addWidget(media_progress, 0, Qt::AlignTop);
                progressLayout->addWidget(media_time, 0, Qt::AlignTop);
            rightLayout->addLayout(progressLayout);
        mainLayout->addLayout(rightLayout);
    centralWidget()->setLayout(mainLayout);

    media_gScene.setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    enableButtons(false);
    findChild<QAction*>("actionPause")->setEnabled(false);

    connect(loaded_files, SIGNAL(mediaInserted(int, int)), this, SLOT(updateQueue(int, int)));
    connect(this, SIGNAL(hasMedia(bool)), this, SLOT(enableButtons(bool)));

    timer_progress = new QTimer(this);
    connect(timer_progress, SIGNAL(timeout()), this, SLOT(updateProgressTimer()));
    timer_progress->start(1000);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actionAbout_triggered() {
    QMessageBox::information(this, tr("About"), tr("Version 1.0.0\nAuthor: Sikny"));
}

void MainWindow::on_actionLoad_media_triggered() {
    QStringList fileUrls = QFileDialog::getOpenFileNames(this, tr("Open media"),
            QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
            tr("Audio files(*.mp3 *.flac *.wav)"));
    QList<QMediaContent> media_files;
    for(QString fileUrl : fileUrls){
        media_files.append(QUrl::fromLocalFile(fileUrl));
    }
    loaded_files->addMedia(media_files);
}

void MainWindow::updateQueue(int start, int end){
    for(int i = start; i <= end; i++){
        media_list->addItem(loaded_files->media(i).canonicalUrl().fileName());
    }
    if(media_list->count() > 0) emit hasMedia(true);
}

void MainWindow::enableButtons(bool status){
    QList<QAction*> mediaActions = {
        findChild<QAction*>("actionPlay"),
        findChild<QAction*>("actionStop"),
        findChild<QAction*>("actionNext"),
        findChild<QAction*>("actionPrevious")
    };
    for(QAction* action : mediaActions){
        action->setEnabled(status);
    }
}

void MainWindow::on_actionPlay_triggered() {
    findChild<QAction*>("actionPause")->setEnabled(true);
    findChild<QAction*>("actionPlay")->setEnabled(false);
    media_player->play();
}

void MainWindow::on_actionPause_triggered() {
    findChild<QAction*>("actionPause")->setEnabled(false);
    findChild<QAction*>("actionPlay")->setEnabled(true);
    media_player->pause();
}

void MainWindow::on_actionStop_triggered() {
    media_player->stop();
}

void MainWindow::updateProgressTimer(){
    media_progress->setMaximum(static_cast<int>(media_player->duration()));
    media_progress->setMinimum(0);
    media_progress->setSliderPosition(static_cast<int>(media_player->position()));
}
