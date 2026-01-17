/*
 * File:           C:\DixomProject\CooCox\DIXOM_DAC_ADAU1962_REG.h
 *
 * Created:        Wednesday, October 3, 2018 1:02:21 AM
 * Description:    DIXOM:DAC_ADAU1962 control register definitions.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2018 Analog Devices, Inc. All rights reserved.
 */
#ifndef __DIXOM_DAC_ADAU1962_REG_H__
#define __DIXOM_DAC_ADAU1962_REG_H__


/* PLL_CLK_CTRL0  - Registers (DAC_ADAU1962) */
#define REG_PLL_CLK_CTRL0_DAC_ADAU1962_ADDR       0x0
#define REG_PLL_CLK_CTRL0_DAC_ADAU1962_BYTE       1
#define REG_PLL_CLK_CTRL0_DAC_ADAU1962_VALUE      0x37

/* PLL_CLK_CTRL1  - Registers (DAC_ADAU1962) */
#define REG_PLL_CLK_CTRL1_DAC_ADAU1962_ADDR       0x1
#define REG_PLL_CLK_CTRL1_DAC_ADAU1962_BYTE       1
#define REG_PLL_CLK_CTRL1_DAC_ADAU1962_VALUE      0x3D

/* PDN_THRMSENS_CTRL_1  - Registers (DAC_ADAU1962) */
#define REG_PDN_THRMSENS_CTRL_1_DAC_ADAU1962_ADDR 0x2
#define REG_PDN_THRMSENS_CTRL_1_DAC_ADAU1962_BYTE 1
#define REG_PDN_THRMSENS_CTRL_1_DAC_ADAU1962_VALUE 0xF1

/* PDN_CTRL2  - Registers (DAC_ADAU1962) */
#define REG_PDN_CTRL2_DAC_ADAU1962_ADDR           0x3
#define REG_PDN_CTRL2_DAC_ADAU1962_BYTE           1
#define REG_PDN_CTRL2_DAC_ADAU1962_VALUE          0x0

/* PDN_CTRL3  - Registers (DAC_ADAU1962) */
#define REG_PDN_CTRL3_DAC_ADAU1962_ADDR           0x4
#define REG_PDN_CTRL3_DAC_ADAU1962_BYTE           1
#define REG_PDN_CTRL3_DAC_ADAU1962_VALUE          0x0

/* THRM_TEMP_STAT  - Registers (DAC_ADAU1962) */
#define REG_THRM_TEMP_STAT_DAC_ADAU1962_ADDR      0x5
#define REG_THRM_TEMP_STAT_DAC_ADAU1962_BYTE      0
#define REG_THRM_TEMP_STAT_DAC_ADAU1962_VALUE     0x64

/* DAC_CTRL0  - Registers (DAC_ADAU1962) */
#define REG_DAC_CTRL0_DAC_ADAU1962_ADDR           0x6
#define REG_DAC_CTRL0_DAC_ADAU1962_BYTE           1
#define REG_DAC_CTRL0_DAC_ADAU1962_VALUE          0x14

/* DAC_CTRL1  - Registers (DAC_ADAU1962) */
#define REG_DAC_CTRL1_DAC_ADAU1962_ADDR           0x7
#define REG_DAC_CTRL1_DAC_ADAU1962_BYTE           1
#define REG_DAC_CTRL1_DAC_ADAU1962_VALUE          0x80

/* DAC_CTRL2  - Registers (DAC_ADAU1962) */
#define REG_DAC_CTRL2_DAC_ADAU1962_ADDR           0x8
#define REG_DAC_CTRL2_DAC_ADAU1962_BYTE           1
#define REG_DAC_CTRL2_DAC_ADAU1962_VALUE          0x1

/* DAC_MUTE1  - Registers (DAC_ADAU1962) */
#define REG_DAC_MUTE1_DAC_ADAU1962_ADDR           0x9
#define REG_DAC_MUTE1_DAC_ADAU1962_BYTE           1
#define REG_DAC_MUTE1_DAC_ADAU1962_VALUE          0x0

/* DAC_MUTE2  - Registers (DAC_ADAU1962) */
#define REG_DAC_MUTE2_DAC_ADAU1962_ADDR           0xA
#define REG_DAC_MUTE2_DAC_ADAU1962_BYTE           1
#define REG_DAC_MUTE2_DAC_ADAU1962_VALUE          0x0

/* DACMSTR_VOL  - Registers (DAC_ADAU1962) */
#define REG_DACMSTR_VOL_DAC_ADAU1962_ADDR         0xB
#define REG_DACMSTR_VOL_DAC_ADAU1962_BYTE         1
#define REG_DACMSTR_VOL_DAC_ADAU1962_VALUE        0x0

