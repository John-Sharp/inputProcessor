#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H
#include <SDL2/SDL.h>
#include "../jTypes/jTypes.h"

typedef juint GAME_STATE;
enum
{
    GS_QUIT = 0,
    GS_NUM_STATES = 50,
    MAX_BINDINGS = 100
};

typedef enum BINDING_TYPE
{
    BINDING_ATOMIC,      // press button to turn state on,
                         // press again to switch off

    BINDING_CONTINUOUS,  // press button and state turns on,
                         // release and state turns off

    BINDING_ONE_TIME     // press button and state turns on,
                         // state must then be turned off
                         // manually
} BINDING_TYPE;

typedef struct keyStateBinding
{
    SDL_Keycode k;
    GAME_STATE s; 
    BINDING_TYPE t;
} keyStateBinding;

void inputProcessorInit();
bool addBinding(keyStateBinding *binding);
bool rmBinding(keyStateBinding *binding);
void processInput();
void activateState(GAME_STATE state);
void deactivateState(GAME_STATE state);
bool isStateActive(GAME_STATE state);

#endif
