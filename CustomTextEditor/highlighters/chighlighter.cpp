#include "chighlighter.h"


CHighlighter::CHighlighter(QTextDocument *parent) : Highlighter(parent)
{
    QStringList keywords;
    keywords << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bchar\\b" << "\\bconst\\b"
             << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
             << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b"
             << "\\bif\\b" << "\\bint\\b" << "\\blong\\b" << "\\bregister\\b" << "\\breturn\\b"
             << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstruct\\b"
             << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
             << "\\bvolatile\\b" << "\\bwhile\\b";

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