/* DAC01_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC01_VOL_DAC_ADAU1962_ADDR           0xC
#define REG_DAC01_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC01_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC02_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC02_VOL_DAC_ADAU1962_ADDR           0xD
#define REG_DAC02_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC02_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC03_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC03_VOL_DAC_ADAU1962_ADDR           0xE
#define REG_DAC03_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC03_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC04_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC04_VOL_DAC_ADAU1962_ADDR           0xF
#define REG_DAC04_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC04_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC05_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC05_VOL_DAC_ADAU1962_ADDR           0x10
#define REG_DAC05_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC05_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC06_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC06_VOL_DAC_ADAU1962_ADDR           0x11
#define REG_DAC06_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC06_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC07_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC07_VOL_DAC_ADAU1962_ADDR           0x12
#define REG_DAC07_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC07_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC08_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC08_VOL_DAC_ADAU1962_ADDR           0x13
#define REG_DAC08_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC08_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC09_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC09_VOL_DAC_ADAU1962_ADDR           0x14
#define REG_DAC09_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC09_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC10_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC10_VOL_DAC_ADAU1962_ADDR           0x15
#define REG_DAC10_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC10_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC11_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC11_VOL_DAC_ADAU1962_ADDR           0x16
#define REG_DAC11_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC11_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC12_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC12_VOL_DAC_ADAU1962_ADDR           0x17
#define REG_DAC12_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC12_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC13_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC13_VOL_DAC_ADAU1962_ADDR           0x18
#define REG_DAC13_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC13_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC14_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC14_VOL_DAC_ADAU1962_ADDR           0x19
#define REG_DAC14_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC14_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC15_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC15_VOL_DAC_ADAU1962_ADDR           0x1A
#define REG_DAC15_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC15_VOL_DAC_ADAU1962_VALUE          0x0

/* DAC16_VOL  - Registers (DAC_ADAU1962) */
#define REG_DAC16_VOL_DAC_ADAU1962_ADDR           0x1B
#define REG_DAC16_VOL_DAC_ADAU1962_BYTE           1
#define REG_DAC16_VOL_DAC_ADAU1962_VALUE          0x0

/* CM_SEL_PAD_STRGTH  - Registers (DAC_ADAU1962) */
#define REG_CM_SEL_PAD_STRGTH_DAC_ADAU1962_ADDR   0x1C
#define REG_CM_SEL_PAD_STRGTH_DAC_ADAU1962_BYTE   1
#define REG_CM_SEL_PAD_STRGTH_DAC_ADAU1962_VALUE  0x2

/* DAC_POWER1  - Registers (DAC_ADAU1962) */
#define REG_DAC_POWER1_DAC_ADAU1962_ADDR          0x1D
#define REG_DAC_POWER1_DAC_ADAU1962_BYTE          1
#define REG_DAC_POWER1_DAC_ADAU1962_VALUE         0xFF

/* DAC_POWER2  - Registers (DAC_ADAU1962) */
#define REG_DAC_POWER2_DAC_ADAU1962_ADDR          0x1E
#define REG_DAC_POWER2_DAC_ADAU1962_BYTE          1
#define REG_DAC_POWER2_DAC_ADAU1962_VALUE         0xFF

/* DAC_POWER3  - Registers (DAC_ADAU1962) */
#define REG_DAC_POWER3_DAC_ADAU1962_ADDR          0x1F
#define REG_DAC_POWER3_DAC_ADAU1962_BYTE          1
#define REG_DAC_POWER3_DAC_ADAU1962_VALUE         0xFF

/* DAC_POWER4  - Registers (DAC_ADAU1962) */
#define REG_DAC_POWER4_DAC_ADAU1962_ADDR          0x20
#define REG_DAC_POWER4_DAC_ADAU1962_BYTE          1
#define REG_DAC_POWER4_DAC_ADAU1962_VALUE         0xFF


/*
 *
 * Control register's field descriptions
 *
 */

