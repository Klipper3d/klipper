// This file computes and sets best-effort clock speeds for the STM32. Note that
// it is (for now) quite conservative and does not use over-drive functionality
// to hit the maximum possible clock rates. It further expects that the MCU is
// powered from 3.3 volts. Lower voltages incur additional FLASH wait states.

// TODO(zbrozek): Support overdrive and underdrive.
// TODO(zbrozek): Support VCC other than 3.3 volts.
// TODO(zbrozek): Make clear the pre-reqs on *rcc for calling RCC_ClockConfig().

#include "rcc.h"

static const uint32_t kPllQMin = 0x2;
static const uint32_t kPllQMax = 0xF;
static const uint32_t kPllRMax = 0x7;

static const uint32_t kHsiClock = 16 * 1000 * 1000;  // 16 MHz.
static const uint32_t kHseClock = HSE_VALUE;  // Project-wide #define.
static const uint32_t kUsbClock = 48 * 1000 * 1000;  // 48 MHz.

static const uint32_t kAhbMaxDivisor = 15;
static const uint32_t kApb1MaxDivisor = 7;
static const uint32_t kApb2MaxDivisor = 7;

static const uint32_t kVcoInMinClock = 1 * 1000 * 1000;  // 1 MHz.
static const uint32_t kVcoInMaxClock = 2 * 1000 * 1000;  // 2 MHz.
static const uint32_t kVcoOutMinClock = 100 * 1000 * 1000;  // 100 MHz.
static const uint32_t kVcoOutMaxClock = 432 * 1000 * 1000;  // 432 MHz.

#ifdef STM32F7
#define kWaitCycles 10
static const uint32_t kPllMaxClock = 180 * 1000 * 1000;  // 180 MHz
static const uint32_t kAhbMaxClock = 180 * 1000 * 1000;  // 180 MHz
static const uint32_t kApb1MaxClock = 45 * 1000 * 1000;  // 45 MHz.
static const uint32_t kApb2MaxClock = 90 * 1000 * 1000;  // 90 MHz.
static const uint8_t kWaitStateLookup[4][kWaitCycles] = {
  {20, 40, 60,  80, 100, 120, 140, 160, 180,   0},
  {22, 44, 66,  88, 110, 132, 154, 176, 198, 216},
  {24, 48, 72,  96, 120, 144, 168, 192, 216,   0},
  {30, 60, 90, 120, 150, 180, 210, 216,   0,   0}
};
#endif

#ifdef STM32F4
#define kWaitCycles 9
static const uint32_t kPllMaxClock = 168 * 1000 * 1000;  // 168 MHz
static const uint32_t kAhbMaxClock = 168 * 1000 * 1000;  // 168 MHz
static const uint32_t kApb1MaxClock = 42 * 1000 * 1000;  // 42 MHz.
static const uint32_t kApb2MaxClock = 84 * 1000 * 1000;  // 84 MHz.
static const uint8_t kWaitStateLookup[4][kWaitCycles] = {
  {20, 40, 60,  80, 100, 120, 140, 160, 168},
  {22, 44, 66,  88, 110, 132, 154, 176, 180},
  {24, 48, 72,  96, 120, 144, 168, 180,   0},
  {30, 60, 90, 120, 150, 180,   0,   0,   0}
};
#endif

static void RCC_EnableHse(bool bypass) {
  RCC->CR |= (bypass ? RCC_CR_HSEBYP : 0);  // No crystal; enable HSE bypass.
  RCC->CR |= RCC_CR_HSEON;  // Enable the HSE.
  while(!(RCC->CR & RCC_CR_HSERDY));  // Wait for the HSE to become stable.
}

static void RCC_EnableHsi(void) {
  RCC->CR |= RCC_CR_HSION;  // Enable the HSI.
  while(!(RCC->CR & RCC_CR_HSIRDY));  // Wsait for the HSI to become stable.
}

static uint32_t RCC_FlashWaitLookup(uint32_t sysclk, uint32_t vcc_mv) {
  // Pick which table to use for lookup based on stated input voltage.
  const uint8_t *wait_table;
  if(vcc_mv >= 2700) {
    wait_table = kWaitStateLookup[3];
  } else if (vcc_mv >= 2400) {
    wait_table = kWaitStateLookup[2];
  } else if (vcc_mv >= 2100) {
    wait_table = kWaitStateLookup[1];
  } else {
    wait_table = kWaitStateLookup[0];
  }
  // Look up the minimum acceptable number of wait states.
  for(int32_t index = kWaitCycles - 1; index >= 0; index--) {
    if((wait_table[index] * 1000 * 1000) >= sysclk) {
      return index;
    }
  }
  // Give up and return the most-conservative value.
  return kWaitCycles - 1;
}

