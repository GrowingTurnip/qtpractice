#ifndef MUSIC_ANALYSIS_H
#define MUSIC_ANALYSIS_H



#include <QString>
#include <QMediaPlayer>
#include <QObject>

typedef struct MUSIC_info
{
    QString mic_name;           //曲名
    QString mic_songer;         //歌手
    QString mic_time;           //时长
    QString mic_album;          //专辑
    QString mic_path;           //mp3所在路径
    QString pic_flag="0";       //是否有图片
    QString  pic_path;          //歌曲图片
    QString  pic_type;          //图片类型 jpg,png
    QString showlist;

    MUSIC_info& operator=(const MUSIC_info &info1);   // 重载 operator= 运算符

}MUSIC_info;

typedef struct TAB_info_st
{
    char format[3];           //格式
    char version;             //版本
    char unuse[2];
    char header_size[4];      //标签帧+标签头的size
}TAB_info;

typedef struct HEAD_info    //标签帧头帧：每帧前8位
{
    char FrameID[4];
    char Size[4];
    char Flags[2];
}HEAD_info;

class music_analysis : public QObject
{
    Q_OBJECT
public:
    explicit music_analysis(QObject *parent = nullptr);
    MUSIC_info m_music_info;
    QMediaPlayer *temp_MP;
    MUSIC_info analyse_music(QString path);

signals:
    void music_ananly_complete_signal(MUSIC_info);      //处理完成信号
private slots:
    void onDurationChanged(qint64);
};






#endif // MUSIC_ANALYSIS_H