/* PLL_CLK_CTRL0 (DAC_ADAU1962) */
#define R0_PUP_DAC_ADAU1962                       0x1    /* 1b	[0] */
#define R0_MCS_DAC_ADAU1962                       0x3    /* 11b	[2:1] */
#define R0_SOFT_RST_DAC_ADAU1962                  0x0    /* 0b	[3] */
#define R0_XTAL_SET_DAC_ADAU1962                  0x3    /* 11b	[5:4] */
#define R0_PLLIN_DAC_ADAU1962                     0x0    /* 00b	[7:6] */
#define R0_PUP_DAC_ADAU1962_MASK                  0x1
#define R0_PUP_DAC_ADAU1962_SHIFT                 0
#define R0_MCS_DAC_ADAU1962_MASK                  0x6
#define R0_MCS_DAC_ADAU1962_SHIFT                 1
#define R0_SOFT_RST_DAC_ADAU1962_MASK             0x8
#define R0_SOFT_RST_DAC_ADAU1962_SHIFT            3
#define R0_XTAL_SET_DAC_ADAU1962_MASK             0x30
#define R0_XTAL_SET_DAC_ADAU1962_SHIFT            4
#define R0_PLLIN_DAC_ADAU1962_MASK                0xC0
#define R0_PLLIN_DAC_ADAU1962_SHIFT               6

/* PLL_CLK_CTRL1 (DAC_ADAU1962) */
#define R1_CLK_SEL_DAC_ADAU1962                   0x1    /* 1b	[0] */
#define R1_VREF_EN_DAC_ADAU1962                   0x0    /* 0b	[1] */
#define R1_PLL_LOCK_DAC_ADAU1962                  0x1    /* 1b	[2] */
#define R1_PLL_MUTE_DAC_ADAU1962                  0x1    /* 1b	[3] */
#define R1_MCLKO_SEL_DAC_ADAU1962                 0x3    /* 11b	[5:4] */
#define R1_LOPWR_MODE_DAC_ADAU1962                0x0    /* 00b	[7:6] */
#define R1_CLK_SEL_DAC_ADAU1962_MASK              0x1
#define R1_CLK_SEL_DAC_ADAU1962_SHIFT             0
#define R1_VREF_EN_DAC_ADAU1962_MASK              0x2
#define R1_VREF_EN_DAC_ADAU1962_SHIFT             1
#define R1_PLL_LOCK_DAC_ADAU1962_MASK             0x4
#define R1_PLL_LOCK_DAC_ADAU1962_SHIFT            2
#define R1_PLL_MUTE_DAC_ADAU1962_MASK             0x8
#define R1_PLL_MUTE_DAC_ADAU1962_SHIFT            3
#define R1_MCLKO_SEL_DAC_ADAU1962_MASK            0x30
#define R1_MCLKO_SEL_DAC_ADAU1962_SHIFT           4
#define R1_LOPWR_MODE_DAC_ADAU1962_MASK           0xC0
#define R1_LOPWR_MODE_DAC_ADAU1962_SHIFT          6

/* PDN_THRMSENS_CTRL_1 (DAC_ADAU1962) */
#define R2_VREG_PDN_DAC_ADAU1962                  0x1    /* 1b	[0] */
#define R2_PLL_PDN_DAC_ADAU1962                   0x0    /* 0b	[1] */
#define R2_TS_PDN_DAC_ADAU1962                    0x0    /* 0b	[2] */
#define R2_THRM_GO_DAC_ADAU1962                   0x1    /* 1b	[4] */
#define R2_THRM_MODE_DAC_ADAU1962                 0x1    /* 1b	[5] */
#define R2_THRM_RATE_DAC_ADAU1962                 0x3    /* 11b	[7:6] */
#define R2_VREG_PDN_DAC_ADAU1962_MASK             0x1
#define R2_VREG_PDN_DAC_ADAU1962_SHIFT            0
#define R2_PLL_PDN_DAC_ADAU1962_MASK              0x2
#define R2_PLL_PDN_DAC_ADAU1962_SHIFT             1
#define R2_TS_PDN_DAC_ADAU1962_MASK               0x4
#define R2_TS_PDN_DAC_ADAU1962_SHIFT              2
#define R2_THRM_GO_DAC_ADAU1962_MASK              0x10
#define R2_THRM_GO_DAC_ADAU1962_SHIFT             4
#define R2_THRM_MODE_DAC_ADAU1962_MASK            0x20
#define R2_THRM_MODE_DAC_ADAU1962_SHIFT           5
#define R2_THRM_RATE_DAC_ADAU1962_MASK            0xC0
#define R2_THRM_RATE_DAC_ADAU1962_SHIFT           6