void RCC_ComputePll(Rcc *rcc, uint32_t target) {
  // Find the best PLL values without going over the target. Loops are ordered
  // to minimize PLL jitter. Divide by pll_m before multiplying by pll_n to
  // avoid integer overflow.
  uint32_t best = 0;
  rcc->pll.in = (rcc->pll.src == eRccSrcHse) ? kHseClock : kHsiClock;
  for(uint32_t pll_m = 2; pll_m <= 63; pll_m++) {
    // Bail on illegal VCO input frequencies.
    uint32_t vco_in = rcc->pll.in / pll_m;
    if(vco_in > kVcoInMaxClock) {continue;}
    if(vco_in < kVcoInMinClock) {break;}

    for(uint32_t pll_n = 2; pll_n <= 511; pll_n++) {
      // Bail on illegal VCO output frequencies.
      uint32_t vco_out = (rcc->pll.in / pll_m) * pll_n;
      if(vco_out > kVcoOutMaxClock) {break;}
      if(vco_out < kVcoOutMinClock) {continue;}

      for(uint32_t pll_p = 0; pll_p <= 3; pll_p++) {
        uint32_t pll_out = vco_out / (2 * (pll_p + 1));
        if(pll_out > kPllMaxClock) {continue;}
        if(pll_out > target) {continue;}
        if((target - pll_out) < (target - best)) {
          rcc->pll.out = best = pll_out;
          rcc->pll.m = pll_m;
          rcc->pll.n = pll_n;
          rcc->pll.p = pll_p;
          rcc->pll.r = kPllRMax;  // Minimum speed; DSI unused.

          // Do our best to satisfy USB clock requirements.
          uint32_t pll_q = vco_out / kUsbClock;
          if(pll_q >= kPllQMin && pll_q <= kPllQMax) {
            rcc->pll.q = pll_q;
          } else {
            rcc->pll.q = kPllQMax;
          }

          // Bail immediately if we've hit the jackpot.
          if(((vco_out % kUsbClock) == 0) && (target == pll_out)) {
            return;
          }
        }
      }
    }
  }
  // Non-jackpot best-effort PLL configuration is set in pllcfg.
}

// Computes the bits with which to configure AHB prescalers.
uint32_t RCC_GetBinaryDivisorBits(uint32_t input_clock, uint32_t *output_clock,
    uint32_t max_shift, uint32_t max_clock) {
  // PLL clock is already in-range; do nothing.
  if(input_clock <= max_clock) {
    *output_clock = input_clock;
    return 0;
  }

  // Find the smallest division ratio that stays below max clock limits.
  for(uint32_t shift = 0; shift <= max_shift; shift++) {
    *output_clock = input_clock >> (shift + 1);
    if(*output_clock <= max_clock) {
      return 0x4u | shift;
    }
  }
  return 0;  // Never happens; silences a compiler warning
}

