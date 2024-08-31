#ifndef NOTHING_REACT_HPP
#define NOTHING_REACT_HPP

#include "error_reaction.hpp"

class cNoneReact : public cErrorReaction
{
public:
  cNoneReact(void);
  void ErrorReaction(void);
};

#endif