/* PDN_CTRL2 (DAC_ADAU1962) */
#define R3_DAC01_PDN_DAC_ADAU1962                 0x0    /* 0b	[0] */
#define R3_DAC02_PDN_DAC_ADAU1962                 0x0    /* 0b	[1] */
#define R3_DAC03_PDN_DAC_ADAU1962                 0x0    /* 0b	[2] */
#define R3_DAC04_PDN_DAC_ADAU1962                 0x0    /* 0b	[3] */
#define R3_DAC05_PDN_DAC_ADAU1962                 0x0    /* 0b	[4] */
#define R3_DAC06_PDN_DAC_ADAU1962                 0x0    /* 0b	[5] */
#define R3_DAC07_PDN_DAC_ADAU1962                 0x0    /* 0b	[6] */
#define R3_DAC08_PDN_DAC_ADAU1962                 0x0    /* 0b	[7] */
#define R3_DAC01_PDN_DAC_ADAU1962_MASK            0x1
#define R3_DAC01_PDN_DAC_ADAU1962_SHIFT           0
#define R3_DAC02_PDN_DAC_ADAU1962_MASK            0x2
#define R3_DAC02_PDN_DAC_ADAU1962_SHIFT           1
#define R3_DAC03_PDN_DAC_ADAU1962_MASK            0x4
#define R3_DAC03_PDN_DAC_ADAU1962_SHIFT           2
#define R3_DAC04_PDN_DAC_ADAU1962_MASK            0x8
#define R3_DAC04_PDN_DAC_ADAU1962_SHIFT           3
#define R3_DAC05_PDN_DAC_ADAU1962_MASK            0x10
#define R3_DAC05_PDN_DAC_ADAU1962_SHIFT           4
#define R3_DAC06_PDN_DAC_ADAU1962_MASK            0x20
#define R3_DAC06_PDN_DAC_ADAU1962_SHIFT           5
#define R3_DAC07_PDN_DAC_ADAU1962_MASK            0x40
#define R3_DAC07_PDN_DAC_ADAU1962_SHIFT           6
#define R3_DAC08_PDN_DAC_ADAU1962_MASK            0x80
#define R3_DAC08_PDN_DAC_ADAU1962_SHIFT           7

/* PDN_CTRL3 (DAC_ADAU1962) */
#define R4_DAC09_PDN_DAC_ADAU1962                 0x0    /* 0b	[0] */
#define R4_DAC10_PDN_DAC_ADAU1962                 0x0    /* 0b	[1] */
#define R4_DAC11_PDN_DAC_ADAU1962                 0x0    /* 0b	[2] */
#define R4_DAC12_PDN_DAC_ADAU1962                 0x0    /* 0b	[3] */
#define R4_DAC13_PDN_DAC_ADAU1962                 0x0    /* 0b	[4] */
#define R4_DAC14_PDN_DAC_ADAU1962                 0x0    /* 0b	[5] */
#define R4_DAC15_PDN_DAC_ADAU1962                 0x0    /* 0b	[6] */
#define R4_DAC16_PDN_DAC_ADAU1962                 0x0    /* 0b	[7] */
#define R4_DAC09_PDN_DAC_ADAU1962_MASK            0x1
#define R4_DAC09_PDN_DAC_ADAU1962_SHIFT           0
#define R4_DAC10_PDN_DAC_ADAU1962_MASK            0x2
#define R4_DAC10_PDN_DAC_ADAU1962_SHIFT           1
#define R4_DAC11_PDN_DAC_ADAU1962_MASK            0x4
#define R4_DAC11_PDN_DAC_ADAU1962_SHIFT           2
#define R4_DAC12_PDN_DAC_ADAU1962_MASK            0x8
#define R4_DAC12_PDN_DAC_ADAU1962_SHIFT           3
#define R4_DAC13_PDN_DAC_ADAU1962_MASK            0x10
#define R4_DAC13_PDN_DAC_ADAU1962_SHIFT           4
#define R4_DAC14_PDN_DAC_ADAU1962_MASK            0x20
#define R4_DAC14_PDN_DAC_ADAU1962_SHIFT           5
#define R4_DAC15_PDN_DAC_ADAU1962_MASK            0x40
#define R4_DAC15_PDN_DAC_ADAU1962_SHIFT           6
#define R4_DAC16_PDN_DAC_ADAU1962_MASK            0x80
#define R4_DAC16_PDN_DAC_ADAU1962_SHIFT           7

/* THRM_TEMP_STAT (DAC_ADAU1962) */
#define R5_TEMP_DAC_ADAU1962                      0x64   /* 01100100b	[7:0] */
#define R5_TEMP_DAC_ADAU1962_MASK                 0xFF
#define R5_TEMP_DAC_ADAU1962_SHIFT                0

