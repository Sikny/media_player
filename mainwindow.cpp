#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), current_channel(nullptr) {
    ui->setupUi(this);
    FMOD_System_Create(&fmod_system);
    FMOD_System_Init(fmod_system, 2, FMOD_INIT_NORMAL, nullptr);

    loaded_files = new QMediaPlaylist(this);

    timer_progress = new QTimer(this);
    connect(timer_progress, SIGNAL(timeout()), this, SLOT(updateProgressTimer()));
    timer_progress->start(100);

    QHBoxLayout *mainLayout = new QHBoxLayout();
        QGroupBox * grpMediaList = new QGroupBox(tr("Current queue"), centralWidget());
            QVBoxLayout *boxLayout = new QVBoxLayout(grpMediaList);
            media_list = new QListWidget(centralWidget());
            boxLayout->addWidget(media_list);
        grpMediaList->setLayout(boxLayout);
        mainLayout->addWidget(grpMediaList);
        QVBoxLayout *rightLayout = new QVBoxLayout();
            rightLayout->addSpacing(25);
            media_gView = new QGraphicsView();
            media_gScene = new QGraphicsScene();
            media_gView->setScene(media_gScene);
            rightLayout->addWidget(media_gView);
            media_gView->setFixedSize(330, 200);
            media_progress = new QSlider(Qt::Horizontal, centralWidget());
            media_progress->setFixedSize(240, 20);
            connect(media_progress, SIGNAL(sliderReleased()), this, SLOT(updateMedia()));
            connect(media_progress, SIGNAL(sliderPressed()), timer_progress, SLOT(stop()));
            media_time = new QLabel("00:00", centralWidget());
            media_time->setFixedSize(30, 16);
            media_cur_time = new QLabel("00:00", centralWidget());
            media_cur_time->setFixedSize(30, 16);
            QHBoxLayout * progressLayout = new QHBoxLayout();
                progressLayout->addWidget(media_cur_time, 0, Qt::AlignTop);
                progressLayout->addWidget(media_progress, 0, Qt::AlignTop);
                progressLayout->addWidget(media_time, 0, Qt::AlignTop);
            rightLayout->addLayout(progressLayout);
        mainLayout->addLayout(rightLayout);
    centralWidget()->setLayout(mainLayout);

    media_gScene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    enableButtons(false);
    findChild<QAction*>("actionPause")->setEnabled(false);

    connect(loaded_files, SIGNAL(mediaInserted(int, int)), this, SLOT(updateQueue(int, int)));
    connect(this, SIGNAL(hasMedia(bool)), this, SLOT(enableButtons(bool)));
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
    QList<QMediaContent> mediaList;
    for(QString fileUrl : fileUrls){
        mediaList.append(QUrl::fromLocalFile(fileUrl));
    }
    loaded_files->addMedia(mediaList);
}

void MainWindow::updateQueue(int start, int end){
    for(int i = start; i <= end; i++){
        media_list->addItem(loaded_files->media(i).canonicalUrl().fileName());
    }
    if(loaded_files->currentIndex() == -1) {
        loaded_files->setCurrentIndex(0);
        FMOD_System_CreateSound(fmod_system,
                    loaded_files->currentMedia().canonicalUrl().toLocalFile().toStdString().c_str(),
                    FMOD_DEFAULT, nullptr, &current_media);
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
    int pauseStatus = 0;
    if(current_channel != nullptr){
        FMOD_Channel_GetPaused(current_channel, &pauseStatus);
    }
   if(!pauseStatus){
        FMOD_System_PlaySound(fmod_system, current_media, nullptr, false, &current_channel);
    } else
        FMOD_Channel_SetPaused(current_channel, false);
}

void MainWindow::on_actionPause_triggered() {
    findChild<QAction*>("actionPause")->setEnabled(false);
    findChild<QAction*>("actionPlay")->setEnabled(true);
    FMOD_Channel_SetPaused(current_channel, true);
}

void MainWindow::on_actionStop_triggered() {
    FMOD_Channel_Stop(current_channel);
    findChild<QAction*>("actionPlay")->setEnabled(true);
    findChild<QAction*>("actionPause")->setEnabled(false);
}

void MainWindow::updateProgressTimer(){
    if(current_channel != nullptr){
        unsigned int media_length, position;
        FMOD_Sound_GetLength(current_media, &media_length, FMOD_TIMEUNIT_MS);
        media_progress->setMaximum(static_cast<int>(media_length));
        media_progress->setMinimum(0);
        FMOD_Channel_GetPosition(current_channel, &position, FMOD_TIMEUNIT_MS);
        media_progress->setSliderPosition(static_cast<int>(position));

        int maxSecs = static_cast<int>(media_length) / 1000;
        int maxMins = maxSecs / 60;
        maxSecs -= maxMins * 60;
        std::stringstream maxTimeText;
        maxTimeText << (maxMins<10?"0":"") << maxMins << ":" << (maxSecs<10?"0":"") << maxSecs;
        media_time->setText(QString::fromStdString(maxTimeText.str()));

        int curSecs = static_cast<int>(position) / 1000;
        int curMins = curSecs / 60;
        curSecs -= curMins * 60;
        std::stringstream minTimeText;
        minTimeText << (curMins<10?"0":"") << curMins << ":" << (curSecs<10?"0":"") << curSecs;
        media_cur_time->setText(QString::fromStdString(minTimeText.str()));
    }
}

void MainWindow::updateMedia(){
    if(current_channel != nullptr){
        FMOD_Channel_SetPosition(current_channel, static_cast<unsigned int>(media_progress->value()),
                                 FMOD_TIMEUNIT_MS);
        timer_progress->start();
    }
}

void MainWindow::setPixel(int x, int y){
    media_gScene->addLine(x, 0, x, y, QPen(QColor(255, 0, 0)));
}

void MainWindow::on_actionNext_triggered() {
    if(loaded_files->currentIndex() < loaded_files->mediaCount()-1){
        loaded_files->next();
        int paused = 0;
        FMOD_Channel_GetPaused(current_channel, &paused);
        FMOD_Channel_Stop(current_channel);
        FMOD_Sound_Release(current_media);
        FMOD_System_CreateSound(fmod_system,
                    loaded_files->currentMedia().canonicalUrl().toLocalFile().toStdString().c_str(),
                    FMOD_DEFAULT, nullptr, &current_media);
        FMOD_System_PlaySound(fmod_system, current_media, nullptr, paused, &current_channel);
    }
}
