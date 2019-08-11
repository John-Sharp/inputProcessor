#include <stdbool.h>

// typedef struct mouseCallbackBindingList mouseCallbackBindingList;
typedef struct mouseCallbackBindingList mouseCallbackBindingList;
struct mouseCallbackBindingList {
    mouseCallbackBinding * val;
    mouseCallbackBindingList * next;    
};

bool mouseCallbackBindingCmp(const mouseCallbackBinding * a, const mouseCallbackBinding * b);

// mouseCallbackBindingListAdd: adds element to head of list
mouseCallbackBindingList * mouseCallbackBindingListAdd(mouseCallbackBindingList * l, mouseCallbackBinding * val);

// mouseCallbackBindingListRmremove: removes element from list
mouseCallbackBindingList * mouseCallbackBindingListRm(mouseCallbackBindingList * l, const mouseCallbackBinding * val, bool (*cmp)(const mouseCallbackBinding * a, const mouseCallbackBinding * b), mouseCallbackBinding ** removedVal);

mouseCallbackBindingList * mouseCallbackBindingListSearch(mouseCallbackBindingList * l, const mouseCallbackBinding * val, bool (*cmp)(const mouseCallbackBinding * a, const mouseCallbackBinding * b));