/* DAC_CTRL0 (DAC_ADAU1962) */
#define R6_MMUTE_DAC_ADAU1962                     0x0    /* 0b	[0] */
#define R6_FS_DAC_ADAU1962                        0x2    /* 10b	[2:1] */
#define R6_SAI_DAC_ADAU1962                       0x2    /* 010b	[5:3] */
#define R6_SDATA_FMT_DAC_ADAU1962                 0x0    /* 00b	[7:6] */
#define R6_MMUTE_DAC_ADAU1962_MASK                0x1
#define R6_MMUTE_DAC_ADAU1962_SHIFT               0
#define R6_FS_DAC_ADAU1962_MASK                   0x6
#define R6_FS_DAC_ADAU1962_SHIFT                  1
#define R6_SAI_DAC_ADAU1962_MASK                  0x38
#define R6_SAI_DAC_ADAU1962_SHIFT                 3
#define R6_SDATA_FMT_DAC_ADAU1962_MASK            0xC0
#define R6_SDATA_FMT_DAC_ADAU1962_SHIFT           6

/* DAC_CTRL1 (DAC_ADAU1962) */
#define R7_SAI_MS_DAC_ADAU1962                    0x0    /* 0b	[0] */
#define R7_BCLK_EDGE_DAC_ADAU1962                 0x0    /* 0b	[1] */
#define R7_BCLK_RATE_DAC_ADAU1962                 0x0    /* 0b	[2] */
#define R7_SAI_MSB_DAC_ADAU1962                   0x0    /* 0b	[4] */
#define R7_LRCLK_POL_DAC_ADAU1962                 0x0    /* 0b	[5] */
#define R7_LRCLK_MODE_DAC_ADAU1962                0x0    /* 0b	[6] */
#define R7_BCLK_GEN_DAC_ADAU1962                  0x1    /* 1b	[7] */
#define R7_SAI_MS_DAC_ADAU1962_MASK               0x1
#define R7_SAI_MS_DAC_ADAU1962_SHIFT              0
#define R7_BCLK_EDGE_DAC_ADAU1962_MASK            0x2
#define R7_BCLK_EDGE_DAC_ADAU1962_SHIFT           1
#define R7_BCLK_RATE_DAC_ADAU1962_MASK            0x4
#define R7_BCLK_RATE_DAC_ADAU1962_SHIFT           2
#define R7_SAI_MSB_DAC_ADAU1962_MASK              0x10
#define R7_SAI_MSB_DAC_ADAU1962_SHIFT             4
#define R7_LRCLK_POL_DAC_ADAU1962_MASK            0x20
#define R7_LRCLK_POL_DAC_ADAU1962_SHIFT           5
#define R7_LRCLK_MODE_DAC_ADAU1962_MASK           0x40
#define R7_LRCLK_MODE_DAC_ADAU1962_SHIFT          6
#define R7_BCLK_GEN_DAC_ADAU1962_MASK             0x80
#define R7_BCLK_GEN_DAC_ADAU1962_SHIFT            7

/* DAC_CTRL2 (DAC_ADAU1962) */
#define R8_DE_EMP_EN_DAC_ADAU1962                 0x1    /* 1b	[0] */
#define R8_DAC_OSR_DAC_ADAU1962                   0x0    /* 0b	[1] */
#define R8_AUTO_MUTE_EN_DAC_ADAU1962              0x0    /* 0b	[2] */
#define R8_DAC_POL_DAC_ADAU1962                   0x0    /* 0b	[3] */
#define R8_BCLK_TDMC_DAC_ADAU1962                 0x0    /* 0b	[4] */
#define R8_VREG_CTRL_DAC_ADAU1962                 0x0    /* 00b	[6:5] */
#define R8_DE_EMP_EN_DAC_ADAU1962_MASK            0x1
#define R8_DE_EMP_EN_DAC_ADAU1962_SHIFT           0
#define R8_DAC_OSR_DAC_ADAU1962_MASK              0x2
#define R8_DAC_OSR_DAC_ADAU1962_SHIFT             1
#define R8_AUTO_MUTE_EN_DAC_ADAU1962_MASK         0x4
#define R8_AUTO_MUTE_EN_DAC_ADAU1962_SHIFT        2
#define R8_DAC_POL_DAC_ADAU1962_MASK              0x8
#define R8_DAC_POL_DAC_ADAU1962_SHIFT             3
#define R8_BCLK_TDMC_DAC_ADAU1962_MASK            0x10
#define R8_BCLK_TDMC_DAC_ADAU1962_SHIFT           4
#define R8_VREG_CTRL_DAC_ADAU1962_MASK            0x60
#define R8_VREG_CTRL_DAC_ADAU1962_SHIFT           5

