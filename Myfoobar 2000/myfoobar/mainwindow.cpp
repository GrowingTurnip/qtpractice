#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QSlider>
#include <QDebug>
#include <QMediaMetaData>

#include "music_analysis.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("myfoobar"));
    setCentralWidget(ui->splitter);  // 使splitter填充满整个工作区
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);  // 循环播放
    player->setPlaylist(playlist);
    sliderVolumn = new QSlider(Qt::Horizontal, this);
    ui->mainToolBar->addWidget(sliderVolumn);  // 向工具栏中添加调节音量的slider
    ui->mainToolBar->addSeparator();
    sliderPosition = new QSlider(Qt::Horizontal, this);
    ui->mainToolBar->addWidget(sliderPosition);  // 向工具栏中添加调节进度的slider
    labRatio = new QLabel(this);
    ui->statusBar->addWidget(labRatio);
    labCurMedia = new QLabel(this);
    ui->statusBar->addWidget(labCurMedia);

    // 树形目录
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel(QString("All Music"));

    // 播放表格
    ui->defaultPlaylistWidget->setColumnCount(3);
    ui->defaultPlaylistWidget->setFrameShape(QFrame::NoFrame);
    ui->defaultPlaylistWidget->setShowGrid(false);  // 隐藏tablewidget的栅格
    ui->defaultPlaylistWidget->verticalHeader()->setVisible(false);  // 隐藏各行的行名称
    ui->defaultPlaylistWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 三列充满表格
    ui->defaultPlaylistWidget->setSelectionMode(QAbstractItemView::SingleSelection);  // 设置选择模式为单选
    ui->defaultPlaylistWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 每次选择一行


    connect(player, SIGNAL(metaDataChanged()),
            this, SLOT(metaDataChanged()));


    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onStateChanged(QMediaPlayer::State)));

    connect(player, SIGNAL(positionChanged(qint64)),
            this, SLOT(onPositionChanged(qint64)));

    connect(player, SIGNAL(durationChanged(qint64)),
            this, SLOT(onDurationChanged(qint64)));

    connect(ui->defaultPlaylistWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(on_tableWidget_doubleClicked(int, int)));


    connect(sliderVolumn, &QSlider::valueChanged,
            this, &MainWindow::on_sliderVolumnChanged);

    connect(sliderPosition, &QSlider::valueChanged,
            this, &MainWindow::on_sliderPositionChanged);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createItemsRow(int rowNo, QString title, QString artist, QString album)
{
    // 添加一行的数据
    // 创建items
    QTableWidgetItem *item;

    // 播放歌名
    item = new QTableWidgetItem(title);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->defaultPlaylistWidget->setItem(rowNo, 0, item);

    // 添加歌手
    item = new QTableWidgetItem(artist);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->defaultPlaylistWidget->setItem(rowNo, 1, item);

    // 添加歌名
    item = new QTableWidgetItem(album);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->defaultPlaylistWidget->setItem(rowNo, 2, item);

}

void MainWindow::creatMusicTree(MUSIC_info musicInfo)
{
    QString songerAndAlbum = (musicInfo.mic_songer + "-" + musicInfo.mic_album);
    QString name = musicInfo.mic_name;

    bool albumIsExisted = false;  // 树形目录中专辑是否存在

    QTreeWidgetItem *searcheditem = searchMusicTree(songerAndAlbum, albumIsExisted);
    if (!albumIsExisted) {
        // 专辑不存在， 则向目录中添加专辑和其子节点歌名
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, songerAndAlbum);
        QTreeWidgetItem *item1 = new QTreeWidgetItem(item);
        item1->setText(0, name);
    }
    else {
        if (searcheditem);  // 专辑存在，找到了歌曲不做任何操作
        else {
            // 在专辑下没有找到歌曲则插入子节点
        QTreeWidgetItem *item1 = new QTreeWidgetItem(searcheditem);
        item1->setText(0, name);
        }
    }


}

QTreeWidgetItem *MainWindow::searchMusicTree(QString str, bool &flag)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        if ((*it)->text(0) == str) {
            flag = true;
            return (*it)->parent();  // 存在节点,返回其父节点
        }
        ++it;
    }
    return nullptr;  // 没有找到
}





