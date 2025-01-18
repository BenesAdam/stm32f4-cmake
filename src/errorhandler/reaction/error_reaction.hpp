#ifndef ERROR_REACTION_HPP
#define ERROR_REACTION_HPP

#include "types.h"

class cErrorReaction
{
public:
  cErrorReaction(void) {}
  virtual void ErrorReaction(const ui16 arg_errorNumber) = 0;
};

#endif