/* DAC_MUTE1 (DAC_ADAU1962) */
#define R9_DAC01_MUTE_DAC_ADAU1962                0x0    /* 0b	[0] */
#define R9_DAC02_MUTE_DAC_ADAU1962                0x0    /* 0b	[1] */
#define R9_DAC03_MUTE_DAC_ADAU1962                0x0    /* 0b	[2] */
#define R9_DAC04_MUTE_DAC_ADAU1962                0x0    /* 0b	[3] */
#define R9_DAC05_MUTE_DAC_ADAU1962                0x0    /* 0b	[4] */
#define R9_DAC06_MUTE_DAC_ADAU1962                0x0    /* 0b	[5] */
#define R9_DAC07_MUTE_DAC_ADAU1962                0x0    /* 0b	[6] */
#define R9_DAC08_MUTE_DAC_ADAU1962                0x0    /* 0b	[7] */
#define R9_DAC01_MUTE_DAC_ADAU1962_MASK           0x1
#define R9_DAC01_MUTE_DAC_ADAU1962_SHIFT          0
#define R9_DAC02_MUTE_DAC_ADAU1962_MASK           0x2
#define R9_DAC02_MUTE_DAC_ADAU1962_SHIFT          1
#define R9_DAC03_MUTE_DAC_ADAU1962_MASK           0x4
#define R9_DAC03_MUTE_DAC_ADAU1962_SHIFT          2
#define R9_DAC04_MUTE_DAC_ADAU1962_MASK           0x8
#define R9_DAC04_MUTE_DAC_ADAU1962_SHIFT          3
#define R9_DAC05_MUTE_DAC_ADAU1962_MASK           0x10
#define R9_DAC05_MUTE_DAC_ADAU1962_SHIFT          4
#define R9_DAC06_MUTE_DAC_ADAU1962_MASK           0x20
#define R9_DAC06_MUTE_DAC_ADAU1962_SHIFT          5
#define R9_DAC07_MUTE_DAC_ADAU1962_MASK           0x40
#define R9_DAC07_MUTE_DAC_ADAU1962_SHIFT          6
#define R9_DAC08_MUTE_DAC_ADAU1962_MASK           0x80
#define R9_DAC08_MUTE_DAC_ADAU1962_SHIFT          7

/* DAC_MUTE2 (DAC_ADAU1962) */
#define R10_DAC09_MUTE_DAC_ADAU1962               0x0    /* 0b	[0] */
#define R10_DAC10_MUTE_DAC_ADAU1962               0x0    /* 0b	[1] */
#define R10_DAC11_MUTE_DAC_ADAU1962               0x0    /* 0b	[2] */
#define R10_DAC12_MUTE_DAC_ADAU1962               0x0    /* 0b	[3] */
#define R10_DAC13_MUTE_DAC_ADAU1962               0x0    /* 0b	[4] */
#define R10_DAC14_MUTE_DAC_ADAU1962               0x0    /* 0b	[5] */
#define R10_DAC15_MUTE_DAC_ADAU1962               0x0    /* 0b	[6] */
#define R10_DAC16_MUTE_DAC_ADAU1962               0x0    /* 0b	[7] */
#define R10_DAC09_MUTE_DAC_ADAU1962_MASK          0x1
#define R10_DAC09_MUTE_DAC_ADAU1962_SHIFT         0
#define R10_DAC10_MUTE_DAC_ADAU1962_MASK          0x2
#define R10_DAC10_MUTE_DAC_ADAU1962_SHIFT         1
#define R10_DAC11_MUTE_DAC_ADAU1962_MASK          0x4
#define R10_DAC11_MUTE_DAC_ADAU1962_SHIFT         2
#define R10_DAC12_MUTE_DAC_ADAU1962_MASK          0x8
#define R10_DAC12_MUTE_DAC_ADAU1962_SHIFT         3
#define R10_DAC13_MUTE_DAC_ADAU1962_MASK          0x10
#define R10_DAC13_MUTE_DAC_ADAU1962_SHIFT         4
#define R10_DAC14_MUTE_DAC_ADAU1962_MASK          0x20
#define R10_DAC14_MUTE_DAC_ADAU1962_SHIFT         5
#define R10_DAC15_MUTE_DAC_ADAU1962_MASK          0x40
#define R10_DAC15_MUTE_DAC_ADAU1962_SHIFT         6
#define R10_DAC16_MUTE_DAC_ADAU1962_MASK          0x80
#define R10_DAC16_MUTE_DAC_ADAU1962_SHIFT         7

/* DACMSTR_VOL (DAC_ADAU1962) */
#define R11_DACMSTR_VOL_DAC_ADAU1962              0x00   /* 00000000b	[7:0] */
#define R11_DACMSTR_VOL_DAC_ADAU1962_MASK         0xFF
#define R11_DACMSTR_VOL_DAC_ADAU1962_SHIFT        0