void MainWindow::metaDataChanged()
{
    // 歌曲切换时更新的信息
    if (player->isMetaDataAvailable()) {
        QString artist = player->metaData(QMediaMetaData::Author).toString();
        QString album = player->metaData(QMediaMetaData::AlbumTitle).toString();
        QString title = player->metaData(QMediaMetaData::Title).toString();
        setWindowTitle(artist + "-" + title);
        labCurMedia->setText(artist + "-" + title);
    }
}






void MainWindow::onStateChanged(QMediaPlayer::State state)
{
    // 播放器状态发生变换，更新按钮状态
    ui->actionPlay->setEnabled(!(state == QMediaPlayer::PlayingState));  // 处于播放状态时不可点
    ui->actionPause->setEnabled(state == QMediaPlayer::PlayingState);
    ui->actionStop->setEnabled(state == QMediaPlayer::PlayingState);

}




void MainWindow::onDurationChanged(qint64 duration)
{
    // 文件时长变化， 更新进度显示
    sliderPosition->setMaximum(duration);
    int secs = duration / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    durationTime = QString::asprintf("%2d:%2d",mins,secs);
    musicInfo.mic_time=QString(durationTime);
    labRatio->setText(positionTime + "/" + durationTime);
}



void MainWindow::onPositionChanged(qint64 position)
{
    // 当前文件播放位置变化，更新进度显示
    if (sliderPosition->isSliderDown())  // 正处于手动调整状态，不处理
        return;
    sliderPosition->setSliderPosition(position);
    int secs = position / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    positionTime = QString::asprintf("%d : %d", mins, secs);
    labRatio->setText(positionTime + "/" + durationTime);

}







void MainWindow::on_actionOpen_File_triggered()
{
    // 添加新文件
    QString curPath = QDir::homePath();  // 获取用户目录
    QString dlgTitle = "Add Files";
    QString filter = "音频文件(*.mp3 *.wav *wma);; mp3 文件(*.mp3);; wav 文件(*.wav);; wma 文件(*.wma);; 所有文件(*.*)";;
    QStringList fileList = QFileDialog::getOpenFileNames(this, dlgTitle, curPath, filter);
    if (fileList.count() < 1)
        return;
    for (int i = 0; i < fileList.count(); ++i) {
        QString aFile = fileList.at(i);
        playlist->addMedia(QUrl::fromLocalFile(aFile));  // 向播放列表中添加文件

         QFileInfo fileInfo(aFile);

        musicInfo = musicAnalysis.analyse_music(fileInfo.filePath());

        QString title = musicInfo.mic_name;
        QString artist = musicInfo.mic_songer;
        QString album = musicInfo.mic_album;

        createItemsRow(i, title, artist, album);
        creatMusicTree(musicInfo);
    }
    if (player->state() != QMediaPlayer::PlayingState)
        playlist->setCurrentIndex(0);
    player->play();

}

void MainWindow::on_actionPlay_triggered()
{
    // 播放歌曲
    if (playlist->currentIndex() < 0)
        playlist->setCurrentIndex(0);
    player->play();
}

void MainWindow::on_actionPause_triggered()
{
    // 暂停播放
    player->pause();
}

void MainWindow::on_actionStop_triggered()
{
    // 停止播放
    player->stop();
}


void MainWindow::on_actionPrevious_triggered()
{
    // 播放上一首
    playlist->previous();
    player->play();
}

void MainWindow::on_actionNext_triggered()
{
    // 播放下一首
    playlist->next();
    player->play();
}

void MainWindow::on_tableWidget_doubleClicked(int pos, int)
{
    // 双击实现播放
    playlist->setCurrentIndex(pos);
    sliderPosition->setValue(0);
    player->play();
}


void MainWindow::on_sliderVolumnChanged(int value)
{
    // 调节音量
    player->setVolume(value);

}

void MainWindow::on_sliderPositionChanged(int value)
{
    // 歌曲进度控制
    player->setPosition(value);

}




void MainWindow::on_actionExit_triggered()
{
    this->close();
}
