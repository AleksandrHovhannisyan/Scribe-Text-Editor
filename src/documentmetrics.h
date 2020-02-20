#ifndef DOCUMENTMETRICS_H
#define DOCUMENTMETRICS_H

struct DocumentMetrics
{
    DocumentMetrics() : wordCount(0), charCount(0), currentLine(1), totalLines(1), currentColumn(1) {}
    int wordCount;
    int charCount;
    int currentLine;
    int totalLines;
    int currentColumn;
};

#endif // DOCUMENTMETRICS_H
