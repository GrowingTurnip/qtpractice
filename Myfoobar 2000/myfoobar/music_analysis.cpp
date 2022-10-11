
#include "music_analysis.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QMediaPlayer>
#include <QObject>

music_analysis::music_analysis(QObject *parent) : QObject(parent)
{
    temp_MP = new QMediaPlayer;
    temp_MP->setVolume(0);
    connect(temp_MP,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
}

MUSIC_info music_analysis::analyse_music(QString path)
{
    //初始化
    m_music_info.mic_songer = "";
    m_music_info.mic_name = "";
    m_music_info.mic_time = "";
    m_music_info.mic_album = "";
    m_music_info.mic_path = "";
    m_music_info.pic_flag = "0";
    m_music_info.pic_type = "";
    m_music_info.pic_path = "";

    //目前只支持mp3IDV2解析
    QFile file(path);
    bool isok=file.open(QIODevice::ReadOnly);
    TAB_info tab_info;
    qint64 head_size=0;       //头部大小
    qint64 file_seek=0;       //文件指针
    quint64 len;
    if(isok==false)
    {
        qDebug()<<"open error";
        file.close();
        MUSIC_info falseinfo;
        return falseinfo;
    }
    //文件打开成功
    m_music_info.mic_path=path;                         //记录mp3文件的路径
    file.read((char*)&tab_info,sizeof(tab_info));
    file_seek=file_seek+10;
    //判断是否为mp3的IDV2格式
    if(QString(tab_info.format)!="ID3\u0003"||(int)tab_info.version !=3)
    {
        qDebug()<<"mp3 is not ID3V2 error";
        MUSIC_info falseinfo;
        return falseinfo;
    }

    head_size=(tab_info.header_size[0]&0xff)<<21 |
              (tab_info.header_size[1]&0xff)<<14 |
              (tab_info.header_size[2]&0xff)<<7  |
              (tab_info.header_size[3]&0xff);   //每8位只用前7位，第8位无效恒为0;

    HEAD_info head_info;
    quint32 size;

    while(file_seek<head_size)
    {
        //读取头部信息
        len=file.read((char*)&head_info,sizeof(head_info));
        file_seek=file_seek+len;
        size=(head_info.Size[0]&0xff) <<24|(head_info.Size[1]&0xff)<<16|(head_info.Size[2]&0xff)<<8|(head_info.Size[3]&0xff);

        if(QString(head_info.FrameID)=="TIT2")        //歌曲名字
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString name = codec->toUnicode(all.toLocal8Bit().data());
            m_music_info.mic_name=name;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }

        if(QString(head_info.FrameID)=="TPE1")        //歌手
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString author = codec->toUnicode(all.toLocal8Bit().data());
            m_music_info.mic_songer=author;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }

        if(QString(head_info.FrameID)=="TALB")        //专辑
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString album = codec->toUnicode(all.toLocal8Bit().data());
            m_music_info.mic_album=album;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }


        file_seek=file_seek+size;
        file.seek(file_seek);
    }
    file.close();
    return m_music_info;
}

void music_analysis::onDurationChanged(qint64 duration)
{
    if(duration!=0){
            //qDebug()<<"信号";
            int secs = duration/1000; //全部秒数
            int mins = secs/60;//分
            secs = secs % 60;//秒
            QString durationTime = QString::asprintf("%2d:%2d",mins,secs);
            m_music_info.mic_time=QString(durationTime);
    }
}



MUSIC_info &MUSIC_info::operator=(const MUSIC_info &info1)
{
    // 重载 operator= 运算符

    this->mic_name = info1.mic_name;
    this->mic_album = info1.mic_album;
    this->mic_path = info1.mic_path;
    this->mic_songer = info1.mic_songer;
    this->mic_time = info1.mic_time;
    this->pic_flag = info1.pic_flag;
    this->pic_path = info1.pic_path;
    this->pic_type = info1.pic_type;

    return *this;

}
