#ifndef __HIGHLIGHTER_H
#define __HIGHLIGHTER_H

#include <QSyntaxHighlighter>



class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

 public:
    Highlighter(QTextDocument *parent = 0);

 protected:
     void highlightBlock(const QString &text);

 private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

/*
     QTextCharFormat header1;
     QTextCharFormat header2;
     QTextCharFormat header3;
     QTextCharFormat comment;
*/
};

#endif // HIGHLIGHTER_H