void RCC_SetupPll(Rcc *rcc) {
  PllCfg *pll = &rcc->pll;
  // Enable the precursor clock. Does not disable any clocks.
  switch(pll->src) {
    case eRccSrcHse:
      RCC_EnableHse(rcc->hse_bypass);
      RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;  // Select HSE as input to PLL.
      break;
    case eRccSrcHsi:
    default:
      RCC_EnableHsi();
      RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;  // Select HSI as input to PLL.
      break;
  }

  // Set PLLQ.
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
  RCC->PLLCFGR |= (pll->q << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ;

#if defined RCC_PLLCFGR_PLLR
  // Set PLLR.
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
  RCC->PLLCFGR |= (pll->r << RCC_PLLCFGR_PLLR_Pos) & RCC_PLLCFGR_PLLR;
#endif

  // Set PLLM.
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
  RCC->PLLCFGR |= (pll->m << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM;

  // Set PLLN.
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
  RCC->PLLCFGR |= (pll->n << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN;

  // Set PLLP.
  RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;
  RCC->PLLCFGR |= (pll->p << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP;

  // Enable the PLL.
  RCC->CR |= RCC_CR_PLLON;  // Turn on the PLL.
  while(!(RCC->CR & RCC_CR_PLLRDY));  // Wait for it to stabilize.
}

void RCC_PrePostOps(Rcc *rcc) {
  // Clear and set PPRE2, PPRE1, HPRE bits to set AHB, APB1, and APB2 clocks.
  uint32_t hpre_bits = RCC_GetBinaryDivisorBits(rcc->sys, &rcc->ahb,
      kAhbMaxDivisor, kAhbMaxClock);
  uint32_t ppre1_bits = RCC_GetBinaryDivisorBits(rcc->ahb, &rcc->apb1,
      kApb1MaxDivisor, kApb1MaxClock);
  uint32_t ppre2_bits = RCC_GetBinaryDivisorBits(rcc->ahb, &rcc->apb2,
      kApb2MaxDivisor, kApb2MaxClock);

  // Set the APB1 prescaler.
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= (ppre1_bits << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1;

  // Set the APB2 prescaler.
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= (ppre2_bits << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2;

  // Set the AHB prescaler.
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CFGR |= (hpre_bits << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE;

  // Set the number of flash wait states.
  uint32_t waits = RCC_FlashWaitLookup(rcc->ahb, 3300);
  FLASH->ACR &= FLASH_ACR_LATENCY;
  FLASH->ACR |= waits << FLASH_ACR_LATENCY_Pos;
}

// Computes values for and sets up the STM32 clock tree, as well as setting
// appropriate flash wait states (assuming 3.3 volts VCC).
uint32_t RCC_ClockConfig(Rcc *rcc, uint32_t target) {
  // Record where we've started.
  Rcc start;
  RCC_ReadClocks(&start);

  // Set up the clock tree up to but not including SYSCLK. Set rcc->sys.
  switch(rcc->src) {
    case eRccSrcHse:
      RCC_EnableHse(rcc->hse_bypass);
      rcc->sys = kHseClock;
      break;
    case eRccSrcPll:
      RCC_ComputePll(rcc, target);  // Figure out our target PLL parameters.
      RCC_SetupPll(rcc);  // Enable the PLL and its precursor.
      rcc->sys = rcc->pll.out;
      break;
    case eRccSrcHsi:
    default:
      RCC_EnableHsi();
      rcc->sys = kHsiClock;
      break;
  };

  // We're about to increase the system bus clock speed. We need to decrease
  // bus clocks and increase flash wait states to avoid faulting the processor
  // immediately upon switching clocks.
  if(rcc->sys >= start.sys) {RCC_PrePostOps(rcc);}

  // Change the system clock mux over to the now-ready source.
  RCC->CFGR = (RCC->CFGR & ~(0x3)) | rcc->src;  // Select the new source.
  while((RCC->CFGR & (rcc->src << 2)) != (rcc->src << 2));  // Wait for switch.

  // We've reduced the system clock, so we should increase downstream bus clocks
  // and flash wait states after the reduction.
  if(target < start.sys) {RCC_PrePostOps(rcc);}

  return rcc->sys;
}

// Populates *rcc with system clock information computed by reading registers.
void RCC_ReadClocks(Rcc *rcc) {
  rcc->src = (RccSrc)((RCC->CFGR & RCC_CFGR_SWS) >> RCC_CFGR_SWS_Pos);
  rcc->hse_bypass = RCC->CR & RCC_CR_HSEBYP;

  uint32_t pllsrc = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
  rcc->pll.src = pllsrc ? eRccSrcHse : eRccSrcHsi;
  rcc->pll.in = pllsrc ? kHseClock : kHsiClock;
  rcc->pll.m = (RCC->PLLCFGR & RCC_PLLCFGR_PLLM);
  rcc->pll.n = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
  rcc->pll.p = (RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos;
  rcc->pll.q = (RCC->PLLCFGR & RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos;
#if defined RCC_PLLCFGR_PLLR
  rcc->pll.r = (RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos;
#else
  rcc->pll.r = 0;
#endif

  uint32_t vco = (rcc->pll.in / rcc->pll.m) * rcc->pll.n;
  rcc->pll.out = vco / ((rcc->pll.p + 1) * 2);

  uint32_t ahb_shift = 0;
  if(RCC->CFGR & RCC_CFGR_HPRE_3) {
    ahb_shift = (RCC->CFGR & (RCC_CFGR_HPRE & (RCC_CFGR_HPRE >> 1)));
    ahb_shift >>= RCC_CFGR_HPRE_Pos + 1;
    ahb_shift++;
  }

  uint32_t apb1_shift = 0;
  if(RCC->CFGR & RCC_CFGR_PPRE1_2) {
    apb1_shift = (RCC->CFGR & (RCC_CFGR_PPRE1 & (RCC_CFGR_PPRE1 >> 1)));
    apb1_shift >>= RCC_CFGR_PPRE1_Pos;
    apb1_shift++;
  }

  uint32_t apb2_shift = 0;
  if(RCC->CFGR & RCC_CFGR_PPRE2_2) {
    apb2_shift = (RCC->CFGR & (RCC_CFGR_PPRE2 & (RCC_CFGR_PPRE2 >> 1)));
    apb2_shift >>= RCC_CFGR_PPRE2_Pos;
    apb2_shift++;
  }

  switch(rcc->src) {
    case eRccSrcHsi:
      rcc->sys = kHsiClock;
      break;
    case eRccSrcHse:
      rcc->sys = kHseClock;
      break;
    case eRccSrcPll:
      rcc->sys = rcc->pll.out;
      break;
    default:
      break;
  }

  rcc->ahb = rcc->sys >> ahb_shift;
  rcc->apb1 = rcc->ahb >> apb1_shift;
  rcc->apb2 = rcc->ahb >> apb2_shift;
}
