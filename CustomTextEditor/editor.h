#ifndef EDITOR_H
#define EDITOR_H
#include "finddialog.h"
#include "gotodialog.h"
#include "searchhistory.h"
#include "documentmetrics.h"
#include "language.h"
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
    inline void setProgrammingLanguage(Language language) { programmingLanguage = language; }
    inline Language getProgrammingLanguage() const { return programmingLanguage; }
    inline bool isUntitled() const { return fileIsUntitled; }

    inline DocumentMetrics getDocumentMetrics() const { return metrics; }
    void launchFontDialog();
    void setFont(QString family, QFont::StyleHint styleHint, bool fixedPitch, int pointSize, int tabStopWidth);
    void updateFileMetrics();

    inline bool isUnsaved() const { return document()->isModified(); }
    void setModifiedState(bool modified) { document()->setModified(modified); }

    void formatSubtext(int startIndex, int endIndex, QTextCharFormat format, bool unformatAllFirst = false);
    void toggleAutoIndent(bool autoIndent) { autoIndentEnabled = autoIndent; }
    void toggleWrapMode(bool wrap) { wrap ? setLineWrapMode(LineWrapMode::WidgetWidth) : setLineWrapMode(LineWrapMode::NoWrap); }

    inline bool redoAvailable() const { return canRedo; }
    inline bool undoAvailable() const { return canUndo; }

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth();

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
    void updateLineNumberArea(const QRect &rectToBeRedrawn, int numPixelsScrolledVertically);

    void setUndoAvailable(bool available) { canUndo = available; }
    void setRedoAvailable(bool available) { canRedo = available; }

private:
    QString getFileNameFromPath();
    QTextDocument::FindFlags getSearchOptionsFromFlags(bool caseSensitive, bool wholeWords);
    bool handleKeyPress(QObject* obj, QEvent* event, int key);
    void moveCursorTo(int positionInText);

    int indentationLevelOfCurrentLine();
    void moveCursorToStartOfCurrentLine();
    void insertTabs(int numTabs);

    Language programmingLanguage;
    DocumentMetrics metrics;
    QString currentFilePath;
    bool fileIsUntitled = true;

    QFont font;
    QTextCharFormat defaultCharFormat;
    SearchHistory searchHistory;

    QWidget *lineNumberArea;
    const int lineNumberAreaPadding = 30;

    bool metricCalculationEnabled = true;
    bool autoIndentEnabled = true;
    bool canRedo = false;
    bool canUndo = false;
};

#endif // EDITOR_H
