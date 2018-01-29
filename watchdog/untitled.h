

/** \file */
/** \defgroup avr_watchdog <avr/wdt.h>: Watchdog timer handling
    \code #include <avr/wdt.h> \endcode

    This header file declares the interface to some inline macros
    handling the watchdog timer present in many AVR devices.  In order
    to prevent the watchdog timer configuration from being
    accidentally altered by a crashing application, a special timed
    sequence is required in order to change it.  The macros within
    this header file handle the required sequence automatically
    before changing any value.  Interrupts will be disabled during
    the manipulation.

    \note Depending on the fuse configuration of the particular
    device, further restrictions might apply, in particular it might
    be disallowed to turn off the watchdog timer.

    Note that for newer devices (ATmega88 and newer, effectively any
    AVR that has the option to also generate interrupts), the watchdog
    timer remains active even after a system reset (except a power-on
    condition), using the fastest prescaler value (approximately 15
    ms).  It is therefore required to turn off the watchdog early
    during program startup, the datasheet recommends a sequence like
    the following:

    \code
    #include <stdint.h>
    #include <avr/wdt.h>

    uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

    void get_mcusr(void) \
      __attribute__((naked)) \
      __attribute__((section(".init3")));
    void get_mcusr(void)
    {
      mcusr_mirror = MCUSR;
      MCUSR = 0;
      wdt_disable();
    }
    \endcode

    Saving the value of MCUSR in \c mcusr_mirror is only needed if the
    application later wants to examine the reset source, but in particular, 
    clearing the watchdog reset flag before disabling the
    watchdog is required, according to the datasheet.
*/

/**
   \ingroup avr_watchdog
   Reset the watchdog timer.  When the watchdog timer is enabled,
   a call to this instruction is required before the timer expires,
   otherwise a watchdog-initiated device reset will occur. 
*/

#define wdt_reset() __asm__ __volatile__ ("wdr")

#define _WD_PS3_MASK       _BV(WDP3)

#define _WD_CONTROL_REG     WDTCSR

#define _WD_CHANGE_BIT      WDCE



/**
   \ingroup avr_watchdog
   Enable the watchdog timer, configuring it for expiry after
   \c timeout (which is a combination of the \c WDP0 through
   \c WDP2 bits to write into the \c WDTCR register; For those devices 
   that have a \c WDTCSR register, it uses the combination of the \c WDP0 
   through \c WDP3 bits).

   See also the symbolic constants \c WDTO_15MS et al.
*/


#else

static __inline__
__attribute__ ((__always_inline__))
void wdt_enable (const uint8_t value)
{
	if (_SFR_IO_REG_P (_WD_CONTROL_REG))
	{
		__asm__ __volatile__ (
				"in __tmp_reg__,__SREG__" "\n\t"
				"cli" "\n\t"
				"wdr" "\n\t"
				"out %0, %1" "\n\t"
				"out __SREG__,__tmp_reg__" "\n\t"
				"out %0, %2" "\n \t"
				: /* no outputs */
				: "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
				"r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
				"r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) |
						_BV(WDE) | (value & 0x07)) )
				: "r0"
		);
	}
	else
	{
		__asm__ __volatile__ (
				"in __tmp_reg__,__SREG__" "\n\t"
				"cli" "\n\t"
				"wdr" "\n\t"
				"sts %0, %1" "\n\t"
				"out __SREG__,__tmp_reg__" "\n\t"
				"sts %0, %2" "\n \t"
				: /* no outputs */
				: "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
				"r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
				"r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) |
						_BV(WDE) | (value & 0x07)) )
				: "r0"
		);
	}
}

static __inline__
__attribute__ ((__always_inline__))
void wdt_disable (void)
{
	if (_SFR_IO_REG_P (_WD_CONTROL_REG))
	{
        uint8_t register temp_reg;
		__asm__ __volatile__ (
				"in __tmp_reg__,__SREG__"    "\n\t"
				"cli"                        "\n\t"
				"wdr"                        "\n\t"
				"in  %[TEMPREG],%[WDTREG]"   "\n\t"
				"ori %[TEMPREG],%[WDCE_WDE]" "\n\t"
				"out %[WDTREG],%[TEMPREG]"   "\n\t"
				"out %[WDTREG],__zero_reg__" "\n\t"
				"out __SREG__,__tmp_reg__"   "\n\t"
				: [TEMPREG] "=d" (temp_reg)
				: [WDTREG]  "I"  (_SFR_IO_ADDR(_WD_CONTROL_REG)),
				[WDCE_WDE]  "n"  ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE)))
				: "r0"
		);
	}
	else
	{
        uint8_t register temp_reg;
		__asm__ __volatile__ (
				"in __tmp_reg__,__SREG__"    "\n\t"
				"cli"                        "\n\t"
				"wdr"                        "\n\t"
				"lds %[TEMPREG],%[WDTREG]"   "\n\t"
				"ori %[TEMPREG],%[WDCE_WDE]" "\n\t"
				"sts %[WDTREG],%[TEMPREG]"   "\n\t"
				"sts %[WDTREG],__zero_reg__" "\n\t"
				"out __SREG__,__tmp_reg__"   "\n\t"
				: [TEMPREG] "=d" (temp_reg)
				: [WDTREG]  "n"  (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
				[WDCE_WDE]  "n"  ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE)))
				: "r0"
		);
	}
}

#endif

#define WDTO_15MS   0

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_30MS   1

/** \ingroup avr_watchdog See
    \c WDT0_15MS */
#define WDTO_60MS   2

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_120MS  3

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_250MS  4

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_500MS  5

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_1S     6

/** \ingroup avr_watchdog
    See \c WDT0_15MS */
#define WDTO_2S     7

#define WDTO_4S     8

#define WDTO_8S     9