/* DAC01_VOL (DAC_ADAU1962) */
#define R12_DAC01_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R12_DAC01_VOL_DAC_ADAU1962_MASK           0xFF
#define R12_DAC01_VOL_DAC_ADAU1962_SHIFT          0

/* DAC02_VOL (DAC_ADAU1962) */
#define R13_DAC02_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R13_DAC02_VOL_DAC_ADAU1962_MASK           0xFF
#define R13_DAC02_VOL_DAC_ADAU1962_SHIFT          0

/* DAC03_VOL (DAC_ADAU1962) */
#define R14_DAC03_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R14_DAC03_VOL_DAC_ADAU1962_MASK           0xFF
#define R14_DAC03_VOL_DAC_ADAU1962_SHIFT          0

/* DAC04_VOL (DAC_ADAU1962) */
#define R15_DAC04_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R15_DAC04_VOL_DAC_ADAU1962_MASK           0xFF
#define R15_DAC04_VOL_DAC_ADAU1962_SHIFT          0

/* DAC05_VOL (DAC_ADAU1962) */
#define R16_DAC05_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R16_DAC05_VOL_DAC_ADAU1962_MASK           0xFF
#define R16_DAC05_VOL_DAC_ADAU1962_SHIFT          0

/* DAC06_VOL (DAC_ADAU1962) */
#define R17_DAC06_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R17_DAC06_VOL_DAC_ADAU1962_MASK           0xFF
#define R17_DAC06_VOL_DAC_ADAU1962_SHIFT          0

/* DAC07_VOL (DAC_ADAU1962) */
#define R18_DAC07_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R18_DAC07_VOL_DAC_ADAU1962_MASK           0xFF
#define R18_DAC07_VOL_DAC_ADAU1962_SHIFT          0

/* DAC08_VOL (DAC_ADAU1962) */
#define R19_DAC08_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R19_DAC08_VOL_DAC_ADAU1962_MASK           0xFF
#define R19_DAC08_VOL_DAC_ADAU1962_SHIFT          0

/* DAC09_VOL (DAC_ADAU1962) */
#define R20_DAC09_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R20_DAC09_VOL_DAC_ADAU1962_MASK           0xFF
#define R20_DAC09_VOL_DAC_ADAU1962_SHIFT          0

/* DAC10_VOL (DAC_ADAU1962) */
#define R21_DAC10_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R21_DAC10_VOL_DAC_ADAU1962_MASK           0xFF
#define R21_DAC10_VOL_DAC_ADAU1962_SHIFT          0

/* DAC11_VOL (DAC_ADAU1962) */
#define R22_DAC11_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R22_DAC11_VOL_DAC_ADAU1962_MASK           0xFF
#define R22_DAC11_VOL_DAC_ADAU1962_SHIFT          0

/* DAC12_VOL (DAC_ADAU1962) */
#define R23_DAC12_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R23_DAC12_VOL_DAC_ADAU1962_MASK           0xFF
#define R23_DAC12_VOL_DAC_ADAU1962_SHIFT          0

/* DAC13_VOL (DAC_ADAU1962) */
#define R24_DAC13_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R24_DAC13_VOL_DAC_ADAU1962_MASK           0xFF
#define R24_DAC13_VOL_DAC_ADAU1962_SHIFT          0

/* DAC14_VOL (DAC_ADAU1962) */
#define R25_DAC14_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R25_DAC14_VOL_DAC_ADAU1962_MASK           0xFF
#define R25_DAC14_VOL_DAC_ADAU1962_SHIFT          0

/* DAC15_VOL (DAC_ADAU1962) */
#define R26_DAC15_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R26_DAC15_VOL_DAC_ADAU1962_MASK           0xFF
#define R26_DAC15_VOL_DAC_ADAU1962_SHIFT          0

/* DAC16_VOL (DAC_ADAU1962) */
#define R27_DAC16_VOL_DAC_ADAU1962                0x00   /* 00000000b	[7:0] */
#define R27_DAC16_VOL_DAC_ADAU1962_MASK           0xFF
#define R27_DAC16_VOL_DAC_ADAU1962_SHIFT          0

/* CM_SEL_PAD_STRGTH (DAC_ADAU1962) */
#define R28_CM_SEL_DAC_ADAU1962                   0x1    /* 1b	[1] */
#define R28_PAD_DRV_DAC_ADAU1962                  0x0    /* 0b	[5] */
#define R28_CM_SEL_DAC_ADAU1962_MASK              0x2
#define R28_CM_SEL_DAC_ADAU1962_SHIFT             1
#define R28_PAD_DRV_DAC_ADAU1962_MASK             0x20
#define R28_PAD_DRV_DAC_ADAU1962_SHIFT            5

