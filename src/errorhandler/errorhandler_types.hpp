#ifndef ERRORHANDLER_TYPES_HPP
#define ERRORHANDLER_TYPES_HPP

#include "types.h"
#include "error_reaction.hpp"
#include "none_react.hpp"
#include "reset_react.hpp"

#define NONE_REACT &NoneReact
#define RESET_REACT &ResetReact

constexpr ui8 errorTableSize = 10U;

enum eErrors:ui8
{
  E_FREE_TO_USE = 0U,

  //End of enum - dont touch it
  E_NUMBER_OF_ERRORS,
  E_NO_ERROR = 255U
};

enum eErrorEnabled:bool
{
  E_ERROR_DISABLED = false,
  E_ERROR_ENABLED = true
};

struct sErrorFlags
{
  bool active : 1;
  bool isAlreadyWritten : 1;

  sErrorFlags();
};

struct sErrorConfig
{
  const eErrors errorNumber;
  const eErrorEnabled enable;
  cErrorReaction* const reaction;
};

extern cNoneReact NoneReact;
extern cResetReact ResetReact;

#endif  /* #ifndef ERRORHANDLER_TYPES_HPP */
