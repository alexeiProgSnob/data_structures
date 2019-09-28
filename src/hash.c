#include "hash.h"
#include "list.h"
#include <stdlib.h>

#define INSERT 666
#define REMOVE 42

struct HashMap {
  List **m_lists;
  size_t m_capacity;
  HashFunction m_hashFunc;
  EqualityFunction m_keysEqualFunc;
};

typedef struct Elements {
  void *m_value;
  void *m_key;
} Elements;

typedef struct SearchStruct {
  void *m_searchKey;
  EqualityFunction m_keyEqaul;
} SearchStruct;

typedef struct ForEachStruct {
  void *m_context;
  KeyValueActionFunction m_KeyValFunc;
} ForEachStruct;

static HashMap* _InitHashMap(HashMap *_hash, List **_plists, size_t _capacity,
                            HashFunction _hashFunc,
                            EqualityFunction _keysEqualFunc);
static void _InitMapStats(Map_Stats *_stats, size_t _capacity);

static size_t _FindNextPrime(size_t _capacity);
static int _PrimeNumCheck(size_t _n);

static Elements *_CreateNewPair(const void *_key, const void *_value);
static ListItr _CalcIndexSearchKey(const HashMap *_map, const void *_key);
static int _SearchKey(void *_item, void *_context);

static int _ActionOnPair(void *_item, void *_context);

static void _DestroyList(List *_list, void (*_keyDestroy)(void *_key),
                        void (*_valDestroy)(void *_value));

static void _CheckMax(size_t *_maxChainLength, size_t _listSize);

HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc,
                        EqualityFunction _keysEqualFunc) {
  HashMap *hash;
  List **plists;
  size_t i;

  if (_hashFunc == NULL || _keysEqualFunc == NULL) {
    return NULL;
  }

  _capacity = _FindNextPrime(_capacity);
  hash = (HashMap *)malloc(sizeof(HashMap));
  if (hash == NULL) {
    return NULL;
  }

  plists = (List **)malloc(_capacity * sizeof(List *));
  if (NULL == plists) {
    free(hash);
    return NULL;
  }

  for (i = 0; i < _capacity; ++i) {
    plists[i] = ListCreate();
    if (plists[i] == NULL) {
      while (i > 0) {
        free(plists[--i]);
      }
      free(hash);
      free(plists);
      return NULL;
    }
  }

  return _InitHashMap(hash, plists, _capacity, _hashFunc, _keysEqualFunc);
}

void HashMapDestroy(HashMap **_map, void (*_keyDestroy)(void *_key),
                     void (*_valDestroy)(void *_value)) {
  size_t i;
  if (_map == NULL || *_map == NULL) {
    return;
  }

  for (i = (*_map)->m_capacity; i > 0; --i) {
    _DestroyList((*_map)->m_lists[i - 1], _keyDestroy, _valDestroy);
    ListDestroy(&((*_map)->m_lists[i - 1]), NULL);
  }
  free((*_map)->m_lists);
  free(*_map);
  *_map = NULL;
}

Map_Result HashMapRehash(HashMap *_map, size_t newCapacity) {
  List **tempList;
  newCapacity = _FindNextPrime(newCapacity);
  tempList = (List **)realloc(_map->m_lists, newCapacity * sizeof(List *));
  if (tempList == NULL) {
    return MAP_ALLOCATION_ERROR;
  }
  _map->m_lists = tempList;
  return MAP_SUCCESS;
}

Map_Result HashMapInsert(HashMap *_map, const void *_key, const void *_value) {
  ListItr itr;
  Elements *elements;

  if (_map == NULL || _value == NULL) {
    return MAP_UNINITIALIZED_ERROR;
  }

  if (_key == NULL) {
    return MAP_KEY_NULL_ERROR;
  }

  itr = _CalcIndexSearchKey(_map, _key);
  if (itr != ListItr_Next(itr)) {
    return MAP_KEY_DUPLICATE_ERROR;
  }

  elements = _CreateNewPair(_key, _value);
  if (elements == NULL) {
    return MAP_ALLOCATION_ERROR;
  }

  itr = ListItr_InsertBefore(itr, elements);
  if (itr == NULL) {
    return MAP_ALLOCATION_ERROR;
  }

  return MAP_SUCCESS;
}

Map_Result HashMapRemove(HashMap *_map, const void *_searchKey, void **_pKey,
                          void **_pValue) {
  ListItr itr;
  Elements *elements;

  if (_map == NULL || _pKey == NULL || _pValue == NULL) {
    return MAP_UNINITIALIZED_ERROR;
  }

  if (_searchKey == NULL) {
    return MAP_KEY_NULL_ERROR;
  }

  itr = _CalcIndexSearchKey(_map, _searchKey);
  if (itr == ListItr_Next(itr)) {
    return MAP_KEY_NOT_FOUND_ERROR;
  }

  elements = ListItr_Remove(itr);
  *_pValue = elements->m_value;
  *_pKey = elements->m_key;
  free(elements);
  return MAP_SUCCESS;
}