/* DAC_POWER1 (DAC_ADAU1962) */
#define R29_DAC01_POWER_DAC_ADAU1962              0x3    /* 11b	[1:0] */
#define R29_DAC02_POWER_DAC_ADAU1962              0x3    /* 11b	[3:2] */
#define R29_DAC03_POWER_DAC_ADAU1962              0x3    /* 11b	[5:4] */
#define R29_DAC04_POWER_DAC_ADAU1962              0x3    /* 11b	[7:6] */
#define R29_DAC01_POWER_DAC_ADAU1962_MASK         0x3
#define R29_DAC01_POWER_DAC_ADAU1962_SHIFT        0
#define R29_DAC02_POWER_DAC_ADAU1962_MASK         0xC
#define R29_DAC02_POWER_DAC_ADAU1962_SHIFT        2
#define R29_DAC03_POWER_DAC_ADAU1962_MASK         0x30
#define R29_DAC03_POWER_DAC_ADAU1962_SHIFT        4
#define R29_DAC04_POWER_DAC_ADAU1962_MASK         0xC0
#define R29_DAC04_POWER_DAC_ADAU1962_SHIFT        6

/* DAC_POWER2 (DAC_ADAU1962) */
#define R30_DAC05_POWER_DAC_ADAU1962              0x3    /* 11b	[1:0] */
#define R30_DAC06_POWER_DAC_ADAU1962              0x3    /* 11b	[3:2] */
#define R30_DAC07_POWER_DAC_ADAU1962              0x3    /* 11b	[5:4] */
#define R30_DAC08_POWER_DAC_ADAU1962              0x3    /* 11b	[7:6] */
#define R30_DAC05_POWER_DAC_ADAU1962_MASK         0x3
#define R30_DAC05_POWER_DAC_ADAU1962_SHIFT        0
#define R30_DAC06_POWER_DAC_ADAU1962_MASK         0xC
#define R30_DAC06_POWER_DAC_ADAU1962_SHIFT        2
#define R30_DAC07_POWER_DAC_ADAU1962_MASK         0x30
#define R30_DAC07_POWER_DAC_ADAU1962_SHIFT        4
#define R30_DAC08_POWER_DAC_ADAU1962_MASK         0xC0
#define R30_DAC08_POWER_DAC_ADAU1962_SHIFT        6

/* DAC_POWER3 (DAC_ADAU1962) */
#define R31_DAC09_POWER_DAC_ADAU1962              0x3    /* 11b	[1:0] */
#define R31_DAC10_POWER_DAC_ADAU1962              0x3    /* 11b	[3:2] */
#define R31_DAC11_POWER_DAC_ADAU1962              0x3    /* 11b	[5:4] */
#define R31_DAC12_POWER_DAC_ADAU1962              0x3    /* 11b	[7:6] */
#define R31_DAC09_POWER_DAC_ADAU1962_MASK         0x3
#define R31_DAC09_POWER_DAC_ADAU1962_SHIFT        0
#define R31_DAC10_POWER_DAC_ADAU1962_MASK         0xC
#define R31_DAC10_POWER_DAC_ADAU1962_SHIFT        2
#define R31_DAC11_POWER_DAC_ADAU1962_MASK         0x30
#define R31_DAC11_POWER_DAC_ADAU1962_SHIFT        4
#define R31_DAC12_POWER_DAC_ADAU1962_MASK         0xC0
#define R31_DAC12_POWER_DAC_ADAU1962_SHIFT        6

/* DAC_POWER4 (DAC_ADAU1962) */
#define R32_DAC13_POWER_DAC_ADAU1962              0x3    /* 11b	[1:0] */
#define R32_DAC14_POWER_DAC_ADAU1962              0x3    /* 11b	[3:2] */
#define R32_DAC15_POWER_DAC_ADAU1962              0x3    /* 11b	[5:4] */
#define R32_DAC16_POWER_DAC_ADAU1962              0x3    /* 11b	[7:6] */
#define R32_DAC13_POWER_DAC_ADAU1962_MASK         0x3
#define R32_DAC13_POWER_DAC_ADAU1962_SHIFT        0
#define R32_DAC14_POWER_DAC_ADAU1962_MASK         0xC
#define R32_DAC14_POWER_DAC_ADAU1962_SHIFT        2
#define R32_DAC15_POWER_DAC_ADAU1962_MASK         0x30
#define R32_DAC15_POWER_DAC_ADAU1962_SHIFT        4
#define R32_DAC16_POWER_DAC_ADAU1962_MASK         0xC0
#define R32_DAC16_POWER_DAC_ADAU1962_SHIFT        6

#endif
