#include "javahighlighter.h"


JavaHighlighter::JavaHighlighter(QTextDocument *parent) : Highlighter (parent)
{
    QStringList keywords;
    keywords << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b"
             << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b"
             << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextends\\b"
             << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b"
             << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b" << "\\bint\\b" << "\\binterface\\b"
             << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b"
             << "\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b" << "\\bstrictfp\\b" << "\\bsuper\\b"
             << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b"
             << "\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b";

    addKeywords(keywords);
    addRule(QRegularExpression("\\b[A-Z_][a-zA-Z0-9_]*\\b"), classFormat);
    addRule(QRegularExpression("(\".*\")|('\\\\.')|('.{0,1}')"), quoteFormat);
    addRule(QRegularExpression("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()"), functionFormat);
    addRule(QRegularExpression("//.*"), inlineCommentFormat);

    blockCommentStart = QRegularExpression("/\\*");
    blockCommentEnd = QRegularExpression("\\*/");

    codeBlockStart = '{';
    codeBlockEnd = '}';
}
