#ifndef MP3_H
#define MP3_H

#include <stdint.h>

/*
 * This is the driver for the VS1053 MP3 Decoder
 */

//MP3_DREQ is PB9/RXD2




#define MP3_DREQ AT91C_PIO_PB9

// VS1053 Instructions
#define VS1053_READ  0x0300 // Read data
#define VS1053_WRITE 0x0200 // Write data


//{{{
//     VS1053 register mapping:
//                         SCI registers, prefix SCI
//      Abbrev[bits]         Reg    Type Reset    Time1      Description
#define VS1053_MODE          0x0  // rw  0x4800     80 CLKI  Mode control
#define VS1053_STATUS        0x1  // rw  0x000C     80 CLKI  Status of VS1053
#define VS1053_BASS          0x2  // rw  0          80 CLKI  Built-in bass/treble control
#define VS1053_CLOCKF        0x3  // rw  0        1200 CLKI  Clock freq + multiplier
#define VS1053_DECODE_TIME   0x4  // rw  0         100 CLKI  Decode time in seconds
#define VS1053_AUDATA        0x5  // rw  0         450 CLKI  Misc. audio data
#define VS1053_WRAM          0x6  // rw  0         100 CLKI  RAM write/read
#define VS1053_WRAMADDR      0x7  // rw  0         100 CLKI  Base address for RAM write/read
#define VS1053_HDAT0         0x8  // r   0          80 CLKI  Stream header data 0
#define VS1053_HDAT1         0x9  // r   0          80 CLKI  Stream header data 1
#define VS1053_AIADDR        0xA  // rw  0         210 CLKI  Start address of application
#define VS1053_VOL           0xB  // rw  0          80 CLKI  Volume control
#define VS1053_AICTRL0       0xC  // rw  0          80 CLKI  Application control register 0
#define VS1053_AICTRL1       0xD  // rw  0          80 CLKI  Application control register 1
#define VS1053_AICTRL2       0xE  // rw  0          80 CLKI  Application control register 2
#define VS1053_AICTRL3       0xF  // rw  0          80 CLKI  Application control register 3


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

//Name                                      Bits    Describtion
#define SS_DO_NOT_JUMP           15//       15      Header in decode, do not fast forward/rewind
#define SS_SWING                 12//       14:12   Set swing to +0dB, +0.5dB, ..., or +3.5dB
#define SS_VCM_OVERLORD          11//       11      GBUF overload indicator '1' = overload
#define SS_VCM_DISAbLE           10//       10      GBUF overload detection '1' = disev
// reserved
#define SS_VER                   4//        7:4     Version
#define SS_APDOWN2               3//        3       Analog driver powerdown
#define SS_APDOWN1               2//        2       Analog internal powerdown
#define SS_AD_CLOCK              1//        1       AD clock select, '0' = 6MHz, '1' = 3MHz
#define SS_REFERENCE_SEL         0//        0       Reference voltage selection, '0' = 1.23V, '1'=1.65V

// SCI BASS (RW) mapping:
#define ST_AMPLITUDE             12//       15:12   Treble Control in 1.5 dB steps (-8..7, 0 = off)
#define ST_FREQLIMIT             8//        11:8    Lower limit frequency in 1000 Hz steps (1..15)
#define SB_AMPLITUDE             4//        7:4     Bass Enhancement in 1 dB steps (0..15, 0 = off)
#define SB_FREQLIMIT             0//        3:0     Lower limit frequency in 10 Hz steps (2..15)

// SCI CLOCKF (RW) mapping:
#define SC_MULT                  13//       15:13   Clock multiplier
#define SC_ADD                   11//       12:11   Allowed multiplier addition
#define SC_FREQ                   0//       10: 0   Clock frequency

// Possible values for SC_MULT:
//                                   MASK      SC MULT       CLKI
#define SC_MULT_XTALI_times_1_0      0x0000//  0             XTALI
#define SC_MULT_XTALI_times_2_0      0x2000//  1             XTALI×2.0
#define SC_MULT_XTALI_times_2_5      0x4000//  2             XTALI×2.5
#define SC_MULT_XTALI_times_3_0      0x6000//  3             XTALI×3.0
#define SC_MULT_XTALI_times_3_5      0x8000//  4             XTALI×3.5
#define SC_MULT_XTALI_times_4_0      0xa000//  5             XTALI×4.0
#define SC_MULT_XTALI_times_4_5      0xc000//  6             XTALI×4.5
#define SC_MULT_XTALI_times_5_0      0xe000//  7             XTALI×5.0

// Possible Values for SC_ADD:
//                                   MASK  //  SC ADD        Multiplier addition
#define SC_ADD_0_0                   0x0000//  0             No modification is allowed
#define SC_ADD_1_0                   0x0800//  1             1.0×
#define SC_ADD_1_5                   0x1000//  2             1.5×
#define SC_ADD_2_0                   0x1800//  3             2.0×





//      SCI WRAMADDR (W) mapping:
// SM WRAMADDR       Dest. addr.      Bits/       Description
// Start..End        Start.. End      Word
// 0x1800..0x18XX    0x1800..0x18XX   16          X data RAM
// 0x5800..0x58XX    0x1800..0x18XX   16          Y data RAM
// 0x8040..0x84FF    0x0040..0x04FF   32          Instruction RAM
// 0xC000..0xFFFF    0xC000..0xFFFF   16          I/O



// SCI HDAT0 and SCI HDAT1 (R)

//Bit Function Value Explanation
//HDAT1[15:5]   syncword          2047       stream valid
//HDAT1[4:3]    ID                3          ISO 11172-3 MPG 1.0
//                                2          ISO 13818-3 MPG 2.0 (1/2-rate)
//                                1          MPG 2.5 (1/4-rate)
//                                0          MPG 2.5 (1/4-rate)
//HDAT1[2:1]    layer             3          I
//                                2          II
//                                1          III
//                                0          reserved
//HDAT1[0]      protect bit       1          No CRC
//                                0          CRC protected
//HDAT0[15:12]  bitrate                      see bitrate table
//HDAT0[11:10]  samplerate        3          reserved
//                                2          32/16/ 8 kHz
//                                1          48/24/12 kHz
//                                0          44/22/11 kHz
//HDAT0[9]      pad bit           1          additional slot
//                                0          normal frame
//HDAT0[8]      private bit                  not defined
//HDAT0[7:6]    mode              3          mono
//                                2          dual channel
//                                1          joint stereo
//                                0          stereo
//HDAT0[5:4]    extension                    see ISO 11172-3
//HDAT0[3]      copyright         1          copyrighted
//                                0          free
//HDAT0[2]      original          1          original
//                                0          copy
//HDAT0[1:0]    emphasis          3          CCITT J.17
//                                2          reserved
//                                1          50/15 microsec
//                                0          none

//}}}



void mp3_init();



uint16_t mp3_read(uint8_t adress);
void mp3_write(uint8_t adress, uint16_t data);






#endif /*MP3_H*/
