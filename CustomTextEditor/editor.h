#ifndef EDITOR_H
#define EDITOR_H
#include "finddialog.h"
#include "gotodialog.h"
#include "searchhistory.h"
#include "documentmetrics.h"
#include "language.h"
#include "highlighter.h"
#include <QPlainTextEdit>
#include <QFont>
#include <QMessageBox>


using namespace ProgrammingLanguage;


/* Disclaimer: the code for painting the editor line numbers was not written by me.
 * I only changed some of the variable names and code to make things clearer,
 * but most of those functions are from this official Qt tutorial on line numbering:
 *
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 *
 * All other code is my own.
 */


class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    ~Editor() override;
    void reset();

    inline QString getFileName() { return getFileNameFromPath(); }
    void setCurrentFilePath(QString newPath);
    inline QString getCurrentFilePath() const { return currentFilePath; }
    inline void setProgrammingLanguage(Language language);
    inline Language getProgrammingLanguage() const { return programmingLanguage; }
    inline bool isUntitled() const { return fileIsUntitled; }

    inline DocumentMetrics getDocumentMetrics() const { return metrics; }
    QFont getFont() { return font; }
    void setFont(QFont newFont, QFont::StyleHint styleHint, bool fixedPitch, int tabStopWidth);
    void updateFileMetrics();

    inline bool isUnsaved() const { return document()->isModified(); }
    void setModifiedState(bool modified) { document()->setModified(modified); }

    void formatSubtext(int startIndex, int endIndex, QTextCharFormat format, bool unformatAllFirst = false);
    void toggleAutoIndent(bool autoIndent);
    bool textIsAutoIndented() const { return autoIndentEnabled; }
    void toggleWrapMode(bool wrap);
    bool textIsWrapped() const { return lineWrapMode == LineWrapMode::WidgetWidth; }

    inline bool redoAvailable() const { return canRedo; }
    inline bool undoAvailable() const { return canUndo; }

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth();

    void setLineWrapMode(LineWrapMode lineWrapMode);

    const static int DEFAULT_FONT_SIZE = 10;
    const static int NUM_CHARS_FOR_TAB = 5;

    bool autoIndentEnabled = true;
    LineWrapMode lineWrapMode = Editor::LineWrapMode::NoWrap;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void findResultReady(QString message);
    void gotoResultReady(QString message);
    void columnCountChanged(int col);
    void windowNeedsToBeUpdated(DocumentMetrics metrics);

public slots:
    bool find(QString query, bool caseSensitive, bool wholeWords);
    void replace(QString what, QString with, bool caseSensitive, bool wholeWords);
    void replaceAll(QString what, QString with, bool caseSensitive, bool wholeWords);
    void goTo(int line);

private slots:
    void on_textChanged();
    void updateLineNumberAreaWidth();
    void on_cursorPositionChanged();

    void redrawLineNumberArea(const QRect &rectToBeRedrawn, int numPixelsScrolledVertically);

    void setUndoAvailable(bool available) { canUndo = available; }
    void setRedoAvailable(bool available) { canRedo = available; }

private:
    Highlighter *generateHighlighterFor(Language language);
    QString getFileNameFromPath();
    QTextDocument::FindFlags getSearchOptionsFromFlags(bool caseSensitive, bool wholeWords);
    bool handleKeyPress(QObject* obj, QEvent* event, int key);
    void moveCursorTo(int positionInText);

    void highlightCurrentLine();
    void updateAndEmitColumnCount();

    int indentationLevelOfCurrentLine();
    void moveCursorToStartOfCurrentLine();
    void insertTabs(int numTabs);
    void indentSelection(QTextDocumentFragment selection);

    void writeSetting(const QString KEY, QVariant VAL) const;
    void writeSettings();
    void readSettings();

    Language programmingLanguage;
    Highlighter *syntaxHighlighter;
    const static QColor LINE_COLOR;

    DocumentMetrics metrics;
    QString currentFilePath;
    bool fileIsUntitled = true;

    QFont font;
    QTextCharFormat defaultCharFormat;
    SearchHistory searchHistory;

    QWidget *lineNumberArea;
    const int lineNumberAreaPadding = 30;

    bool metricCalculationEnabled = true;
    bool canRedo = false;
    bool canUndo = false;

    const QString AUTO_INDENT_KEY = "auto_indent";
    const QString LINE_WRAP_KEY = "line_wrap";
};

#endif // EDITOR_H