Map_Result HashMapFind(const HashMap *_map, const void *__searchKey,
                        void **_pValue) {
  ListItr itr;
  Elements *elements;

  if (_map == NULL || _pValue == NULL) {
    return MAP_UNINITIALIZED_ERROR;
  }

  if (__searchKey == NULL) {
    return MAP_KEY_NULL_ERROR;
  }

  itr = _CalcIndexSearchKey(_map, __searchKey);

  if (itr == ListItr_Next(itr)) {
    return MAP_KEY_NOT_FOUND_ERROR;
  }

  elements = ListItr_Get(itr);
  *_pValue = (void *)elements->m_value;
  return MAP_SUCCESS;
}

size_t HashMapSize(const HashMap *_map) {
  if (_map == NULL) {
    return 0;
  }
  return _map->m_capacity;
}

size_t HashMapForEach(const HashMap *_map, KeyValueActionFunction _action,
                       void *_context) {
  size_t idx;
  ListItr itr;
  ListItr begin;
  ListItr end;
  ForEachStruct sForEach;
  if (_map == NULL || _action == NULL) {
    return MAP_UNINITIALIZED_ERROR;
  }

  sForEach.m_context = _context;
  sForEach.m_KeyValFunc = _action;
  for (idx = 0; idx < _map->m_capacity; ++idx) {
    begin = ListItr_Begin(_map->m_lists[idx]);
    end = ListItr_End(_map->m_lists[idx]);
    itr = ListItr_ForEach(begin, end, _ActionOnPair, &sForEach);
    if (end != itr) {
      return idx;
    }
  }
  return idx;
}

Map_Stats HashMapGetStatistics(const HashMap *_map) {
  Map_Stats stats;
  size_t idx;
  ListItr begin;
  ListItr end;
  size_t sumAllLength = 0;
  size_t tempListSize;

  _InitMapStats(&stats, _map->m_capacity);
  for (idx = 0; idx < _map->m_capacity; ++idx) {
    begin = ListItr_Begin(_map->m_lists[idx]);
    end = ListItr_End(_map->m_lists[idx]);
    if (end != begin) {
      ++(stats.numberOfChains);
      tempListSize = ListSize(_map->m_lists[idx]);
      _CheckMax(&(stats.maxChainLength), tempListSize);
      sumAllLength += tempListSize;
    }
  }
  stats.averageChainLength = sumAllLength / (stats.numberOfChains);
  return stats;
}

static void _CheckMax(size_t *_maxChainLength, size_t _listSize) {
  if (_listSize > *_maxChainLength) {
    *_maxChainLength = _listSize;
  }
}

static int _ActionOnPair(void *_item, void *_context) {
  ForEachStruct *sForEach = _context;
  Elements *element = _item;
  return sForEach->m_KeyValFunc(element->m_key, element->m_value,
                                sForEach->m_context);
}

static Elements *_CreateNewPair(const void *_key, const void *_value) {
  Elements *elements;
  elements = (Elements *)malloc(sizeof(Elements));
  if (elements == NULL) {
    return NULL;
  }

  elements->m_key = (void *)_key;
  elements->m_value = (void *)_value;
  return elements;
}

static ListItr _CalcIndexSearchKey(const HashMap *_map, const void *_key) {
  size_t idx;
  ListItr itr;
  ListItr begin;
  ListItr end;
  SearchStruct search;

  idx = _map->m_hashFunc(_key);
  begin = ListItr_Begin(_map->m_lists[idx]);
  end = ListItr_End(_map->m_lists[idx]);

  search.m_searchKey = (void *)_key;
  search.m_keyEqaul = _map->m_keysEqualFunc;

  itr = ListItr_FindFirst(begin, end, _SearchKey, &search);

  return itr;
}

static int _SearchKey(void *_item, void *_context) {
  SearchStruct *search = _context;
  Elements *element = _item;
  return !(search->m_keyEqaul(search->m_searchKey, element->m_key));
}

static size_t _FindNextPrime(size_t _capacity) {
  while (_PrimeNumCheck(_capacity) != 0) {
    ++_capacity;
  }
  return _capacity;
}

static int _PrimeNumCheck(size_t _n) {
  size_t i;
  for (i = 2; i <= _n / 2; ++i) {
    if (_n % i == 0) {
      return 1;
    }
  }
  return 0;
}

static void _DestroyList(List *_list, void (*_keyDestroy)(void *_key),
                        void (*_valDestroy)(void *_value)) {
  ListItr begin = ListItr_Begin(_list);
  ListItr end = ListItr_End(_list);
  Elements *elements;
  while (ListItr_Equals(begin, end) == 0) {
    elements = ListItr_Remove(begin);
    if (_keyDestroy != NULL) {
      _keyDestroy(elements->m_key);
    }
    if (_valDestroy != NULL) {
      _valDestroy(elements->m_value);
    }
    free(elements);
    begin = ListItr_Begin(_list);
  }
}

static HashMap *_InitHashMap(HashMap *_hash, List **_plists, size_t _capacity,
                            HashFunction _hashFunc,
                            EqualityFunction _keysEqualFunc) {
  _hash->m_lists = _plists;
  _hash->m_capacity = _capacity;
  _hash->m_hashFunc = _hashFunc;
  _hash->m_keysEqualFunc = _keysEqualFunc;
  return _hash;
}

static void _InitMapStats(Map_Stats *_stats, size_t _capacity) {
  _stats->numberOfBuckets = _capacity;
  _stats->numberOfChains = 0;
  _stats->maxChainLength = 0;
  _stats->averageChainLength = 0;
}
