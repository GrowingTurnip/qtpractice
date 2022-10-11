#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QLabel>
#include <QString>
#include <QTreeWidget>

#include "music_analysis.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMediaPlayer *player;  // 播放器
    QMediaPlaylist *playlist;  // 播放列表
    QString durationTime;  // 总长度
    QString positionTime;  // 当前播放的位置
    QSlider *sliderVolumn;  //  音量
    QSlider *sliderPosition;  // 进度条
    QLabel *labRatio;   // 播放进度
    QLabel *labCurMedia;  // 当前播放文件名

    music_analysis musicAnalysis;
    MUSIC_info musicInfo;




    void createItemsRow(int rowNo, QString title, QString artist, QString album);  // 向tablewidget插入一行数据

    void creatMusicTree(MUSIC_info musicInfo);   // 根据歌曲信息创建树形目录

    QTreeWidgetItem* searchMusicTree(QString str, bool&);  // 查找树形目中是否存在节点

private slots:
    // 自定义槽函数
    void metaDataChanged();
    void onStateChanged(QMediaPlayer::State state);  // 播放状态改变
    void onDurationChanged(qint64 duration);  //  播放文件变化
    void onPositionChanged(qint64 position);  //   播放位置改变

    void on_actionOpen_File_triggered();
    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();
    void on_tableWidget_doubleClicked(int, int);

    void on_sliderVolumnChanged(int value);  // 调节音量
    void on_sliderPositionChanged(int value);  // 调节播放进度



    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
