#ifndef RESET_REACT_HPP
#define RESET_REACT_HPP

#include "error_reaction.hpp"

class cResetReact : public cErrorReaction
{
public:
  cResetReact(void);
  void ErrorReaction(void);
};

#endif
