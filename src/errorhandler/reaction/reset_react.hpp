#ifndef RESET_REACT_HPP
#define RESET_REACT_HPP

#include "error_reaction.hpp"

class cResetReact : public cErrorReaction
{
public:
  cResetReact(void);
  void ErrorReaction(const ui16 arg_errorNumber);
};

#endif
