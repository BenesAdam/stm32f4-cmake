#include "header_checker.hpp"
#include "libopencm3/stm32/f4/nvic.h"
#include "libopencm3/cm3/scb.h"
#include "libopencm3/cm3/systick.h"
#include "libopencm3/stm32/hash.h"
#include "libopencm3/stm32/rcc.h"
#include "sha256.hpp"
#include "rsa_2048.hpp"
#include "systick.hpp"

typedef void (*Application)(void);
ui32 cHeaderChecker::HeaderAddress = 0U;

cHeaderChecker::cHeaderChecker(void) :
  Header(nullptr),
  Hash(),
  Hash_from_sig(),
  AppFounded(false),
  SignitureRight(false),
  AppRuntime(0)
{
}

void cHeaderChecker::CheckApplication(void)
{
  ui32 startTime = cSysTick::Millis();

  AppFounded = FindApplication();

  if (AppFounded)
  {
    SignitureRight = CheckSignature();
  }

  AppRuntime = cSysTick::Millis() - startTime;
}

bool cHeaderChecker::IsAppValidated(void)
{
  return AppFounded && SignitureRight;
}

bool cHeaderChecker::FindApplication(void)
{
  for (const sAddressRange& range : CheckMemory.AddressRange)
  {
    if ((range.lowerAddress & 0xFFU) == 0U)
    {
      ui32 address = range.lowerAddress;
      while (address + sizeof(sHeader) <= range.upperAddress + 1U)
      {
        if (*(volatile ui32*)address == 0x12345678U)
        {
          HeaderAddress = address + sizeof(sHeader);
          Header = reinterpret_cast<sHeader*>(address);
          return true;
        }

        address += 0x100U;
      }
    }
  }

  return false;
}

bool cHeaderChecker::CheckSignature(void)
{
  bool isProgramValid = true;
  sha256_mem(Header->startAddress, Header->endAddress, Hash);
  rsa_sig_to_hash(Hash_from_sig, Header);

  for (ui8 index = 0U; index < 32U; index++)
  {
    if (Hash[index] != Hash_from_sig[index])
    {
      isProgramValid = false;
    }
  }

  return isProgramValid;
}

void cHeaderChecker::RunApplication(void)
{
  ui32 appStack = *(volatile ui32*)HeaderAddress;
  ui32 appEntry = *(volatile ui32*)(HeaderAddress + 4U);

  Application JumpToApp = (Application)appEntry;

  __asm__ volatile("cpsid i");

  systick_counter_disable();
  systick_interrupt_disable();

  for (int i = 0; i < 8; i++)
  {
    NVIC_ICER(i) = 0xFFFFFFFF;
    NVIC_ICPR(i) = 0xFFFFFFFF;
  }

  __asm__ volatile("msr msp, %0" : : "r"(appStack) :);

  SCB_VTOR = HeaderAddress;

  JumpToApp();

  while (true)
  {
  }
}

cHeaderChecker HeaderChecker;
