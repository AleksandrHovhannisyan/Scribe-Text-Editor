#ifndef SEARCHHISTORY_H
#define SEARCHHISTORY_H
#include <QMap>
#include <QPair>

class SearchHistory
{
public:
    SearchHistory(){}
    inline void clear() { searchHistory.clear(); }
    void add(QString term, int cursorPositionBeforeFirstSearch, int firstFoundAt);
    inline bool previouslyFound(QString term) { return searchHistory.find(term) != searchHistory.end(); }
    inline int cursorPositionBeforeFirstSearchFor(QString term) { return searchHistory[term].first; }
    inline int firstFoundAt(QString term){ return searchHistory[term].second; }
    inline QMap<QString, QPair<int, int>> *getSearchHistory() { return &searchHistory; }

private:
    // This will only ever contain one entry at a time, for the current search "chain"
    QMap<QString, QPair<int, int>> searchHistory;
};

#endif // SEARCHHISTORY_H
