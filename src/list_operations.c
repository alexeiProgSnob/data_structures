#include "list_operations.h"
#include "list.h"
#include "listInternal.h"

static void BubbleUp(ListItr _itrNext, ListItr _itrEnd, LessFunction _less);
static void swap(ListItr _itrNext, void* _elementBig, void* _elementSmall);

ListItr ListItr_FindFirst(ListItr _begin, ListItr _end,
                          PredicateFunction _predicate, void* _context) {
    ListItr itr = _begin;

    if (_predicate == NULL || _begin == NULL || _end == NULL) {
        return NULL;
    }

    itr =
        ListItr_ForEach(_begin, _end, (ListActionFunction)_predicate, _context);

    return itr;
}

size_t ListItr_CountIf(ListItr _begin, ListItr _end,
                       PredicateFunction _predicate, void* _context) {
    ListItr itrEnd = _end;
    ListItr itr = _begin;
    void* elemnt;
    size_t count = 0;
    if (_predicate == NULL || _begin == NULL || _end == NULL) {
        return 0;
    }

    while (ListItr_Equals(itr, itrEnd) == 0) {
        elemnt = ListItr_Get(itr);
        if (_predicate(elemnt, _context) != 0) {
            ++count;
        }
        itr = ListItr_Next(itr);
    }
    return count;
}

ListItr ListItr_ForEach(ListItr _begin, ListItr _end,
                        ListActionFunction _action, void* _context) {
    ListItr itr = _begin;
    void* element;
    if (_action == NULL || _begin == NULL || _end == NULL) {
        return NULL;
    }

    while (ListItr_Equals(itr, _end) == 0) {
        element = ListItr_Get(itr);
        if (_action(element, _context) == 0) {
            break;
        }
        itr = ListItr_Next(itr);
    }
    return itr;
}

void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less) {
    ListItr itrNext = _begin;
    ListItr itrEnd = ListItr_Prev(_end);
    if (_begin == NULL || _end == NULL || _less == NULL) {
        return;
    }
    while (ListItr_Equals(_begin, itrEnd) == 0) {
        itrNext = _begin;
        BubbleUp(itrNext, itrEnd, _less);
        itrEnd = ListItr_Prev(itrEnd);
    }
}

static void BubbleUp(ListItr _itrNext, ListItr _itrEnd, LessFunction _less) {
    void* elemntSmall;
    void* elemntBig;
    while (ListItr_Equals(_itrEnd, _itrNext) == 0) {
        elemntSmall = ListItr_Get(_itrNext);
        _itrNext = ListItr_Next(_itrNext);
        elemntBig = ListItr_Get(_itrNext);
        if (_less(elemntBig, elemntSmall)) {
            swap(_itrNext, elemntBig, elemntSmall);
        }
    }
}

static void swap(ListItr _itrNext, void* _elementBig, void* _elementSmall) {
    ListItr_Set(_itrNext, _elementSmall);
    ListItr_Set(ListItr_Prev(_itrNext), _elementBig);
}

ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end) {
    ListItr oldBegin = _begin;
    void* elem;
    while (ListItr_Equals(oldBegin, _end) == 0) {
        _begin = oldBegin;
        oldBegin = ListItr_Next(oldBegin);
        elem = ListItr_Remove(_begin);
        _begin = ListItr_InsertBefore(_dest, elem);
    }
    return _begin;
}

ListItr ListItr_Merge(ListItr _destBegin, ListItr _firstBegin,
                      ListItr _firstEnd, ListItr _secondBegin,
                      ListItr _secondEnd, LessFunction _less) {
    ListItr sortBegin;
    ListItr next;
    if (_destBegin == NULL || _firstBegin == NULL || _firstEnd == NULL ||
        _secondBegin == NULL || _secondEnd == NULL) {
        return NULL;
    }

    next = ListItr_Next(_firstBegin);
    sortBegin = ListItr_Splice(_destBegin, _firstBegin, next);
    ListItr_Splice(_destBegin, next, _firstEnd);
    ListItr_Splice(_destBegin, _secondBegin, _secondEnd);
    ListItr_Sort(sortBegin, _destBegin, _less);

    return _destBegin;
}

List* ListItr_Cut(ListItr _begin, ListItr _end) {
    List* desList;
    ListItr desBegin;
    if (_begin == NULL && _end == NULL) {
        return NULL;
    }

    desList = ListCreate();
    if (desList == NULL) {
        return NULL;
    }

    desBegin = ListItr_Begin(desList);
    ListItr_Splice(desBegin, _begin, _end);

    return desList;
}

List* ListItr_Unique(ListItr _begin, ListItr _end, EqualsFunction _equals) {
    ListItr nextItr = _begin;
    void* elementBegin;
    void* elementNext;
    List* desList;
    ListItr desBegin;

    if (_begin == NULL || _end == NULL || _equals == NULL) {
        return NULL;
    }

    desList = ListCreate();
    if (desList == NULL) {
        return NULL;
    }

    desBegin = ListItr_Begin(desList);

    while (ListItr_Equals(ListItr_Next(_begin), _end) == 0) {
        nextItr = ListItr_Next(_begin);

        elementBegin = ListItr_Get(_begin);
        elementNext = ListItr_Get(nextItr);

        if (_equals(elementBegin, elementNext)) {
            ListItr_Splice(desBegin, nextItr, ListItr_Next(nextItr));
        } else {
            _begin = nextItr;
        }
    }

    return desList;
}
