#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

bool mouseCallbackBindingCmp(const mouseCallbackBinding * a, const mouseCallbackBinding * b)
{
    return (memcmp(a, b, sizeof(*a)) == 0);
}

mouseCallbackBindingList * mouseCallbackBindingListAdd(mouseCallbackBindingList * l, mouseCallbackBinding * val)
{
    mouseCallbackBindingList * nn = malloc(sizeof(*nn));
    if (!nn)
    {
        fprintf(stderr, "error allocating memory for list\n");
        exit(1);
        return l;
    }
    nn->val = val;
    nn->next = l;

    return nn;
}

mouseCallbackBindingList * mouseCallbackBindingListRm(mouseCallbackBindingList * l, const mouseCallbackBinding * val, bool (*cmp)(const mouseCallbackBinding * a, const mouseCallbackBinding * b), mouseCallbackBinding ** removedVal)
{
    mouseCallbackBindingList * n;
    mouseCallbackBindingList * nPrev = NULL;
    if (removedVal)
        *removedVal = NULL;
    for (n = l; n != NULL; n = n->next)
    {
        if (cmp(val, (n->val)))
        {
            if (!nPrev)
            {
                mouseCallbackBindingList * newList = n->next;
                if (removedVal)
                    *removedVal = n->val;
                free(n);
                return newList;
            }
            nPrev->next = n->next;
            if (removedVal)
                *removedVal = n->val;
            free(n);
            return l;
        }
    }

    return l;
}

mouseCallbackBindingList * mouseCallbackBindingListSearch(mouseCallbackBindingList * l, const mouseCallbackBinding * val, bool (*cmp)(const mouseCallbackBinding * a, const mouseCallbackBinding * b))
{
    mouseCallbackBindingList * n;
    for (n = l; n != NULL; n = n->next)
    {
        if (cmp(val, (n->val)))
        {
            return n;
        }
    }

    return NULL;
}
