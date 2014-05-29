/*
 *拼音转换类，输入汉字得到拼音，如果输入的是英文就仍然输出英文。空格会被过滤掉（不然转换很多问题），默认符号也会过滤掉。
 *但是你可以手动选择不过滤，但是中文标点是不被识别为符号的，而是识别为未知拼音的，你可以手动选择不过滤
*/

#ifndef PINYINCONVERT_H
#define PINYINCONVERT_H

#include <QString>

class PinYinConvert
{
public:
    PinYinConvert();
    ~PinYinConvert();

    QString convertToPY(QString str,bool m_blnSimaple=false, bool polyphone_support=false,
                        bool m_blnFirstBig=false,bool m_blnAllBiG=false,bool m_LetterEnd=false,
                        bool m_unknowSkip=true,bool m_filterPunc=true);

protected:
    void pinyin_utf8(char* inbuf,char* outbuf, bool m_blnSimaple, bool polyphone_support,
                     bool m_blnFirstBig,bool m_blnAllBiG,bool m_LetterEnd,
                     bool m_unknowSkip,bool m_filterPunc);
    void pinyin_gb2312(char* inbuf,char* outbuf, bool m_blnSimaple,bool polyphone_support ,
                       bool m_blnFirstBig,bool m_blnAllBiG,bool m_LetterEnd,
                       bool m_unknowSkip,bool m_filterPunc);
    int findLetter(int nCode, char* strValue);
    int is_utf8_string(char *utf);
    bool safeAddToOutbuf(char* outbuf,int &iOutbuf,char* pinyinValue,int iPinyinValue);
    static const char *_pinyin_table_[20902];

private:
    char* outbuf;
    char * inbuf;
};

/*
    bool m_blnSimaple;    //是否简拼
    bool polyphone_supporte;     //多音字支持
    bool m_blnFirstBig;   //拼音第一个字母是否大写
    bool m_blnAllBiG=;     //拼音是否大写
    bool m_LetterEnd;     //每个拼音后是否用分隔符
    bool m_unknowSkip;        //未知拼音的字是否略过
    bool m_filterPunc;     //是否过滤标点
    */

#endif // PINYINCONVERT_H
