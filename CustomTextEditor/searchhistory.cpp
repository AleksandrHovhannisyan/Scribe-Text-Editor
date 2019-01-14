#include "searchhistory.h"


/* TODO document
 */
void SearchHistory::add(QString term, int cursorPositionBeforeFirstSearch, int firstFoundAt)
{
    // If this search term breaks the current search "chain", clear the history
    if(!previouslyFound(term))
    {
        searchHistory.clear();
    }

    QPair<int,int> locations;
    locations.first = cursorPositionBeforeFirstSearch;
    locations.second = firstFoundAt;
    searchHistory.insert(term, locations);
}
