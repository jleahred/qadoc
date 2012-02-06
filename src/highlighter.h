#ifndef __HIGHLIGHTER_H
#define __HIGHLIGHTER_H

#include <QSyntaxHighlighter>


class  AspellConfig;
class  AspellSpeller;


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

 public:
    Highlighter(QTextDocument *parent = 0);
    ~Highlighter();

 protected:
     void highlightBlock(const QString &text);

 private:
     AspellConfig * spell_config;
     AspellSpeller * spell_checker;

     struct HighlightingRule
     {
         QRegExp            pattern;
         QTextCharFormat    format;
         bool               check_spelling;
     };
     QVector<HighlightingRule> highlightingRules;

     //QTextCharFormat spellCheckFormat;

     bool   checkWord(const QString& word);
     void   spellCheck(const QString &text);
};

#endif // HIGHLIGHTER_H
