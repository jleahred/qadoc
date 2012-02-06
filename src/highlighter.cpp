#include "highlighter.h"

#include "aspell.h"
#include <iostream>




Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent), spell_config(new_aspell_config()), spell_checker(0)
{
    {
        aspell_config_replace(spell_config, "lang", "en_US");
        AspellCanHaveError * possible_err = new_aspell_speller(spell_config);
        spell_checker = 0;

        if (aspell_error_number(possible_err) != 0)
            std::cerr <<  aspell_error_message(possible_err);
        else
            spell_checker = to_aspell_speller(possible_err);
    }


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
        QTextCharFormat format;
        format.setForeground(Qt::blue);
        QFont font("monospace", 9);
        font.setUnderline(true);
        format.setFont(font);
        highlightingRules.append({QRegExp("^include::.*\\[\\] *"), format, false });
    }

     {       //  http link      ->     http://www.methods.co.nz/asciidoc/[asciidoc]
         // header 1
         QTextCharFormat format;
         //format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("http://[^\\] ]+\\]"), format, false });

     }
     {       //  http link      ->     http://www.methods.co.nz/asciidoc/
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         //format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("http://[^ ]+"), format, false });
     }

     {       //  link      ->     link:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkBlue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("link:[^\\]]+\\]"), format, false });
     }
     {       //  link      ->     link:eadoc:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(QColor(Qt::red).lighter(160));
         QFont font("monospace", 8);
         font.setUnderline(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("link:eadoc:[^\\]]+\\]"), format, false });
     }
     {       //  link      ->     link:adoc:prueba[caption]
         // header 1
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(QColor(Qt::blue).lighter(160));
         QFont font("monospace", 8);
         font.setUnderline(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("link:adoc:[^\\]]+\\]"), format, false });
     }

     {  //  link description bigger
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setUnderline(true);
         font.setBold(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("\\[.*\\]"), format, false });
     }



    {
        // header 1
        HighlightingRule rule;
        QTextCharFormat format;
        format.setForeground(Qt::blue);
        format.setFont(QFont("monospace", 23));
        rule.pattern = QRegExp("^= .*$");
        highlightingRules.append({QRegExp("^= .*$"), format, true });
    }
    {
         // header 2
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 18));
         highlightingRules.append({QRegExp("^== .*$"), format, true });
     }
     {
         // header 3
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 14));
         highlightingRules.append({QRegExp("^=== .*$"), format, true });
     }

     {
         // header 4
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         format.setFont(QFont("monospace", 11));
         highlightingRules.append({QRegExp("^==== .*$"), format, true });
     }

     {
         // bold
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("(^| )\\*[A-Za-z0-9][^\\*]*[A-Za-z0-9]\\*($| )"), format, true });
     }

     {
         // italic
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setItalic(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("(^| )\\_[A-Za-z0-9][^\\*]*[A-Za-z0-9]\\_($| )"), format, true });
     }


     {
         // comment line
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::gray);
         QFont font("monospace", 9);
         font.setItalic(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("^//.*$"), format, true });
     }

     {
         // filter
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkRed);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("^\\[.*\\] *$"), format, true });
     }

     {
         // labeled list  ::
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::darkBlue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("^.*::"), format, true });
     }

     {
         // labeled list  ;;
         HighlightingRule rule;
         QTextCharFormat format;
         format.setForeground(Qt::blue);
         QFont font("monospace", 9);
         font.setBold(true);
         format.setFont(font);
         highlightingRules.append({QRegExp("^.*;;"), format, true });
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
         highlightingRules.append({QRegExp("<<.+>>"), format, false });
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
         highlightingRules.append({QRegExp("^\\[\\[.+\\]\\] *$"), format, true });
     }

     //spellCheckFormat.setUnderlineColor(QColor(Qt::red));
     //spellCheckFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

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

Highlighter::~Highlighter()
{
    delete_aspell_speller(spell_checker);
    delete  spell_config;
}


 void Highlighter::highlightBlock(const QString &text)
 {
     setCurrentBlockState(0);

     if (currentBlockState() == 0)
         spellCheck(text);

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
            spellCheck(text.mid(startIndex, commentLength));
         }
     }



     foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
             if(rule.check_spelling)
                 spellCheck(text.mid(index, length+1));
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
            spellCheck(text.mid(startIndex, commentLength));
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
            spellCheck(text.mid(startIndex, commentLength));
         }
     }
}

bool   Highlighter::checkWord(const QString& word)
{
    if(spell_checker)
        return  aspell_speller_check(spell_checker, word.toStdString().c_str(), -1);
    else
        return true;
}

 void Highlighter::spellCheck(const QString &text)
 {
     // split text into words
     QString str = text.simplified();
     if (!str.isEmpty()) {
             QStringList Checkliste = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
                             QString::SkipEmptyParts);
             int l,number;
             // check all words
             for (int i=0; i<Checkliste.size(); ++i) {
                     str = Checkliste.at(i);
                     if (str.length()>1 &&!str.startsWith('\\'))
                     {
                             if (!checkWord(str)) {
                                     number = text.count(QRegExp("\\b" + str + "\\b"));
                                     l=-1;
                                     // underline all incorrect occurences of misspelled word
                                     for (int j=0;j < number; ++j)
                                     {
                                             l = text.indexOf(QRegExp("\\b" + str + "\\b"),l+1);
                                             if (l>=0)
                                             {
                                                 QTextCharFormat  current_format =  this->format(l);
                                                 current_format.setUnderlineColor(QColor(Qt::red));
                                                 current_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
                                                 setFormat(l, str.length(), current_format);
                                                 //setFormat(l, str.length(), spellCheckFormat);
                                             }
                                     } // for j
                             } // if spell chek error
                     } // if str.length > 1
             } // for
     } // if str.isEmpty
 }
