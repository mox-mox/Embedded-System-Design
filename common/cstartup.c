//-----------------------------------------------------------------------------
//         ATMEL Microcontroller Software Support  -  ROUSSET  -
//-----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// File Name           : Cstartup_SAM7.c
// Object              : Low level initialisations written in C for Tools
//                       For AT91SAM7S with 2 flash plane
// Creation            : JPP  09-May-2006
//-----------------------------------------------------------------------------

// slightly modified by Martin Thomas:

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <board.h>

#include <dbgu.h>

/*  The following functions must be written in ARM mode this function is called
	 directly by the exception vector */
extern void AT91F_Spurious_handler(void);
extern void AT91F_Default_IRQ_handler(void);
extern void AT91F_Default_FIQ_handler(void);

/* try go handle unhandled interrupts
** don't hang or crash =) */
void _AT91F_Spurious_handler(void)
{
	dbgu_puts("spurious handler\n");
	//for(;;);
}

void _AT91F_Default_IRQ_handler(void)
{
	dbgu_puts("irq handler\n");
	for(;;);
}

void _AT91F_Default_FIQ_handler(void)
{
	dbgu_puts("fiq handler\n");
	for(;;);
}

//------------------------------------------------------------------------------
/// Default spurious interrupt handler
//------------------------------------------------------------------------------
void DefaultSpuriousHandler(void)
{
	dbgu_puts("spurious handler\n");
    while (1);
}

//------------------------------------------------------------------------------
/// Default handler for fast interrupt requests.
//------------------------------------------------------------------------------
void DefaultFiqHandler(void)
{
	dbgu_puts("fiq handler\n");
	while (1);
}

//------------------------------------------------------------------------------
/// Default handler for standard interrupt requests.
//------------------------------------------------------------------------------
void DefaultIrqHandler(void)
{
	dbgu_puts("irq handler\n");
	while (1);
}


//*----------------------------------------------------------------------------
//* \fn    AT91F_LowLevelInit
//* \brief This function performs very low level HW initialization
//*        this function can use a Stack, depending on the compilation
//*        optimization mode
//*----------------------------------------------------------------------------
void AT91F_LowLevelInit(void)
{
    unsigned char i;

    ///////////////////////////////////////////////////////////////////////////
    //  Disable Watchdog (write once register)
    ///////////////////////////////////////////////////////////////////////////
    delayus(10);
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
    AT91C_BASE_RSTC->RSTC_RMR = (AT91C_RSTC_KEY & 0xa5000000) | AT91C_RSTC_URSTEN | (AT91C_RSTC_ERSTL & 0x1000);

       // EFC Init, 5 cycles read
    //AT91C_BASE_EFC->EFC_FMR = AT91C_EFC_FWS_6WS;
    AT91C_BASE_EFC->EFC_FMR = 6 << 8;
    // Initialize main oscillator
    AT91C_BASE_PMC->PMC_MOR = BOARD_OSCOUNT | AT91C_CKGR_MOSCEN;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));


    // Initialize PLLA at 200MHz (198.656)
    AT91C_BASE_PMC->PMC_PLLAR = BOARD_CKGR_PLLA
                                | BOARD_PLLACOUNT
                                | BOARD_MULA
                                | BOARD_DIVA;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA));

    // Initialize PLLB for USB usage (if not already locked)
    if (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB)) {
        AT91C_BASE_PMC->PMC_PLLBR = BOARD_USBDIV
                                    | BOARD_CKGR_PLLB
                                    | BOARD_PLLBCOUNT
                                    | BOARD_MULB
                                    | BOARD_DIVB;
        while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB));
    }

    // Wait for the master clock if it was already initialized
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));


     // Switch to fast clock
    //---------------------
    // Switch to main oscillator + prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_MDIV_2;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
        // Switch to PLL + prescaler
    //AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLLA_CLK | BOARD_PRESCALER | AT91C_PMC_PRES_CLK_2;
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_MDIV_2;

    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));


    ///////////////////////////////////////////////////////////////////////////
    	// Reset Controller Mode Register
        // URSTEN: low level on NRST triggers user reset
        // External Reset Length: Slow Clock Cycles * 2^(ERSTL+1) => (1/32kHz) * 2^5 =~ 1ms
        // Passwort: must be 0xA5
        ///////////////////////////////////////////////////////////////////////////
        AT91C_BASE_RSTC->RSTC_RMR = AT91C_RSTC_URSTEN | (0x4 << 8) | (0xA5 << 24);

        ///////////////////////////////////////////////////////////////////////////
        //  Init AIC: assign corresponding handler for each interrupt source
        ///////////////////////////////////////////////////////////////////////////
        AT91C_BASE_AIC->AIC_SVR[0] = (unsigned int)_AT91F_Default_FIQ_handler;
        for(i = 1; i < 31; i++) {
            AT91C_BASE_AIC->AIC_SVR[i] = (unsigned int)_AT91F_Default_IRQ_handler;
        }
        AT91C_BASE_AIC->AIC_SPU = (unsigned int)_AT91F_Spurious_handler;


/*
   // Initialize AIC
        //---------------
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;
        AT91C_BASE_AIC->AIC_SVR[0] = (unsigned int) DefaultFiqHandler;
        for (i = 1; i < 31; i++) {

            AT91C_BASE_AIC->AIC_SVR[i] = (unsigned int) DefaultIrqHandler;
        }
        AT91C_BASE_AIC->AIC_SPU = (unsigned int) DefaultSpuriousHandler;

        // Unstack nested interrupts
        for (i = 0; i < 8 ; i++) {

            AT91C_BASE_AIC->AIC_EOICR = 0;
        }

        // Watchdog initialization
        //------------------------
        AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;

*/
        //read CP15 register 1 into r0
        //I Cache enable
        //D Cache enable
        //write CP15 register 1
       asm volatile (
    		   ".align                \n\t"
    		   ".arm                  \n\t"
    		   "MRC p15, 0, r0, c1, c0, 0 \n\t"
       	   	   "ORR r0, r0, #(0x1 <<12)   \n\t"
    		   "ORR r0, r0, #(0x1 <<2)    \n\t"
    		   "MCR p15, 0, r0, c1, c0, 0 "
    		   	   );


        // Remap

#define WRITE(peripheral, register, value)  (peripheral->register = value)
        WRITE(AT91C_BASE_MATRIX,
                 MATRIX_MRCR,
                 (AT91C_MATRIX_RCA926I | AT91C_MATRIX_RCA926D));


        // Disable RTT and PIT interrupts (potential problem when program A
        // configures RTT, then program B wants to use PIT only, interrupts
        // from the RTT will still occur since they both use AT91C_ID_SYS)
 //       AT91C_BASE_RTTC->RTTC_RTMR &= ~(AT91C_RTTC_ALMIEN | AT91C_RTTC_RTTINCIEN);
 //       AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;
}
