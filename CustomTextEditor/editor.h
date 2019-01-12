#ifndef EDITOR_H
#define EDITOR_H

#include "finddialog.h"
#include "documentmetrics.h"
#include <QPlainTextEdit>
#include <QFont>


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
    inline QString getFileName() const { return getFileNameFromPath(); }
    inline void setCurrentFilePath(QString newPath) { currentFilePath = newPath; }
    inline QString getCurrentFilePath() const { return currentFilePath; }
    void setFont(QString family, QFont::StyleHint styleHint, bool fixedPitch, int pointSize, int tabStopWidth);
    void launchFindDialog();
    void updateFileMetrics();
    inline bool isUnsaved() const { return fileNeedsToBeSaved; }
    void setFileNeedsToBeSaved(bool status);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void windowNeedsToBeUpdated(DocumentMetrics metrics);

public slots:
    bool find(QString query, bool caseSensitive, bool wholeWords);
    void replace(QString what, QString with, bool caseSensitive, bool wholeWords);
    void replaceAll(QString what, QString with, bool caseSensitive, bool wholeWords);
    void on_textChanged();

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rectToBeRedrawn,
                              int numPixelsScrolledVertically);

private:
    QString getFileNameFromPath() const;
    void updateSearchHistory(int oldCharCount, int newCharCount);

    DocumentMetrics metrics;
    QString currentFilePath;
    bool fileNeedsToBeSaved;
    QFont font;
    FindDialog *findDialog;
    QWidget *lineNumberArea;
    const int lineNumberAreaPadding = 30;
    bool metricCalculationDisabled = false;
};

#endif // EDITOR_H
