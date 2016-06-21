/*
 * Register definitions for remote debugging on the Freescale Kinetis chips.
 * Based on definitions from:
 *
 * Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include "arm_reg.h"

// chapter 11: Port control and interrupts (PORT)
#define REG_PORTA_PCR0              0x40049000            // Pin Control Register n
#define REG_PORT_PCR_ISF            0x01000000            // Interrupt Status Flag
#define REG_PORT_PCR_IRQC(n)        (((n) & 15) << 16)    // Interrupt Configuration
#define REG_PORT_PCR_IRQC_MASK      0x000F0000
#define REG_PORT_PCR_LK             0x00008000            // Lock Register
#define REG_PORT_PCR_MUX(n)         (((n) & 7) << 8)      // Pin Mux Control
#define REG_PORT_PCR_MUX_MASK       0x00000700
#define REG_PORT_PCR_DSE            0x00000040            // Drive Strength Enable
#define REG_PORT_PCR_ODE            0x00000020            // Open Drain Enable
#define REG_PORT_PCR_PFE            0x00000010            // Passive Filter Enable
#define REG_PORT_PCR_SRE            0x00000004            // Slew Rate Enable
#define REG_PORT_PCR_PE             0x00000002            // Pull Enable
#define REG_PORT_PCR_PS             0x00000001            // Pull Select
#define REG_PORTA_PCR1              0x40049004            // Pin Control Register n
#define REG_PORTA_PCR2              0x40049008            // Pin Control Register n
#define REG_PORTA_PCR3              0x4004900C            // Pin Control Register n
#define REG_PORTA_PCR4              0x40049010            // Pin Control Register n
#define REG_PORTA_PCR5              0x40049014            // Pin Control Register n
#define REG_PORTA_PCR6              0x40049018            // Pin Control Register n
#define REG_PORTA_PCR7              0x4004901C            // Pin Control Register n
#define REG_PORTA_PCR8              0x40049020            // Pin Control Register n
#define REG_PORTA_PCR9              0x40049024            // Pin Control Register n
#define REG_PORTA_PCR10             0x40049028            // Pin Control Register n
#define REG_PORTA_PCR11             0x4004902C            // Pin Control Register n
#define REG_PORTA_PCR12             0x40049030            // Pin Control Register n
#define REG_PORTA_PCR13             0x40049034            // Pin Control Register n
#define REG_PORTA_PCR14             0x40049038            // Pin Control Register n
#define REG_PORTA_PCR15             0x4004903C            // Pin Control Register n
#define REG_PORTA_PCR16             0x40049040            // Pin Control Register n
#define REG_PORTA_PCR17             0x40049044            // Pin Control Register n
#define REG_PORTA_PCR18             0x40049048            // Pin Control Register n
#define REG_PORTA_PCR19             0x4004904C            // Pin Control Register n
#define REG_PORTA_PCR20             0x40049050            // Pin Control Register n
#define REG_PORTA_PCR21             0x40049054            // Pin Control Register n
#define REG_PORTA_PCR22             0x40049058            // Pin Control Register n
#define REG_PORTA_PCR23             0x4004905C            // Pin Control Register n
#define REG_PORTA_PCR24             0x40049060            // Pin Control Register n
#define REG_PORTA_PCR25             0x40049064            // Pin Control Register n
#define REG_PORTA_PCR26             0x40049068            // Pin Control Register n
#define REG_PORTA_PCR27             0x4004906C            // Pin Control Register n
#define REG_PORTA_PCR28             0x40049070            // Pin Control Register n
#define REG_PORTA_PCR29             0x40049074            // Pin Control Register n
#define REG_PORTA_PCR30             0x40049078            // Pin Control Register n
#define REG_PORTA_PCR31             0x4004907C            // Pin Control Register n
#define REG_PORTA_GPCLR             0x40049080            // Global Pin Control Low Register
#define REG_PORTA_GPCHR             0x40049084            // Global Pin Control High Register
#define REG_PORTA_ISFR              0x400490A0            // Interrupt Status Flag Register
#define REG_PORTB_PCR0              0x4004A000            // Pin Control Register n
#define REG_PORTB_PCR1              0x4004A004            // Pin Control Register n
#define REG_PORTB_PCR2              0x4004A008            // Pin Control Register n
#define REG_PORTB_PCR3              0x4004A00C            // Pin Control Register n
#define REG_PORTB_PCR4              0x4004A010            // Pin Control Register n
#define REG_PORTB_PCR5              0x4004A014            // Pin Control Register n
#define REG_PORTB_PCR6              0x4004A018            // Pin Control Register n
#define REG_PORTB_PCR7              0x4004A01C            // Pin Control Register n
#define REG_PORTB_PCR8              0x4004A020            // Pin Control Register n
#define REG_PORTB_PCR9              0x4004A024            // Pin Control Register n
#define REG_PORTB_PCR10             0x4004A028            // Pin Control Register n
#define REG_PORTB_PCR11             0x4004A02C            // Pin Control Register n
#define REG_PORTB_PCR12             0x4004A030            // Pin Control Register n
#define REG_PORTB_PCR13             0x4004A034            // Pin Control Register n
#define REG_PORTB_PCR14             0x4004A038            // Pin Control Register n
#define REG_PORTB_PCR15             0x4004A03C            // Pin Control Register n
#define REG_PORTB_PCR16             0x4004A040            // Pin Control Register n
#define REG_PORTB_PCR17             0x4004A044            // Pin Control Register n
#define REG_PORTB_PCR18             0x4004A048            // Pin Control Register n
#define REG_PORTB_PCR19             0x4004A04C            // Pin Control Register n
#define REG_PORTB_PCR20             0x4004A050            // Pin Control Register n
#define REG_PORTB_PCR21             0x4004A054            // Pin Control Register n
#define REG_PORTB_PCR22             0x4004A058            // Pin Control Register n
#define REG_PORTB_PCR23             0x4004A05C            // Pin Control Register n
#define REG_PORTB_PCR24             0x4004A060            // Pin Control Register n
#define REG_PORTB_PCR25             0x4004A064            // Pin Control Register n
#define REG_PORTB_PCR26             0x4004A068            // Pin Control Register n
#define REG_PORTB_PCR27             0x4004A06C            // Pin Control Register n
#define REG_PORTB_PCR28             0x4004A070            // Pin Control Register n
#define REG_PORTB_PCR29             0x4004A074            // Pin Control Register n
#define REG_PORTB_PCR30             0x4004A078            // Pin Control Register n
#define REG_PORTB_PCR31             0x4004A07C            // Pin Control Register n
#define REG_PORTB_GPCLR             0x4004A080            // Global Pin Control Low Register
#define REG_PORTB_GPCHR             0x4004A084            // Global Pin Control High Register
#define REG_PORTB_ISFR              0x4004A0A0            // Interrupt Status Flag Register
#define REG_PORTC_PCR0              0x4004B000            // Pin Control Register n
#define REG_PORTC_PCR1              0x4004B004            // Pin Control Register n
#define REG_PORTC_PCR2              0x4004B008            // Pin Control Register n
#define REG_PORTC_PCR3              0x4004B00C            // Pin Control Register n
#define REG_PORTC_PCR4              0x4004B010            // Pin Control Register n
#define REG_PORTC_PCR5              0x4004B014            // Pin Control Register n
#define REG_PORTC_PCR6              0x4004B018            // Pin Control Register n
#define REG_PORTC_PCR7              0x4004B01C            // Pin Control Register n
#define REG_PORTC_PCR8              0x4004B020            // Pin Control Register n
#define REG_PORTC_PCR9              0x4004B024            // Pin Control Register n
#define REG_PORTC_PCR10             0x4004B028            // Pin Control Register n
#define REG_PORTC_PCR11             0x4004B02C            // Pin Control Register n
#define REG_PORTC_PCR12             0x4004B030            // Pin Control Register n
#define REG_PORTC_PCR13             0x4004B034            // Pin Control Register n
#define REG_PORTC_PCR14             0x4004B038            // Pin Control Register n
#define REG_PORTC_PCR15             0x4004B03C            // Pin Control Register n
#define REG_PORTC_PCR16             0x4004B040            // Pin Control Register n
#define REG_PORTC_PCR17             0x4004B044            // Pin Control Register n
#define REG_PORTC_PCR18             0x4004B048            // Pin Control Register n
#define REG_PORTC_PCR19             0x4004B04C            // Pin Control Register n
#define REG_PORTC_PCR20             0x4004B050            // Pin Control Register n
#define REG_PORTC_PCR21             0x4004B054            // Pin Control Register n
#define REG_PORTC_PCR22             0x4004B058            // Pin Control Register n
#define REG_PORTC_PCR23             0x4004B05C            // Pin Control Register n
#define REG_PORTC_PCR24             0x4004B060            // Pin Control Register n
#define REG_PORTC_PCR25             0x4004B064            // Pin Control Register n
#define REG_PORTC_PCR26             0x4004B068            // Pin Control Register n
#define REG_PORTC_PCR27             0x4004B06C            // Pin Control Register n
#define REG_PORTC_PCR28             0x4004B070            // Pin Control Register n
#define REG_PORTC_PCR29             0x4004B074            // Pin Control Register n
#define REG_PORTC_PCR30             0x4004B078            // Pin Control Register n
#define REG_PORTC_PCR31             0x4004B07C            // Pin Control Register n
#define REG_PORTC_GPCLR             0x4004B080            // Global Pin Control Low Register
#define REG_PORTC_GPCHR             0x4004B084            // Global Pin Control High Register
#define REG_PORTC_ISFR              0x4004B0A0            // Interrupt Status Flag Register
#define REG_PORTD_PCR0              0x4004C000            // Pin Control Register n
#define REG_PORTD_PCR1              0x4004C004            // Pin Control Register n
#define REG_PORTD_PCR2              0x4004C008            // Pin Control Register n
#define REG_PORTD_PCR3              0x4004C00C            // Pin Control Register n
#define REG_PORTD_PCR4              0x4004C010            // Pin Control Register n
#define REG_PORTD_PCR5              0x4004C014            // Pin Control Register n
#define REG_PORTD_PCR6              0x4004C018            // Pin Control Register n
#define REG_PORTD_PCR7              0x4004C01C            // Pin Control Register n
#define REG_PORTD_PCR8              0x4004C020            // Pin Control Register n
#define REG_PORTD_PCR9              0x4004C024            // Pin Control Register n
#define REG_PORTD_PCR10             0x4004C028            // Pin Control Register n
#define REG_PORTD_PCR11             0x4004C02C            // Pin Control Register n
#define REG_PORTD_PCR12             0x4004C030            // Pin Control Register n
#define REG_PORTD_PCR13             0x4004C034            // Pin Control Register n
#define REG_PORTD_PCR14             0x4004C038            // Pin Control Register n
#define REG_PORTD_PCR15             0x4004C03C            // Pin Control Register n
#define REG_PORTD_PCR16             0x4004C040            // Pin Control Register n
#define REG_PORTD_PCR17             0x4004C044            // Pin Control Register n
#define REG_PORTD_PCR18             0x4004C048            // Pin Control Register n
#define REG_PORTD_PCR19             0x4004C04C            // Pin Control Register n
#define REG_PORTD_PCR20             0x4004C050            // Pin Control Register n
#define REG_PORTD_PCR21             0x4004C054            // Pin Control Register n
#define REG_PORTD_PCR22             0x4004C058            // Pin Control Register n
#define REG_PORTD_PCR23             0x4004C05C            // Pin Control Register n
#define REG_PORTD_PCR24             0x4004C060            // Pin Control Register n
#define REG_PORTD_PCR25             0x4004C064            // Pin Control Register n
#define REG_PORTD_PCR26             0x4004C068            // Pin Control Register n
#define REG_PORTD_PCR27             0x4004C06C            // Pin Control Register n
#define REG_PORTD_PCR28             0x4004C070            // Pin Control Register n
#define REG_PORTD_PCR29             0x4004C074            // Pin Control Register n
#define REG_PORTD_PCR30             0x4004C078            // Pin Control Register n
#define REG_PORTD_PCR31             0x4004C07C            // Pin Control Register n
#define REG_PORTD_GPCLR             0x4004C080            // Global Pin Control Low Register
#define REG_PORTD_GPCHR             0x4004C084            // Global Pin Control High Register
#define REG_PORTD_ISFR              0x4004C0A0            // Interrupt Status Flag Register
#define REG_PORTE_PCR0              0x4004D000            // Pin Control Register n
#define REG_PORTE_PCR1              0x4004D004            // Pin Control Register n
#define REG_PORTE_PCR2              0x4004D008            // Pin Control Register n
#define REG_PORTE_PCR3              0x4004D00C            // Pin Control Register n
#define REG_PORTE_PCR4              0x4004D010            // Pin Control Register n
#define REG_PORTE_PCR5              0x4004D014            // Pin Control Register n
#define REG_PORTE_PCR6              0x4004D018            // Pin Control Register n
#define REG_PORTE_PCR7              0x4004D01C            // Pin Control Register n
#define REG_PORTE_PCR8              0x4004D020            // Pin Control Register n
#define REG_PORTE_PCR9              0x4004D024            // Pin Control Register n
#define REG_PORTE_PCR10             0x4004D028            // Pin Control Register n
#define REG_PORTE_PCR11             0x4004D02C            // Pin Control Register n
#define REG_PORTE_PCR12             0x4004D030            // Pin Control Register n
#define REG_PORTE_PCR13             0x4004D034            // Pin Control Register n
#define REG_PORTE_PCR14             0x4004D038            // Pin Control Register n
#define REG_PORTE_PCR15             0x4004D03C            // Pin Control Register n
#define REG_PORTE_PCR16             0x4004D040            // Pin Control Register n
#define REG_PORTE_PCR17             0x4004D044            // Pin Control Register n
#define REG_PORTE_PCR18             0x4004D048            // Pin Control Register n
#define REG_PORTE_PCR19             0x4004D04C            // Pin Control Register n
#define REG_PORTE_PCR20             0x4004D050            // Pin Control Register n
#define REG_PORTE_PCR21             0x4004D054            // Pin Control Register n
#define REG_PORTE_PCR22             0x4004D058            // Pin Control Register n
#define REG_PORTE_PCR23             0x4004D05C            // Pin Control Register n
#define REG_PORTE_PCR24             0x4004D060            // Pin Control Register n
#define REG_PORTE_PCR25             0x4004D064            // Pin Control Register n
#define REG_PORTE_PCR26             0x4004D068            // Pin Control Register n
#define REG_PORTE_PCR27             0x4004D06C            // Pin Control Register n
#define REG_PORTE_PCR28             0x4004D070            // Pin Control Register n
#define REG_PORTE_PCR29             0x4004D074            // Pin Control Register n
#define REG_PORTE_PCR30             0x4004D078            // Pin Control Register n
#define REG_PORTE_PCR31             0x4004D07C            // Pin Control Register n
#define REG_PORTE_GPCLR             0x4004D080            // Global Pin Control Low Register
#define REG_PORTE_GPCHR             0x4004D084            // Global Pin Control High Register
#define REG_PORTE_ISFR              0x4004D0A0            // Interrupt Status Flag Register

// Chapter 12: System Integration Module (SIM)
#define REG_SIM_SOPT1               0x40047000        // System Options Register 1
#define REG_SIM_SOPT1CFG            0x40047004        // SOPT1 Configuration Register
#define REG_SIM_SOPT2               0x40048004        // System Options Register 2
#define REG_SIM_SOPT2_USBSRC        0x00040000        // 0=USB_CLKIN, 1=FFL/PLL
#define REG_SIM_SOPT2_PLLFLLSEL     0x00010000        // 0=FLL, 1=PLL
#define REG_SIM_SOPT2_TRACECLKSEL   0x00001000        // 0=MCGOUTCLK, 1=CPU
#define REG_SIM_SOPT2_PTD7PAD       0x00000800        // 0=normal, 1=double drive PTD7
#define REG_SIM_SOPT2_CLKOUTSEL(n)  (((n) & 7) << 5)  // Selects the clock to output on the CLKOUT pin.
#define REG_SIM_SOPT2_RTCCLKOUTSEL  0x00000010        // RTC clock out select
#define REG_SIM_SOPT4               0x4004800C        // System Options Register 4
#define REG_SIM_SOPT5               0x40048010        // System Options Register 5
#define REG_SIM_SOPT7               0x40048018        // System Options Register 7
#define REG_SIM_SDID                0x40048024        // System Device Identification Register
#define REG_SIM_SCGC4               0x40048034        // System Clock Gating Control Register 4
#define REG_SIM_SCGC4_VREF          0x00100000        // VREF Clock Gate Control
#define REG_SIM_SCGC4_CMP           0x00080000        // Comparator Clock Gate Control
#define REG_SIM_SCGC4_USBOTG        0x00040000        // USB Clock Gate Control
#define REG_SIM_SCGC4_UART2         0x00001000        // UART2 Clock Gate Control
#define REG_SIM_SCGC4_UART1         0x00000800        // UART1 Clock Gate Control
#define REG_SIM_SCGC4_UART0         0x00000400        // UART0 Clock Gate Control
#define REG_SIM_SCGC4_I2C0          0x00000040        // I2C0 Clock Gate Control
#define REG_SIM_SCGC4_CMT           0x00000004        // CMT Clock Gate Control
#define REG_SIM_SCGC4_EWM           0x00000002        // EWM Clock Gate Control
#define REG_SIM_SCGC5               0x40048038        // System Clock Gating Control Register 5
#define REG_SIM_SCGC5_PORTE         0x00002000        // Port E Clock Gate Control
#define REG_SIM_SCGC5_PORTD         0x00001000        // Port D Clock Gate Control
#define REG_SIM_SCGC5_PORTC         0x00000800        // Port C Clock Gate Control
#define REG_SIM_SCGC5_PORTB         0x00000400        // Port B Clock Gate Control
#define REG_SIM_SCGC5_PORTA         0x00000200        // Port A Clock Gate Control
#define REG_SIM_SCGC5_TSI           0x00000020        // Touch Sense Input TSI Clock Gate Control
#define REG_SIM_SCGC5_LPTIMER       0x00000001        // Low Power Timer Access Control
#define REG_SIM_SCGC6               0x4004803C        // System Clock Gating Control Register 6
#define REG_SIM_SCGC6_RTC           0x20000000        // RTC Access
#define REG_SIM_SCGC6_ADC0          0x08000000        // ADC0 Clock Gate Control
#define REG_SIM_SCGC6_FTM1          0x02000000        // FTM1 Clock Gate Control
#define REG_SIM_SCGC6_FTM0          0x01000000        // FTM0 Clock Gate Control
#define REG_SIM_SCGC6_PIT           0x00800000        // PIT Clock Gate Control
#define REG_SIM_SCGC6_PDB           0x00400000        // PDB Clock Gate Control
#define REG_SIM_SCGC6_USBDCD        0x00200000        // USB DCD Clock Gate Control
#define REG_SIM_SCGC6_CRC           0x00040000        // CRC Clock Gate Control
#define REG_SIM_SCGC6_I2S           0x00008000        // I2S Clock Gate Control
#define REG_SIM_SCGC6_SPI0          0x00001000        // SPI0 Clock Gate Control
#define REG_SIM_SCGC6_DMAMUX        0x00000002        // DMA Mux Clock Gate Control
#define REG_SIM_SCGC6_FTFL          0x00000001        // Flash Memory Clock Gate Control
#define REG_SIM_SCGC7               0x40048040        // System Clock Gating Control Register 7
#define REG_SIM_SCGC7_DMA           0x00000020        // DMA Clock Gate Control
#define REG_SIM_CLKDIV1             0x40048044            // System Clock Divider Register 1
#define REG_SIM_CLKDIV1_OUTDIV1(n)  (((n) & 0x0F) << 28)  // divide value for the core/system clock
#define REG_SIM_CLKDIV1_OUTDIV2(n)  (((n) & 0x0F) << 24)  // divide value for the peripheral clock
#define REG_SIM_CLKDIV1_OUTDIV4(n)  (((n) & 0x0F) << 16)  // divide value for the flash clock
#define REG_SIM_CLKDIV2             0x40048048            // System Clock Divider Register 2
#define REG_SIM_CLKDIV2_USBDIV(n)   (((n) & 0x07) << 1)
#define REG_SIM_CLKDIV2_USBFRAC     0x01
#define REG_SIM_FCFG1               0x4004804C        // Flash Configuration Register 1
#define REG_SIM_FCFG2               0x40048050        // Flash Configuration Register 2
#define REG_SIM_UIDH                0x40048054        // Unique Identification Register High
#define REG_SIM_UIDMH               0x40048058        // Unique Identification Register Mid-High
#define REG_SIM_UIDML               0x4004805C        // Unique Identification Register Mid Low
#define REG_SIM_UIDL                0x40048060        // Unique Identification Register Low

// Chapter 13: Reset Control Module (RCM)
#define REG_RCM_SRS0                0x4007F000        // System Reset Status Register 0
#define REG_RCM_SRS1                0x4007F001        // System Reset Status Register 1
#define REG_RCM_RPFC                0x4007F004        // Reset Pin Filter Control Register
#define REG_RCM_RPFW                0x4007F005        // Reset Pin Filter Width Register
#define REG_RCM_MR                  0x4007F007        // Mode Register

// Chapter 14: System Mode Controller
#define REG_SMC_PMPROT              0x4007E000          // Power Mode Protection Register
#define REG_SMC_PMPROT_AVLP         0x20                // Allow very low power modes
#define REG_SMC_PMPROT_ALLS         0x08                // Allow low leakage stop mode
#define REG_SMC_PMPROT_AVLLS        0x02                // Allow very low leakage stop mode
#define REG_SMC_PMCTRL              0x4007E001          // Power Mode Control Register
#define REG_SMC_PMCTRL_LPWUI        0x80                // Low Power Wake Up on Interrupt
#define REG_SMC_PMCTRL_RUNM(n)      (((n) & 0x03) << 5) // Run Mode Control
#define REG_SMC_PMCTRL_STOPA        0x08                // Stop Aborted
#define REG_SMC_PMCTRL_STOPM(n)     ((n) & 0x07)        // Stop Mode Control
#define REG_SMC_VLLSCTRL            0x4007E002          // VLLS Control Register
#define REG_SMC_VLLSCTRL_PORPO      0x20                // POR Power Option
#define REG_SMC_VLLSCTRL_VLLSM(n)   ((n) & 0x07)        // VLLS Mode Control
#define REG_SMC_PMSTAT              0x4007E003          // Power Mode Status Register
#define REG_SMC_PMSTAT_RUN          0x01                // Current power mode is RUN
#define REG_SMC_PMSTAT_STOP         0x02                // Current power mode is STOP
#define REG_SMC_PMSTAT_VLPR         0x04                // Current power mode is VLPR
#define REG_SMC_PMSTAT_VLPW         0x08                // Current power mode is VLPW
#define REG_SMC_PMSTAT_VLPS         0x10                // Current power mode is VLPS
#define REG_SMC_PMSTAT_LLS          0x20                // Current power mode is LLS
#define REG_SMC_PMSTAT_VLLS         0x40                // Current power mode is VLLS

// Chapter 15: Power Management Controller
#define REG_PMC_LVDSC1              0x4007D000          // Low Voltage Detect Status And Control 1 register
#define REG_PMC_LVDSC1_LVDF         0x80                // Low-Voltage Detect Flag
#define REG_PMC_LVDSC1_LVDACK       0x40                // Low-Voltage Detect Acknowledge
#define REG_PMC_LVDSC1_LVDIE        0x20                // Low-Voltage Detect Interrupt Enable
#define REG_PMC_LVDSC1_LVDRE        0x10                // Low-Voltage Detect Reset Enable
#define REG_PMC_LVDSC1_LVDV(n)      ((n) & 0x03)        // Low-Voltage Detect Voltage Select
#define REG_PMC_LVDSC2              0x4007D001          // Low Voltage Detect Status And Control 2 register
#define REG_PMC_LVDSC2_LVWF         0x80                // Low-Voltage Warning Flag
#define REG_PMC_LVDSC2_LVWACK       0x40                // Low-Voltage Warning Acknowledge
#define REG_PMC_LVDSC2_LVWIE        0x20                // Low-Voltage Warning Interrupt Enable
#define REG_PMC_LVDSC2_LVWV(n)      ((n) & 0x03)        // Low-Voltage Warning Voltage Select
#define REG_PMC_REGSC               0x4007D002          // Regulator Status And Control register
#define REG_PMC_REGSC_BGEN          0x10                // Bandgap Enable In VLPx Operation
#define REG_PMC_REGSC_ACKISO        0x08                // Acknowledge Isolation
#define REG_PMC_REGSC_REGONS        0x04                // Regulator In Run Regulation Status
#define REG_PMC_REGSC_BGBE          0x01                // Bandgap Buffer Enable

// Chapter 17: Miscellaneous Control Module (MCM)
#define REG_MCM_PLASC               0xE0080008          // Crossbar Switch (AXBS) Slave Configuration
#define REG_MCM_PLAMC               0xE008000A          // Crossbar Switch (AXBS) Master Configuration
#define REG_MCM_PLACR               0xE008000C          // Crossbar Switch (AXBS) Control Register

// Chapter 24: Multipurpose Clock Generator (MCG)
#define REG_MCG_C1                  0x40064000          // MCG Control 1 Register
#define REG_MCG_C1_IREFSTEN         0x01                // Internal Reference Stop Enable, Controls whether or not the internal reference clock remains enabled when the MCG enters Stop mode.
#define REG_MCG_C1_IRCLKEN          0x02                // Internal Reference Clock Enable, Enables the internal reference clock for use as MCGIRCLK.
#define REG_MCG_C1_IREFS            0x04                // Internal Reference Select, Selects the reference clock source for the FLL.
#define REG_MCG_C1_FRDIV(n)         (((n) & 0x07) << 3) // FLL External Reference Divider, Selects the amount to divide down the external reference clock for the FLL
#define REG_MCG_C1_CLKS(n)          (((n) & 0x03) << 6) // Clock Source Select, Selects the clock source for MCGOUTCLK
#define REG_MCG_C2                  0x40064001          // MCG Control 2 Register
#define REG_MCG_C2_IRCS             0x01                // Internal Reference Clock Select, Selects between the fast or slow internal reference clock source.
#define REG_MCG_C2_LP               0x02                // Low Power Select, Controls whether the FLL or PLL is disabled in BLPI and BLPE modes.
#define REG_MCG_C2_EREFS            0x04                // External Reference Select, Selects the source for the external reference clock.
#define REG_MCG_C2_HGO0             0x08                // High Gain Oscillator Select, Controls the crystal oscillator mode of operation
#define REG_MCG_C2_RANGE0(n)        (((n) & 0x03) << 4) // Frequency Range Select, Selects the frequency range for the crystal oscillator
#define REG_MCG_C2_LOCRE0           0x80                // Loss of Clock Reset Enable, Determines whether an interrupt or a reset request is made following a loss of OSC0
#define REG_MCG_C3                  0x40064002          // MCG Control 3 Register
#define REG_MCG_C3_SCTRIM(n)        (n)                 // Slow Internal Reference Clock Trim Setting
#define REG_MCG_C4                  0x40064003          // MCG Control 4 Register
#define REG_MCG_C4_SCFTRIM          0x01                // Slow Internal Reference Clock Fine Trim
#define REG_MCG_C4_FCTRIM(n)        (((n) & 0x0F) << 1) // Fast Internal Reference Clock Trim Setting
#define REG_MCG_C4_DRST_DRS(n)      (((n) & 0x03) << 5) // DCO Range Select
#define REG_MCG_C4_DMX32            0x80                // DCO Maximum Frequency with 32.768 kHz Reference, controls whether the DCO frequency range is narrowed
#define REG_MCG_C5                  0x40064004          // MCG Control 5 Register
#define REG_MCG_C5_PRDIV0(n)        ((n) & 0x1F)        // PLL External Reference Divider
#define REG_MCG_C5_PLLSTEN0         0x20                // PLL Stop Enable
#define REG_MCG_C5_PLLCLKEN0        0x40                // PLL Clock Enable
#define REG_MCG_C6                  0x40064005          // MCG Control 6 Register
#define REG_MCG_C6_VDIV0(n)         ((n) & 0x1F)        // VCO 0 Divider
#define REG_MCG_C6_CME0             0x20                // Clock Monitor Enable
#define REG_MCG_C6_PLLS             0x40                // PLL Select, Controls whether the PLL or FLL output is selected as the MCG source when CLKS[1:0]=00.
#define REG_MCG_C6_LOLIE0           0x80                // Loss of Lock Interrrupt Enable
#define REG_MCG_S                   0x40064006          // MCG Status Register
#define REG_MCG_S_IRCST             0x01                // Internal Reference Clock Status
#define REG_MCG_S_OSCINIT0          0x02                // OSC Initialization,  resets to 0, is set to 1 after the initialization cycles of the crystal oscillator
#define REG_MCG_S_CLKST(n)          (((n) & 0x03) << 2) // Clock Mode Status, 0=FLL is selected, 1= Internal ref, 2=External ref, 3=PLL
#define REG_MCG_S_CLKST_MASK        0x0C
#define REG_MCG_S_IREFST            0x10                // Internal Reference Status
#define REG_MCG_S_PLLST             0x20                // PLL Select Status
#define REG_MCG_S_LOCK0             0x40                // Lock Status, 0=PLL Unlocked, 1=PLL Locked
#define REG_MCG_S_LOLS0             0x80                // Loss of Lock Status
#define REG_MCG_SC                  0x40064008          // MCG Status and Control Register
#define REG_MCG_SC_LOCS0            0x01                // OSC0 Loss of Clock Status
#define REG_MCG_SC_FCRDIV(n)        (((n) & 0x07) << 1) // Fast Clock Internal Reference Divider
#define REG_MCG_SC_FLTPRSRV         0x10                // FLL Filter Preserve Enable
#define REG_MCG_SC_ATMF             0x20                // Automatic Trim Machine Fail Flag
#define REG_MCG_SC_ATMS             0x40                // Automatic Trim Machine Select
#define REG_MCG_SC_ATME             0x80                // Automatic Trim Machine Enable
#define REG_MCG_ATCVH               0x4006400A          // MCG Auto Trim Compare Value High Register
#define REG_MCG_ATCVL               0x4006400B          // MCG Auto Trim Compare Value Low Register
#define REG_MCG_C7                  0x4006400C          // MCG Control 7 Register
#define REG_MCG_C8                  0x4006400D          // MCG Control 8 Register

// Chapter 25: Oscillator (OSC)
#define REG_OSC0_CR                 0x40065000          // OSC Control Register
#define REG_OSC_SC16P               0x01                // Oscillator 16 pF Capacitor Load Configure
#define REG_OSC_SC8P                0x02                // Oscillator 8 pF Capacitor Load Configure
#define REG_OSC_SC4P                0x04                // Oscillator 4 pF Capacitor Load Configure
#define REG_OSC_SC2P                0x08                // Oscillator 2 pF Capacitor Load Configure
#define REG_OSC_EREFSTEN            0x20                // External Reference Stop Enable, Controls whether or not the external reference clock (OSCERCLK) remains enabled when MCU enters Stop mode.
#define REG_OSC_ERCLKEN             0x80                // External Reference Enable, Enables external reference clock (OSCERCLK).

// Chapter 27: Flash Memory Controller (FMC)
#define REG_FMC_PFAPR               0x4001F000          // Flash Access Protection
#define REG_FMC_PFB0CR              0x4001F004          // Flash Control
#define REG_FMC_TAGVDW0S0           0x4001F100          // Cache Tag Storage
#define REG_FMC_TAGVDW0S1           0x4001F104          // Cache Tag Storage
#define REG_FMC_TAGVDW1S0           0x4001F108          // Cache Tag Storage
#define REG_FMC_TAGVDW1S1           0x4001F10C          // Cache Tag Storage
#define REG_FMC_TAGVDW2S0           0x4001F110          // Cache Tag Storage
#define REG_FMC_TAGVDW2S1           0x4001F114          // Cache Tag Storage
#define REG_FMC_TAGVDW3S0           0x4001F118          // Cache Tag Storage
#define REG_FMC_TAGVDW3S1           0x4001F11C          // Cache Tag Storage
#define REG_FMC_DATAW0S0            0x4001F200          // Cache Data Storage
#define REG_FMC_DATAW0S1            0x4001F204          // Cache Data Storage
#define REG_FMC_DATAW1S0            0x4001F208          // Cache Data Storage
#define REG_FMC_DATAW1S1            0x4001F20C          // Cache Data Storage
#define REG_FMC_DATAW2S0            0x4001F210          // Cache Data Storage
#define REG_FMC_DATAW2S1            0x4001F214          // Cache Data Storage
#define REG_FMC_DATAW3S0            0x4001F218          // Cache Data Storage
#define REG_FMC_DATAW3S1            0x4001F21C          // Cache Data Storage

// Chapter 28: Flash Memory Module (FTFL)
#define REG_FTFL_FSTAT              0x40020000          // Flash Status Register
#define REG_FTFL_FSTAT_CCIF         0x80                // Command Complete Interrupt Flag
#define REG_FTFL_FSTAT_RDCOLERR     0x40                // Flash Read Collision Error Flag
#define REG_FTFL_FSTAT_ACCERR       0x20                // Flash Access Error Flag
#define REG_FTFL_FSTAT_FPVIOL       0x10                // Flash Protection Violation Flag
#define REG_FTFL_FSTAT_MGSTAT0      0x01                // Memory Controller Command Completion Status Flag
#define REG_FTFL_FCNFG              0x40020001          // Flash Configuration Register
#define REG_FTFL_FCNFG_CCIE         0x80                // Command Complete Interrupt Enable
#define REG_FTFL_FCNFG_RDCOLLIE     0x40                // Read Collision Error Interrupt Enable
#define REG_FTFL_FCNFG_ERSAREQ      0x20                // Erase All Request
#define REG_FTFL_FCNFG_ERSSUSP      0x10                // Erase Suspend
#define REG_FTFL_FCNFG_PFLSH        0x04                // Flash memory configuration
#define REG_FTFL_FCNFG_RAMRDY       0x02                // RAM Ready
#define REG_FTFL_FCNFG_EEERDY       0x01                // EEPROM Ready
#define REG_FTFL_FSEC               0x40020002          // Flash Security Register
#define REG_FTFL_FOPT               0x40020003          // Flash Option Register0x18
#define REG_FTFL_FCCOB3             0x40020004          // Flash Common Command Object Registers
#define REG_FTFL_FCCOB2             0x40020005
#define REG_FTFL_FCCOB1             0x40020006
#define REG_FTFL_FCCOB0             0x40020007
#define REG_FTFL_FCCOB7             0x40020008
#define REG_FTFL_FCCOB6             0x40020009
#define REG_FTFL_FCCOB5             0x4002000A
#define REG_FTFL_FCCOB4             0x4002000B
#define REG_FTFL_FCCOBB             0x4002000C
#define REG_FTFL_FCCOBA             0x4002000D
#define REG_FTFL_FCCOB9             0x4002000E
#define REG_FTFL_FCCOB8             0x4002000F
#define REG_FTFL_FPROT3             0x40020010          // Program Flash Protection Registers
#define REG_FTFL_FPROT2             0x40020011          // Program Flash Protection Registers
#define REG_FTFL_FPROT1             0x40020012          // Program Flash Protection Registers
#define REG_FTFL_FPROT0             0x40020013          // Program Flash Protection Registers
#define REG_FTFL_FEPROT             0x40020016          // EEPROM Protection Register
#define REG_FTFL_FDPROT             0x40020017          // Data Flash Protection Register

// Chapter 47: General-Purpose Input/Output (GPIO)
#define REG_GPIOA_PDOR              0x400FF000          // Port Data Output Register
#define REG_GPIOA_PSOR              0x400FF004          // Port Set Output Register
#define REG_GPIOA_PCOR              0x400FF008          // Port Clear Output Register
#define REG_GPIOA_PTOR              0x400FF00C          // Port Toggle Output Register
#define REG_GPIOA_PDIR              0x400FF010          // Port Data Input Register
#define REG_GPIOA_PDDR              0x400FF014          // Port Data Direction Register
#define REG_GPIOB_PDOR              0x400FF040          // Port Data Output Register
#define REG_GPIOB_PSOR              0x400FF044          // Port Set Output Register
#define REG_GPIOB_PCOR              0x400FF048          // Port Clear Output Register
#define REG_GPIOB_PTOR              0x400FF04C          // Port Toggle Output Register
#define REG_GPIOB_PDIR              0x400FF050          // Port Data Input Register
#define REG_GPIOB_PDDR              0x400FF054          // Port Data Direction Register
#define REG_GPIOC_PDOR              0x400FF080          // Port Data Output Register
#define REG_GPIOC_PSOR              0x400FF084          // Port Set Output Register
#define REG_GPIOC_PCOR              0x400FF088          // Port Clear Output Register
#define REG_GPIOC_PTOR              0x400FF08C          // Port Toggle Output Register
#define REG_GPIOC_PDIR              0x400FF090          // Port Data Input Register
#define REG_GPIOC_PDDR              0x400FF094          // Port Data Direction Register
#define REG_GPIOD_PDOR              0x400FF0C0          // Port Data Output Register
#define REG_GPIOD_PSOR              0x400FF0C4          // Port Set Output Register
#define REG_GPIOD_PCOR              0x400FF0C8          // Port Clear Output Register
#define REG_GPIOD_PTOR              0x400FF0CC          // Port Toggle Output Register
#define REG_GPIOD_PDIR              0x400FF0D0          // Port Data Input Register
#define REG_GPIOD_PDDR              0x400FF0D4          // Port Data Direction Register
#define REG_GPIOE_PDOR              0x400FF100          // Port Data Output Register
#define REG_GPIOE_PSOR              0x400FF104          // Port Set Output Register
#define REG_GPIOE_PCOR              0x400FF108          // Port Clear Output Register
#define REG_GPIOE_PTOR              0x400FF10C          // Port Toggle Output Register
#define REG_GPIOE_PDIR              0x400FF110          // Port Data Input Register
#define REG_GPIOE_PDDR              0x400FF114          // Port Data Direction Register

// System Control Space (SCS), ARMv7 ref manual, B3.2, page 708
#define REG_SCB_CPUID               0xE000ED00          // CPUID Base Register
#define REG_SCB_ICSR                0xE000ED04          // Interrupt Control and State
#define REG_SCB_ICSR_PENDSTSET      0x04000000
#define REG_SCB_VTOR                0xE000ED08          // Vector Table Offset
#define REG_SCB_AIRCR               0xE000ED0C          // Application Interrupt and Reset Control
#define REG_SCB_SCR                 0xE000ED10          // System Control Register
#define REG_SCB_CCR                 0xE000ED14          // Configuration and Control
#define REG_SCB_SHPR1               0xE000ED18          // System Handler Priority Register 1
#define REG_SCB_SHPR2               0xE000ED1C          // System Handler Priority Register 2
#define REG_SCB_SHPR3               0xE000ED20          // System Handler Priority Register 3
#define REG_SCB_SHCSR               0xE000ED24          // System Handler Control and State
#define REG_SCB_CFSR                0xE000ED28          // Configurable Fault Status Register
#define REG_SCB_HFSR                0xE000ED2C          // HardFault Status
#define REG_SCB_DFSR                0xE000ED30          // Debug Fault Status
#define REG_SCB_MMFAR               0xE000ED34          // MemManage Fault Address
#define REG_SCB_DHCSR               0xE000EDF0          // Debug Halting Control and Status Register
#define REG_SCB_DCRSR               0xE000EDF4          // Debug Core Register Selector Register
#define REG_SCB_DCRDR               0xE000EDF8          // Debug Core Register Data Register
#define REG_SCB_DEMCR               0xE000EDFC          // Debug Exception and Monitor Control Register

#define REG_SYST_CSR                0xE000E010          // SysTick Control and Status
#define REG_SYST_CSR_COUNTFLAG      0x00010000
#define REG_SYST_CSR_CLKSOURCE      0x00000004
#define REG_SYST_CSR_TICKINT        0x00000002
#define REG_SYST_CSR_ENABLE         0x00000001
#define REG_SYST_RVR                0xE000E014          // SysTick Reload Value Register
#define REG_SYST_CVR                0xE000E018          // SysTick Current Value Register
#define REG_SYST_CALIB              0xE000E01C          // SysTick Calibration Value

#define REG_ARM_DEMCR               0xE000EDFC          // Debug Exception and Monitor Control
#define REG_ARM_DEMCR_TRCENA        (1 << 24)           // Enable debugging & monitoring blocks
#define REG_ARM_DWT_CTRL            0xE0001000          // DWT control register
#define REG_ARM_DWT_CTRL_CYCCNTENA  (1 << 0)            // Enable cycle count
#define REG_ARM_DWT_CYCCNT          0xE0001004          // Cycle count register

// Chapter 40: Universal Serial Bus OTG Controller (USBOTG)
#define REG_USB0_PERID                  0x40072000 // Peripheral ID register
#define REG_USB0_IDCOMP                 0x40072004 // Peripheral ID Complement register
#define REG_USB0_REV                    0x40072008 // Peripheral Revision register
#define REG_USB0_ADDINFO                0x4007200C // Peripheral Additional Info register
#define REG_USB0_OTGISTAT               0x40072010 // OTG Interrupt Status register
#define REG_USB_OTGISTAT_IDCHG          (uint8_t)0x80           //
#define REG_USB_OTGISTAT_ONEMSEC        (uint8_t)0x40           //
#define REG_USB_OTGISTAT_LINE_STATE_CHG (uint8_t)0x20           //
#define REG_USB_OTGISTAT_SESSVLDCHG     (uint8_t)0x08           //
#define REG_USB_OTGISTAT_B_SESS_CHG     (uint8_t)0x04           //
#define REG_USB_OTGISTAT_AVBUSCHG       (uint8_t)0x01           //
#define REG_USB0_OTGICR                 0x40072014 // OTG Interrupt Control Register
#define REG_USB_OTGICR_IDEN             (uint8_t)0x80           //
#define REG_USB_OTGICR_ONEMSECEN        (uint8_t)0x40           //
#define REG_USB_OTGICR_LINESTATEEN      (uint8_t)0x20           //
#define REG_USB_OTGICR_SESSVLDEN        (uint8_t)0x08           //
#define REG_USB_OTGICR_BSESSEN          (uint8_t)0x04           //
#define REG_USB_OTGICR_AVBUSEN          (uint8_t)0x01           //
#define REG_USB0_OTGSTAT                0x40072018 // OTG Status register
#define REG_USB_OTGSTAT_ID              (uint8_t)0x80           //
#define REG_USB_OTGSTAT_ONEMSECEN       (uint8_t)0x40           //
#define REG_USB_OTGSTAT_LINESTATESTABLE (uint8_t)0x20           //
#define REG_USB_OTGSTAT_SESS_VLD        (uint8_t)0x08           //
#define REG_USB_OTGSTAT_BSESSEND        (uint8_t)0x04           //
#define REG_USB_OTGSTAT_AVBUSVLD        (uint8_t)0x01           //
#define REG_USB0_OTGCTL                 0x4007201C // OTG Control Register
#define REG_USB_OTGCTL_DPHIGH           (uint8_t)0x80           //
#define REG_USB_OTGCTL_DPLOW            (uint8_t)0x20           //
#define REG_USB_OTGCTL_DMLOW            (uint8_t)0x10           //
#define REG_USB_OTGCTL_OTGEN            (uint8_t)0x04           //
#define REG_USB0_ISTAT                  0x40072080 // Interrupt Status Register
#define REG_USB_ISTAT_STALL             (uint8_t)0x80           //
#define REG_USB_ISTAT_ATTACH            (uint8_t)0x40           //
#define REG_USB_ISTAT_RESUME            (uint8_t)0x20           //
#define REG_USB_ISTAT_SLEEP             (uint8_t)0x10           //
#define REG_USB_ISTAT_TOKDNE            (uint8_t)0x08           //
#define REG_USB_ISTAT_SOFTOK            (uint8_t)0x04           //
#define REG_USB_ISTAT_ERROR             (uint8_t)0x02           //
#define REG_USB_ISTAT_USBRST            (uint8_t)0x01           //
#define REG_USB0_INTEN                  0x40072084 // Interrupt Enable Register
#define REG_USB_INTEN_STALLEN           (uint8_t)0x80           //
#define REG_USB_INTEN_ATTACHEN          (uint8_t)0x40           //
#define REG_USB_INTEN_RESUMEEN          (uint8_t)0x20           //
#define REG_USB_INTEN_SLEEPEN           (uint8_t)0x10           //
#define REG_USB_INTEN_TOKDNEEN          (uint8_t)0x08           //
#define REG_USB_INTEN_SOFTOKEN          (uint8_t)0x04           //
#define REG_USB_INTEN_ERROREN           (uint8_t)0x02           //
#define REG_USB_INTEN_USBRSTEN          (uint8_t)0x01           //
#define REG_USB0_ERRSTAT                0x40072088 // Error Interrupt Status Register
#define REG_USB_ERRSTAT_BTSERR          (uint8_t)0x80           //
#define REG_USB_ERRSTAT_DMAERR          (uint8_t)0x20           //
#define REG_USB_ERRSTAT_BTOERR          (uint8_t)0x10           //
#define REG_USB_ERRSTAT_DFN8            (uint8_t)0x08           //
#define REG_USB_ERRSTAT_CRC16           (uint8_t)0x04           //
#define REG_USB_ERRSTAT_CRC5EOF         (uint8_t)0x02           //
#define REG_USB_ERRSTAT_PIDERR          (uint8_t)0x01           //
#define REG_USB0_ERREN                  0x4007208C // Error Interrupt Enable Register
#define REG_USB_ERREN_BTSERREN          (uint8_t)0x80           //
#define REG_USB_ERREN_DMAERREN          (uint8_t)0x20           //
#define REG_USB_ERREN_BTOERREN          (uint8_t)0x10           //
#define REG_USB_ERREN_DFN8EN            (uint8_t)0x08           //
#define REG_USB_ERREN_CRC16EN           (uint8_t)0x04           //
#define REG_USB_ERREN_CRC5EOFEN         (uint8_t)0x02           //
#define REG_USB_ERREN_PIDERREN          (uint8_t)0x01           //
#define REG_USB0_STAT                   0x40072090 // Status Register
#define REG_USB_STAT_TX                 (uint8_t)0x08           //
#define REG_USB_STAT_ODD                (uint8_t)0x04           //
#define REG_USB_STAT_ENDP(n)            (uint8_t)((n) >> 4)     //
#define REG_USB0_CTL                    0x40072094 // Control Register
#define REG_USB_CTL_JSTATE              (uint8_t)0x80           //
#define REG_USB_CTL_SE0                 (uint8_t)0x40           //
#define REG_USB_CTL_TXSUSPENDTOKENBUSY  (uint8_t)0x20           //
#define REG_USB_CTL_RESET               (uint8_t)0x10           //
#define REG_USB_CTL_HOSTMODEEN          (uint8_t)0x08           //
#define REG_USB_CTL_RESUME              (uint8_t)0x04           //
#define REG_USB_CTL_ODDRST              (uint8_t)0x02           //
#define REG_USB_CTL_USBENSOFEN          (uint8_t)0x01           //
#define REG_USB0_ADDR                   0x40072098 // Address Register
#define REG_USB0_BDTPAGE1               0x4007209C // BDT Page Register 1
#define REG_USB0_FRMNUML                0x400720A0 // Frame Number Register Low
#define REG_USB0_FRMNUMH                0x400720A4 // Frame Number Register High
#define REG_USB0_TOKEN                  0x400720A8 // Token Register
#define REG_USB0_SOFTHLD                0x400720AC // SOF Threshold Register
#define REG_USB0_BDTPAGE2               0x400720B0 // BDT Page Register 2
#define REG_USB0_BDTPAGE3               0x400720B4 // BDT Page Register 3
#define REG_USB0_ENDPT0                 0x400720C0 // Endpoint Control Register
#define REG_USB_ENDPT_HOSTWOHUB         (uint8_t)0x80           // host only, enable low speed
#define REG_USB_ENDPT_RETRYDIS          (uint8_t)0x40           // host only, set to disable NAK retry
#define REG_USB_ENDPT_EPCTLDIS          (uint8_t)0x10           // 0=control, 1=bulk, interrupt, isync
#define REG_USB_ENDPT_EPRXEN            (uint8_t)0x08           // enables the endpoint for RX transfers.
#define REG_USB_ENDPT_EPTXEN            (uint8_t)0x04           // enables the endpoint for TX transfers.
#define REG_USB_ENDPT_EPSTALL           (uint8_t)0x02           // set to stall endpoint
#define REG_USB_ENDPT_EPHSHK            (uint8_t)0x01           // enable handshaking during a transaction, generally set unless Isochronous
#define REG_USB0_ENDPT1                 0x400720C4 // Endpoint Control Register
#define REG_USB0_ENDPT2                 0x400720C8 // Endpoint Control Register
#define REG_USB0_ENDPT3                 0x400720CC // Endpoint Control Register
#define REG_USB0_ENDPT4                 0x400720D0 // Endpoint Control Register
#define REG_USB0_ENDPT5                 0x400720D4 // Endpoint Control Register
#define REG_USB0_ENDPT6                 0x400720D8 // Endpoint Control Register
#define REG_USB0_ENDPT7                 0x400720DC // Endpoint Control Register
#define REG_USB0_ENDPT8                 0x400720E0 // Endpoint Control Register
#define REG_USB0_ENDPT9                 0x400720E4 // Endpoint Control Register
#define REG_USB0_ENDPT10                0x400720E8 // Endpoint Control Register
#define REG_USB0_ENDPT11                0x400720EC // Endpoint Control Register
#define REG_USB0_ENDPT12                0x400720F0 // Endpoint Control Register
#define REG_USB0_ENDPT13                0x400720F4 // Endpoint Control Register
#define REG_USB0_ENDPT14                0x400720F8 // Endpoint Control Register
#define REG_USB0_ENDPT15                0x400720FC // Endpoint Control Register
#define REG_USB0_USBCTRL                0x40072100 // USB Control Register
#define REG_USB_USBCTRL_SUSP            (uint8_t)0x80           // Places the USB transceiver into the suspend state.
#define REG_USB_USBCTRL_PDE             (uint8_t)0x40           // Enables the weak pulldowns on the USB transceiver.
#define REG_USB0_OBSERVE                0x40072104 // USB OTG Observe Register
#define REG_USB_OBSERVE_DPPU            (uint8_t)0x80           //
#define REG_USB_OBSERVE_DPPD            (uint8_t)0x40           //
#define REG_USB_OBSERVE_DMPD            (uint8_t)0x10           //
#define REG_USB0_CONTROL                0x40072108 // USB OTG Control Register
#define REG_USB_CONTROL_DPPULLUPNONOTG  (uint8_t)0x10           //  Provides control of the DP PULLUP in the USB OTG module, if USB is configured in non-OTG device mode.
#define REG_USB0_USBTRC0                0x4007210C // USB Transceiver Control Register 0
#define REG_USB_USBTRC_USBRESET         (uint8_t)0x80           //
#define REG_USB_USBTRC_USBRESMEN        (uint8_t)0x20           //
#define REG_USB_USBTRC_SYNC_DET         (uint8_t)0x02           //
#define REG_USB_USBTRC_USB_RESUME_INT   (uint8_t)0x01           //
#define REG_USB0_USBFRMADJUST           0x40072114 // Frame Adjust Register

// FlexRAM base address
#define REG_FLEXRAM_BASE            0x14000000
#define REG_APPLICATION_BASE        0x00000000

// MDM-AP debug port registers
#define REG_MDM_STATUS              0x01000000
#define REG_MDM_CONTROL             0x01000004
#define REG_MDM_IDR                 0x010000FC

// MDM-AP status bits
#define REG_MDM_STATUS_FLASH_ERASE_ACK      (1 << 0)
#define REG_MDM_STATUS_FLASH_READY          (1 << 1)
#define REG_MDM_STATUS_SYS_SECURITY         (1 << 2)
#define REG_MDM_STATUS_SYS_NRESET           (1 << 3)
#define REG_MDM_STATUS_MASS_ERASE_ENABLE    (1 << 5)
#define REG_MDM_STATUS_BACKDOOR_ENABLE      (1 << 6)
#define REG_MDM_STATUS_LP_ENABLE            (1 << 7)
#define REG_MDM_STATUS_VLP_ENABLE           (1 << 8)
#define REG_MDM_STATUS_LLS_EXIT             (1 << 9)
#define REG_MDM_STATUS_VLLS_EXIT            (1 << 10)
#define REG_MDM_STATUS_CORE_HALTED          (1 << 16)
#define REG_MDM_STATUS_CORE_SLEEPDEEP       (1 << 17)
#define REG_MDM_STATUS_CORE_SLEEPING        (1 << 18)

// MDM-AP control bits
#define REG_MDM_CONTROL_MASS_ERASE          (1 << 0)
#define REG_MDM_CONTROL_DEBUG_DISABLE       (1 << 1)
#define REG_MDM_CONTROL_DEBUG_REQ           (1 << 2)
#define REG_MDM_CONTROL_SYS_RESET_REQ       (1 << 3)
#define REG_MDM_CONTROL_CORE_HOLD_RESET     (1 << 4)
#define REG_MDM_CONTROL_VLLS_DEBUG_REQ      (1 << 5)
#define REG_MDM_CONTROL_VLLS_DEBUG_ACK      (1 << 6)
#define REG_MDM_CONTROL_LLS_ACK             (1 << 7)
#define REG_MDM_CONTROL_TIMESTAMP_DISABLE   (1 << 8)
