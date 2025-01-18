#ifndef NOTHING_REACT_HPP
#define NOTHING_REACT_HPP

#include "error_reaction.hpp"

class cNoneReact : public cErrorReaction
{
public:
  cNoneReact(void);
  void ErrorReaction(const ui16 arg_errorNumber);
};

#endif
