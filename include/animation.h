#ifndef ANIMATION_H
#define ANIMATION_H

#include "game.h"


void drawCharacterFrame(Position pos, AnimationState state);
void clearCharacterFrame(Position pos);
void handleAndAnimateCharacterMovement(Position *pos, int input);

#endif // ANIMATION_H
