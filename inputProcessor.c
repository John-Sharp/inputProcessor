#include "inputProcessor.h"

bool activeStates[GS_NUM_STATES];
keyStateBinding bindings[MAX_BINDINGS];
juint numberOfBindings;

#include "listHeaders/mouseCallbackBindingList.h"
#include "listCode/mouseCallbackBindingList.inc"
mouseCallbackBindingList * mouseCBList;

void inputProcessorInit()
{
    memset(activeStates, 0, GS_NUM_STATES); 
    numberOfBindings = 0;

    mouseCBList = NULL;

    return;
}

int bindingKeyCompar(const void *vKey, const void *vMember)
{
   const SDL_Keycode *key = vKey;
   const keyStateBinding *member = vMember;

   if (*key < member->k)
       return -1;
   if (*key > member->k)
       return 1;
   return 0;
}

keyStateBinding *searchBindingsByKey(SDL_Keycode k)
{
    keyStateBinding *b = bsearch(
            &k,
            bindings,
            numberOfBindings,
            sizeof(keyStateBinding),
            bindingKeyCompar);

    if (b == NULL)
        return NULL;

    keyStateBinding *a;

    while (b->k == k) {
        a = b;
        b -= 1;
    }

    return a;
}

bool addBinding(keyStateBinding *binding)
{
    if (binding->s >= GS_NUM_STATES) 
    {
        fprintf(stderr, "State exceeds GS_NUM_STATES\n");
        return false;
    }

    if (numberOfBindings + 1 >=  MAX_BINDINGS)
    {
        fprintf(stderr, "Number of bindings exceeds MAX_BINDINGS\n");
        return false;
    }

    keyStateBinding *extant_binding = bindings;
    int i = 0;

    while (binding->k > extant_binding->k && i < numberOfBindings)
    {
        extant_binding += 1;
        i += 1;
    }

    int num_states_to_move = (numberOfBindings - i);
    
    memmove(
            &(bindings[i + 1]),
            &bindings[i],
            num_states_to_move * sizeof(keyStateBinding));

    bindings[i] = *binding;
    numberOfBindings += 1;
    return true;
}

bool rm_binding(keyStateBinding *binding)
{
    keyStateBinding *b = searchBindingsByKey(binding->k);
    keyStateBinding *b_end = 
        bindings + numberOfBindings * sizeof(keyStateBinding);

    if (b == NULL)
        return true;

    while (b->k == binding->k && b != b_end) {
        if (b->s == binding->s && b->t == binding->t) {
            int i = (b - bindings) / sizeof(keyStateBinding);
            int num_states_to_move = (numberOfBindings - 1 - i);

            memmove(
                    b,
                    b + 1,
                    num_states_to_move * sizeof(keyStateBinding));
            numberOfBindings -= 1;
            b_end = 
                bindings + numberOfBindings * sizeof(keyStateBinding);
            b--;
        }
        b++;
    }
    return true;
}

void processKeyboardEvent(SDL_KeyboardEvent *event)
{
    SDL_Keycode key = event->keysym.sym;

    keyStateBinding *b = searchBindingsByKey(key);
    keyStateBinding *bEnd = 
        bindings + numberOfBindings * sizeof(keyStateBinding);

    if (b == NULL)
        return;

    while (b->k == key && b != bEnd) {
        if (event->type == SDL_KEYDOWN) {
            if (b->t == BINDING_ATOMIC && isStateActive(b->s)) {
                deactivateState(b->s);
                b++;
                continue;
            }
            activateState(b->s);
            b++;
            continue;
        }

        if (event->type == SDL_KEYUP && b->t == BINDING_CONTINUOUS)
            deactivateState(b->s);
        b++;
    }
}

bool typeButtonCmp(const mouseCallbackBinding * a, const mouseCallbackBinding * b)
{
    if (a->type == b->type && a->button == b->button)
        return true;
    return false;
}

void processMouseButtonEvent(SDL_MouseButtonEvent *event)
{
    mouseCallbackBinding searchCandidate = {.type = event->type, .button = event->button};
    mouseCallbackBindingList * searchList = mouseCBList;

    while (searchList)
    {
        mouseCallbackBindingList * foundBinding = mouseCallbackBindingListSearch(
                searchList, &searchCandidate, typeButtonCmp);

        if (!foundBinding)
            break;

        // call the callback with its owner
        foundBinding->val->callback(event->x, event->y, foundBinding->val->owner);
        searchList = foundBinding->next;
    }
}

void processInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                activeStates[GS_QUIT] = true;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                processKeyboardEvent(&event.key);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                processMouseButtonEvent(&event.button);
                break;
        }
    }
}

void activateState(GAME_STATE state)
{
    activeStates[state] = true;
}

void deactivateState(GAME_STATE state)
{
    activeStates[state] = false;
}

bool isStateActive(GAME_STATE state)
{
    return activeStates[state];
}

bool addMouseCallback(const mouseCallbackBinding * binding)
{
    mouseCallbackBinding * bindingCopy = malloc(sizeof(*bindingCopy));
    if (!bindingCopy)
    {
        return false;
    }
    *bindingCopy = *binding;

    mouseCBList = mouseCallbackBindingListAdd(mouseCBList, bindingCopy);
    return true;
}

bool rmMouseCallback(const mouseCallbackBinding * binding)
{
    mouseCallbackBinding * bindingCopy;

    mouseCBList = mouseCallbackBindingListRm(mouseCBList, binding, mouseCallbackBindingCmp, &bindingCopy);

    if (bindingCopy)
        free(bindingCopy);

    return true;
}
