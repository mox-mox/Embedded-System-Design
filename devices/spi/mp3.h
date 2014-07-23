#ifndef MP3_H_
#define MP3_H_

/*
 * This is the driver for the VS1053 MP3 Decoder
 */

//MP3_DREQ is PB9/RXD2









//     VS1053 register mapping:
//                         SCI registers, prefix SCI
//      Abbrev[bits]  Reg     Type Reset    Time1      Description
#define MODE          0x0  // rw  0x4800     80 CLKI  Mode control
#define STATUS        0x1  // rw  0x000C     80 CLKI  Status of VS1053
#define BASS          0x2  // rw  0          80 CLKI  Built-in bass/treble control
#define CLOCKF        0x3  // rw  0        1200 CLKI  Clock freq + multiplier
#define DECODE_TIME   0x4  // rw  0         100 CLKI  Decode time in seconds
#define AUDATA        0x5  // rw  0         450 CLKI  Misc. audio data
#define WRAM          0x6  // rw  0         100 CLKI  RAM write/read
#define WRAMADDR      0x7  // rw  0         100 CLKI  Base address for RAM write/read
#define HDAT0         0x8  // r   0          80 CLKI  Stream header data 0
#define HDAT1         0x9  // r   0          80 CLKI  Stream header data 1
#define AIADDR        0xA  // rw  0         210 CLKI  Start address of application
#define VOL           0xB  // rw  0          80 CLKI  Volume control
#define AICTRL0       0xC  // rw  0          80 CLKI  Application control register 0
#define AICTRL1       0xD  // rw  0          80 CLKI  Application control register 1
#define AICTRL2       0xE  // rw  0          80 CLKI  Application control register 2
#define AICTRL3       0xF  // rw  0          80 CLKI  Application control register 3


//SCI_MODE (RW) mapping:
//
//Bit   Name                Function                        Value   Description
//0     SM_DIFF             Differential                    0       normal in-phase audio
//                                                          1       left channel inverted
//1	  SM_LAYER12          Allow MPEG layer I&II           0       no
//                                                          1       yes
//2     SM_RESET            Soft reset                      0       no reset
//                                                          1       reset
//3     SM_CANCEL           Cancel decoding current file    0       no
//                                                          1       yes
//4     SM_EARSPEAKER_LO    EarSpeaker low setting          0       off
//                                                          1       active
//5     SM_TESTS            Allow SDI tests                 0       not allowed
//                                                          1       allowed
//6     SM_STREAM           Stream mode                     0       no
//                                                          1       yes
//7     SM_EARSPEAKER_HI    EarSpeaker high setting         0       off
//                                                          1       active
//8     SM_DACT             DCLK active edge                0       rising
//                                                          1       falling
//9     SM_SDIORD           SDI bit order                   0       MSb first
//                                                          1       MSb last
//10    SM_SDISHARE         Share SPI chip select           0       no
//                                                          1       yes
//11    SM_SDINEW           VS1002 native SPI modes         0       no
//                                                          1       yes
//12    SM_ADPCM            ADPCM recording active          0       no
//                                                          1       yes
//13    --------            -----------------------         -       ----
//14    SM_LINE1            MIC/LINE1 selector              0       MICP
//                                                          1       LINE1
//15    SM_CLK_ranger       Input clock range               0       12..13 MHz
//                                                          1       24..26 MHz




// SCI_STATUS (RW) mapping:

Name                Bits     Describtion
SS_DO_NOT_JUMP      15       Header in decode, do not fast forward/rewind
SS_SWING            14:12    Set swing to +0dB, +0.5dB, ..., or +3.5dB
SS_VCM_OVERLORD     11       GBUF overload indicator '1' = overload
SS_VCM_DISAbLE      10       GBUF overload detection '1' = disev
// reserved
SS_VER              7:4      Version
SS_APDOWN2           3       Analog driver powerdown
SS_APDOWN1           2       Analog internal powerdown
SS_AD_CLOCK         1   AD clock select, '0' = 6MHz, '1' = 3MHz
SS_REFERENCE_SEL    0        Reference voltage selection, '0' = 1.23V, '1'=1.65V




#endif /*MP3_H_*/
