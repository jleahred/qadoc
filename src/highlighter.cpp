#include "highlighter.h"






 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     //HighlightingRule rule;

    //  tokens
     /*
     {
         predefinedVars.setForeground(Qt::darkCyan);
         QStringList keywordPatterns;
         keywordPatterns << "__any__" << "__isalpha__"
                         << "__islower__" << "__isupper__"
                         << "__isdigit__" << "__isalnum__"
                         << "__endl__" << "__isspace__"
                         << "__isspace*__" << "__isspace+__"
                         << "__space_tab__"
                         ;
         foreach (const QString &pattern, keywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = predefinedVars;
             highlightingRules.append(rule);
         }
    }


     {
         keywordFormat.setForeground(Qt::darkRed);
         keywordFormat.setFontWeight(QFont::Bold);
         QStringList keywordPatterns;
         keywordPatterns << " ::= " << " ##transf2->";

            foreach (const QString &pattern, keywordPatterns) {
                 rule.pattern = QRegExp(pattern);
                 rule.format = keywordFormat;
                 highlightingRules.append(rule);
             }
        }
    */




    {       //  link 1     ->         include::including_section.adoc[]
        // header 1
        HighlightingRule rule;
        QTextCharFormat format;
        format.setForeground(Qt::blue);
        QFont font("monospace", 9);
        font.setUnderline(true);
        format.setFont(font);
        rule.pattern = QRegExp("^include::.*\\[\\] *");
        rule.format = format;
        highlightingRules.append(rule);
    }

     {       //  http link      ->     http://www.methods.co.nz/asciidoc/[asciidoc]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         //format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("http://[^\\] ]+\\]");
         rule.format = format;
         highlightingRules.append(rule);
     }
     {       //  http link      ->     http://www.methods.co.nz/asciidoc/
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         //format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("http://[^ ]+");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {       //  link      ->     link:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkBlue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("link:[^\\]]+\\]");
         rule.format = format;
         highlightingRules.append(rule);
     }
     {       //  link      ->     link:eadoc:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(QColor(Qt::red).lighter(160));
         QFont font("monospace", 8);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("link:eadoc:[^\\]]+\\]");
         rule.format = format;
         highlightingRules.append(rule);
     }
     {       //  link      ->     link:adoc:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(QColor(Qt::blue).lighter(160));
         QFont font("monospace", 8);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("link:adoc:[^\\]]+\\]");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {  //  link description bigger
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         font.setBold(true);
         format.setFont(font);
         rule.pattern = QRegExp("\\[.*\\]");
         rule.format = format;
         highlightingRules.append(rule);

     }



    {
        // header 1
        HighlightingRule rule;
        QTextCharFormat format;
        format.setForeground(Qt::blue);
        format.setFont(QFont("monospace", 23));
        rule.pattern = QRegExp("^= .*$");
        rule.format = format;
        highlightingRules.append(rule);
    }
    {
         // header 2
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 18));
         rule.pattern = QRegExp("^== .*$");
         rule.format = format;
         highlightingRules.append(rule);
     }
     {
         // header 3
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 14));
         rule.pattern = QRegExp("^=== .*$");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // header 4
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 11));
         rule.pattern = QRegExp("^==== .*$");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // bold
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         rule.pattern = QRegExp("(^| )\\*[A-Za-z0-9][^\\*]*[A-Za-z0-9]\\*($| )");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // italic
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setItalic(true);
         format.setFont(font);
         rule.pattern = QRegExp("(^| )\\_[A-Za-z0-9][^\\*]*[A-Za-z0-9]\\_($| )");
         rule.format = format;
         highlightingRules.append(rule);
     }


     {
         // comment line
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::gray);
         QFont font("monospace", 9);
         font.setItalic(true);
         format.setFont(font);
         rule.pattern = QRegExp("^//.*$");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // filter
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkRed);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         rule.pattern = QRegExp("^\\[.*\\] *$");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // labeled list  ::
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkBlue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         rule.pattern = QRegExp("^.*::");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {
         // labeled list  ;;
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         rule.pattern = QRegExp("^.*;;");
         rule.format = format;
         highlightingRules.append(rule);
     }


     {       //  internal link      ->     <<destination, text to show>>
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setBackground(QColor::fromRgb(240, 240, 240, 255));
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("<<.+>>");
         rule.format = format;
         highlightingRules.append(rule);
     }

     {       //  mark to receive an internal link      ->     [[text]]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setBackground(QColor::fromRgb(235, 235, 235, 255));
         QFont font("monospace", 9);
         font.setBold(true);
         //font.setUnderline(true);
         format.setFont(font);
         rule.pattern = QRegExp("^\\[\\[.+\\]\\] *$");
         rule.format = format;
         highlightingRules.append(rule);
     }

     /*
     {
         // comment block
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::black);
         format.setBackground(Qt::lightGray);
         QFont font("monospace", 9);
         font.setItalic(true);
         format.setFont(font);
         rule.pattern = QRegExp("//+(a|b)+");
         rule.format = format;
         highlightingRules.append(rule);
     }
*/

     /*
     expandVar.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\$\\([^\\)]+\\)");
     rule.format = expandVar;
     highlightingRules.append(rule);

     {
         expandPredefVar.setForeground(Qt::darkGreen);
         QStringList keywordPatterns;
         keywordPatterns << "\\$\\(__endl__\\)" << "\\$\\(__nothing__\\)"
                         << "\\$\\(__space__\\)" << "\\$\\(__counter__\\)"
                         ;
         foreach (const QString &pattern, keywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = expandPredefVar;
             highlightingRules.append(rule);
         }

         literal.setForeground(Qt::magenta);
         rule.pattern = QRegExp("'[^\']*'");
         rule.format = literal;
         highlightingRules.append(rule);

         singleLineCommentFormat.setForeground(Qt::blue);
         singleLineCommentFormat.setFontItalic(true);
         rule.pattern = QRegExp("^//[^\n]*");
         rule.format = singleLineCommentFormat;
         highlightingRules.append(rule);
     }
     */
     /*
     expandPredefVar.setFontItalic(true);
     expandPredefVar.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\$\\(__[^\\)]+__\\)");
     rule.format = expandPredefVar;
     highlightingRules.append(rule);
    */

 }


 void Highlighter::highlightBlock(const QString &text)
 {
     setCurrentBlockState(0);


     {  //  sidebar block
         QTextCharFormat multiLineCommentFormat;
         multiLineCommentFormat.setBackground(QColor::fromRgb(245, 245, 190, 255));

         QRegExp startExpression("^\\*\\*\\*\\*+$");
         QRegExp endExpression("^\\*\\*\\*\\*+$");

         int startIndex = 0;
         int startIndexLengh = 0;
         if (previousBlockState() != 3)
         {
             startIndex = text.indexOf(startExpression);
             startIndexLengh = startExpression.matchedLength();
         }

         while (startIndex >= 0) {
            int endIndex = text.indexOf(endExpression, startIndex+startIndexLengh);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(3);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + endExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(startExpression,
                                      startIndex + commentLength);
         }
     }



     foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }


     if (currentBlockState() == 0)
     {  //  block comment
         QTextCharFormat multiLineCommentFormat;
         multiLineCommentFormat.setForeground(Qt::gray);
         //multiLineCommentFormat.setBackground(QColor::fromRgb(223, 223, 223, 255));
         multiLineCommentFormat.setFontItalic(true);

         QRegExp startExpression("^////+$");
         QRegExp endExpression("^////+$");

         //setCurrentBlockState(0);

         int startIndex = 0;
         int startIndexLengh = 0;
         if (previousBlockState() != 1)
         {
             startIndex = text.indexOf(startExpression);
             startIndexLengh = startExpression.matchedLength();
         }

         while (startIndex >= 0) {
            int endIndex = text.indexOf(endExpression, startIndex+startIndexLengh);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + endExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(startExpression,
                                      startIndex + commentLength);
         }
     }

     if (currentBlockState() == 0)
     {  //  listing block
         QTextCharFormat multiLineCommentFormat;
         multiLineCommentFormat.setForeground(Qt::blue);
         multiLineCommentFormat.setBackground(QColor::fromRgb(223, 223, 223, 255));

         QRegExp startExpression("^\\-\\-\\-\\-+$");
         QRegExp endExpression("^\\-\\-\\-\\-+$");

         //setCurrentBlockState(0);

         int startIndex = 0;
         int startIndexLengh = 0;
         if (previousBlockState() != 2)
         {
             startIndex = text.indexOf(startExpression);
             startIndexLengh = startExpression.matchedLength();
         }

         while (startIndex >= 0) {
            int endIndex = text.indexOf(endExpression, startIndex+startIndexLengh);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(2);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + endExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(startExpression,
                                      startIndex + commentLength);
         }
     }


 }
