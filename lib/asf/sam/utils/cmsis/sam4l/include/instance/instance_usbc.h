/**
 * \file
 *
 * \brief Instance description for USBC
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAM4L_USBC_INSTANCE_
#define _SAM4L_USBC_INSTANCE_

/* ========== Register definition for USBC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USBC_UDCON             (0x400A5000U) /**< \brief (USBC) Device General Control Register */
#define REG_USBC_UDINT             (0x400A5004U) /**< \brief (USBC) Device Global Interupt Register */
#define REG_USBC_UDINTCLR          (0x400A5008U) /**< \brief (USBC) Device Global Interrupt Clear Register */
#define REG_USBC_UDINTSET          (0x400A500CU) /**< \brief (USBC) Device Global Interrupt Set Regsiter */
#define REG_USBC_UDINTE            (0x400A5010U) /**< \brief (USBC) Device Global Interrupt Enable Register */
#define REG_USBC_UDINTECLR         (0x400A5014U) /**< \brief (USBC) Device Global Interrupt Enable Clear Register */
#define REG_USBC_UDINTESET         (0x400A5018U) /**< \brief (USBC) Device Global Interrupt Enable Set Register */
#define REG_USBC_UERST             (0x400A501CU) /**< \brief (USBC) Endpoint Enable/Reset Register */
#define REG_USBC_UDFNUM            (0x400A5020U) /**< \brief (USBC) Device Frame Number Register */
#define REG_USBC_UECFG0            (0x400A5100U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG1            (0x400A5104U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG2            (0x400A5108U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG3            (0x400A510CU) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG4            (0x400A5110U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG5            (0x400A5114U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG6            (0x400A5118U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG7            (0x400A511CU) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UESTA0            (0x400A5130U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA1            (0x400A5134U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA2            (0x400A5138U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA3            (0x400A513CU) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA4            (0x400A5140U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA5            (0x400A5144U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA6            (0x400A5148U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA7            (0x400A514CU) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA0CLR         (0x400A5160U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA1CLR         (0x400A5164U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA2CLR         (0x400A5168U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA3CLR         (0x400A516CU) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA4CLR         (0x400A5170U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA5CLR         (0x400A5174U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA6CLR         (0x400A5178U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA7CLR         (0x400A517CU) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA0SET         (0x400A5190U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA1SET         (0x400A5194U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA2SET         (0x400A5198U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA3SET         (0x400A519CU) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA4SET         (0x400A51A0U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA5SET         (0x400A51A4U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA6SET         (0x400A51A8U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA7SET         (0x400A51ACU) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UECON0            (0x400A51C0U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON1            (0x400A51C4U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON2            (0x400A51C8U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON3            (0x400A51CCU) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON4            (0x400A51D0U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON5            (0x400A51D4U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON6            (0x400A51D8U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON7            (0x400A51DCU) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON0SET         (0x400A51F0U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON1SET         (0x400A51F4U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON2SET         (0x400A51F8U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON3SET         (0x400A51FCU) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON4SET         (0x400A5200U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON5SET         (0x400A5204U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON6SET         (0x400A5208U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON7SET         (0x400A520CU) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON0CLR         (0x400A5220U) /**< \brief (USBC) Endpoint Control Clear Register */
#define REG_USBC_UECON1CLR         (0x400A5224U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON2CLR         (0x400A5228U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON3CLR         (0x400A522CU) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON4CLR         (0x400A5230U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON5CLR         (0x400A5234U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON6CLR         (0x400A5238U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON7CLR         (0x400A523CU) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UHCON             (0x400A5400U) /**< \brief (USBC) Host General Control Register */
#define REG_USBC_UHINT             (0x400A5404U) /**< \brief (USBC) Host Global Interrupt Register */
#define REG_USBC_UHINTCLR          (0x400A5408U) /**< \brief (USBC) Host Global Interrrupt Clear Register */
#define REG_USBC_UHINTSET          (0x400A540CU) /**< \brief (USBC) Host Global Interrupt Set Register */
#define REG_USBC_UHINTE            (0x400A5410U) /**< \brief (USBC) Host Global Interrupt Enable Register */
#define REG_USBC_UHINTECLR         (0x400A5414U) /**< \brief (USBC) Host Global Interrupt Enable Clear Register */
#define REG_USBC_UHINTESET         (0x400A5418U) /**< \brief (USBC) Host Global Interrupt Enable Set Register */
#define REG_USBC_UPRST             (0x400A541CU) /**< \brief (USBC) Pipe Reset Register */
#define REG_USBC_UHFNUM            (0x400A5420U) /**< \brief (USBC) Host Frame Number Register */
#define REG_USBC_UHSOFC            (0x400A5424U) /**< \brief (USBC) Host Start of Frame Control Register */
#define REG_USBC_UPCFG0            (0x400A5500U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG1            (0x400A5504U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG2            (0x400A5508U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG3            (0x400A550CU) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG4            (0x400A5510U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG5            (0x400A5514U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG6            (0x400A5518U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG7            (0x400A551CU) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPSTA0            (0x400A5530U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA1            (0x400A5534U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA2            (0x400A5538U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA3            (0x400A553CU) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA4            (0x400A5540U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA5            (0x400A5544U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA6            (0x400A5548U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA7            (0x400A554CU) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA0CLR         (0x400A5560U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA1CLR         (0x400A5564U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA2CLR         (0x400A5568U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA3CLR         (0x400A556CU) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA4CLR         (0x400A5570U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA5CLR         (0x400A5574U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA6CLR         (0x400A5578U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA7CLR         (0x400A557CU) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA0SET         (0x400A5590U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA1SET         (0x400A5594U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA2SET         (0x400A5598U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA3SET         (0x400A559CU) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA4SET         (0x400A55A0U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA5SET         (0x400A55A4U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA6SET         (0x400A55A8U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA7SET         (0x400A55ACU) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPCON0            (0x400A55C0U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON1            (0x400A55C4U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON2            (0x400A55C8U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON3            (0x400A55CCU) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON4            (0x400A55D0U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON5            (0x400A55D4U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON6            (0x400A55D8U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON7            (0x400A55DCU) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON0SET         (0x400A55F0U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON1SET         (0x400A55F4U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON2SET         (0x400A55F8U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON3SET         (0x400A55FCU) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON4SET         (0x400A5600U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON5SET         (0x400A5604U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON6SET         (0x400A5608U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON7SET         (0x400A560CU) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON0CLR         (0x400A5620U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON1CLR         (0x400A5624U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON2CLR         (0x400A5628U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON3CLR         (0x400A562CU) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON4CLR         (0x400A5630U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON5CLR         (0x400A5634U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON6CLR         (0x400A5638U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON7CLR         (0x400A563CU) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPINRQ0           (0x400A5650U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ1           (0x400A5654U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ2           (0x400A5658U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ3           (0x400A565CU) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ4           (0x400A5660U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ5           (0x400A5664U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ6           (0x400A5668U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ7           (0x400A566CU) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_USBCON            (0x400A5800U) /**< \brief (USBC) General Control Register */
#define REG_USBC_USBSTA            (0x400A5804U) /**< \brief (USBC) General Status Register */
#define REG_USBC_USBSTACLR         (0x400A5808U) /**< \brief (USBC) General Status Clear Register */
#define REG_USBC_USBSTASET         (0x400A580CU) /**< \brief (USBC) General Status Set Register */
#define REG_USBC_UVERS             (0x400A5818U) /**< \brief (USBC) IP Version Register */
#define REG_USBC_UFEATURES         (0x400A581CU) /**< \brief (USBC) IP Features Register */
#define REG_USBC_UADDRSIZE         (0x400A5820U) /**< \brief (USBC) IP PB address size Register */
#define REG_USBC_UNAME1            (0x400A5824U) /**< \brief (USBC) IP Name Part One: HUSB */
#define REG_USBC_UNAME2            (0x400A5828U) /**< \brief (USBC) IP Name Part Two: HOST */
#define REG_USBC_USBFSM            (0x400A582CU) /**< \brief (USBC) USB internal finite state machine */
#define REG_USBC_UDESC             (0x400A5830U) /**< \brief (USBC) Endpoint descriptor table */
#else
#define REG_USBC_UDCON             (*(RwReg  *)0x400A5000U) /**< \brief (USBC) Device General Control Register */
#define REG_USBC_UDINT             (*(RoReg  *)0x400A5004U) /**< \brief (USBC) Device Global Interupt Register */
#define REG_USBC_UDINTCLR          (*(WoReg  *)0x400A5008U) /**< \brief (USBC) Device Global Interrupt Clear Register */
#define REG_USBC_UDINTSET          (*(WoReg  *)0x400A500CU) /**< \brief (USBC) Device Global Interrupt Set Regsiter */
#define REG_USBC_UDINTE            (*(RoReg  *)0x400A5010U) /**< \brief (USBC) Device Global Interrupt Enable Register */
#define REG_USBC_UDINTECLR         (*(WoReg  *)0x400A5014U) /**< \brief (USBC) Device Global Interrupt Enable Clear Register */
#define REG_USBC_UDINTESET         (*(WoReg  *)0x400A5018U) /**< \brief (USBC) Device Global Interrupt Enable Set Register */
#define REG_USBC_UERST             (*(RwReg  *)0x400A501CU) /**< \brief (USBC) Endpoint Enable/Reset Register */
#define REG_USBC_UDFNUM            (*(RoReg  *)0x400A5020U) /**< \brief (USBC) Device Frame Number Register */
#define REG_USBC_UECFG0            (*(RwReg  *)0x400A5100U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG1            (*(RwReg  *)0x400A5104U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG2            (*(RwReg  *)0x400A5108U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG3            (*(RwReg  *)0x400A510CU) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG4            (*(RwReg  *)0x400A5110U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG5            (*(RwReg  *)0x400A5114U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG6            (*(RwReg  *)0x400A5118U) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UECFG7            (*(RwReg  *)0x400A511CU) /**< \brief (USBC) Endpoint Configuration Register */
#define REG_USBC_UESTA0            (*(RoReg  *)0x400A5130U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA1            (*(RoReg  *)0x400A5134U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA2            (*(RoReg  *)0x400A5138U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA3            (*(RoReg  *)0x400A513CU) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA4            (*(RoReg  *)0x400A5140U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA5            (*(RoReg  *)0x400A5144U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA6            (*(RoReg  *)0x400A5148U) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA7            (*(RoReg  *)0x400A514CU) /**< \brief (USBC) Endpoint Status Register */
#define REG_USBC_UESTA0CLR         (*(WoReg  *)0x400A5160U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA1CLR         (*(WoReg  *)0x400A5164U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA2CLR         (*(WoReg  *)0x400A5168U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA3CLR         (*(WoReg  *)0x400A516CU) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA4CLR         (*(WoReg  *)0x400A5170U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA5CLR         (*(WoReg  *)0x400A5174U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA6CLR         (*(WoReg  *)0x400A5178U) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA7CLR         (*(WoReg  *)0x400A517CU) /**< \brief (USBC) Endpoint Status Clear Register */
#define REG_USBC_UESTA0SET         (*(WoReg  *)0x400A5190U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA1SET         (*(WoReg  *)0x400A5194U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA2SET         (*(WoReg  *)0x400A5198U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA3SET         (*(WoReg  *)0x400A519CU) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA4SET         (*(WoReg  *)0x400A51A0U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA5SET         (*(WoReg  *)0x400A51A4U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA6SET         (*(WoReg  *)0x400A51A8U) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UESTA7SET         (*(WoReg  *)0x400A51ACU) /**< \brief (USBC) Endpoint Status Set Register */
#define REG_USBC_UECON0            (*(RoReg  *)0x400A51C0U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON1            (*(RoReg  *)0x400A51C4U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON2            (*(RoReg  *)0x400A51C8U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON3            (*(RoReg  *)0x400A51CCU) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON4            (*(RoReg  *)0x400A51D0U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON5            (*(RoReg  *)0x400A51D4U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON6            (*(RoReg  *)0x400A51D8U) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON7            (*(RoReg  *)0x400A51DCU) /**< \brief (USBC) Endpoint Control Register */
#define REG_USBC_UECON0SET         (*(WoReg  *)0x400A51F0U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON1SET         (*(WoReg  *)0x400A51F4U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON2SET         (*(WoReg  *)0x400A51F8U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON3SET         (*(WoReg  *)0x400A51FCU) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON4SET         (*(WoReg  *)0x400A5200U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON5SET         (*(WoReg  *)0x400A5204U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON6SET         (*(WoReg  *)0x400A5208U) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON7SET         (*(WoReg  *)0x400A520CU) /**< \brief (USBC) Endpoint Control Set Register */
#define REG_USBC_UECON0CLR         (*(WoReg  *)0x400A5220U) /**< \brief (USBC) Endpoint Control Clear Register */
#define REG_USBC_UECON1CLR         (*(WoReg  *)0x400A5224U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON2CLR         (*(WoReg  *)0x400A5228U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON3CLR         (*(WoReg  *)0x400A522CU) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON4CLR         (*(WoReg  *)0x400A5230U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON5CLR         (*(WoReg  *)0x400A5234U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON6CLR         (*(WoReg  *)0x400A5238U) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UECON7CLR         (*(WoReg  *)0x400A523CU) /**< \brief (USBC) TXINE Clear */
#define REG_USBC_UHCON             (*(RwReg  *)0x400A5400U) /**< \brief (USBC) Host General Control Register */
#define REG_USBC_UHINT             (*(RoReg  *)0x400A5404U) /**< \brief (USBC) Host Global Interrupt Register */
#define REG_USBC_UHINTCLR          (*(WoReg  *)0x400A5408U) /**< \brief (USBC) Host Global Interrrupt Clear Register */
#define REG_USBC_UHINTSET          (*(WoReg  *)0x400A540CU) /**< \brief (USBC) Host Global Interrupt Set Register */
#define REG_USBC_UHINTE            (*(RoReg  *)0x400A5410U) /**< \brief (USBC) Host Global Interrupt Enable Register */
#define REG_USBC_UHINTECLR         (*(WoReg  *)0x400A5414U) /**< \brief (USBC) Host Global Interrupt Enable Clear Register */
#define REG_USBC_UHINTESET         (*(WoReg  *)0x400A5418U) /**< \brief (USBC) Host Global Interrupt Enable Set Register */
#define REG_USBC_UPRST             (*(RwReg  *)0x400A541CU) /**< \brief (USBC) Pipe Reset Register */
#define REG_USBC_UHFNUM            (*(RwReg  *)0x400A5420U) /**< \brief (USBC) Host Frame Number Register */
#define REG_USBC_UHSOFC            (*(RwReg  *)0x400A5424U) /**< \brief (USBC) Host Start of Frame Control Register */
#define REG_USBC_UPCFG0            (*(RwReg  *)0x400A5500U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG1            (*(RwReg  *)0x400A5504U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG2            (*(RwReg  *)0x400A5508U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG3            (*(RwReg  *)0x400A550CU) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG4            (*(RwReg  *)0x400A5510U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG5            (*(RwReg  *)0x400A5514U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG6            (*(RwReg  *)0x400A5518U) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPCFG7            (*(RwReg  *)0x400A551CU) /**< \brief (USBC) Pipe Configuration Register */
#define REG_USBC_UPSTA0            (*(RoReg  *)0x400A5530U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA1            (*(RoReg  *)0x400A5534U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA2            (*(RoReg  *)0x400A5538U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA3            (*(RoReg  *)0x400A553CU) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA4            (*(RoReg  *)0x400A5540U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA5            (*(RoReg  *)0x400A5544U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA6            (*(RoReg  *)0x400A5548U) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA7            (*(RoReg  *)0x400A554CU) /**< \brief (USBC) Pipe Status Register */
#define REG_USBC_UPSTA0CLR         (*(WoReg  *)0x400A5560U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA1CLR         (*(WoReg  *)0x400A5564U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA2CLR         (*(WoReg  *)0x400A5568U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA3CLR         (*(WoReg  *)0x400A556CU) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA4CLR         (*(WoReg  *)0x400A5570U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA5CLR         (*(WoReg  *)0x400A5574U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA6CLR         (*(WoReg  *)0x400A5578U) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA7CLR         (*(WoReg  *)0x400A557CU) /**< \brief (USBC) Pipe Status Clear Register */
#define REG_USBC_UPSTA0SET         (*(WoReg  *)0x400A5590U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA1SET         (*(WoReg  *)0x400A5594U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA2SET         (*(WoReg  *)0x400A5598U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA3SET         (*(WoReg  *)0x400A559CU) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA4SET         (*(WoReg  *)0x400A55A0U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA5SET         (*(WoReg  *)0x400A55A4U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA6SET         (*(WoReg  *)0x400A55A8U) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPSTA7SET         (*(WoReg  *)0x400A55ACU) /**< \brief (USBC) Pipe Status Set Register */
#define REG_USBC_UPCON0            (*(RoReg  *)0x400A55C0U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON1            (*(RoReg  *)0x400A55C4U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON2            (*(RoReg  *)0x400A55C8U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON3            (*(RoReg  *)0x400A55CCU) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON4            (*(RoReg  *)0x400A55D0U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON5            (*(RoReg  *)0x400A55D4U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON6            (*(RoReg  *)0x400A55D8U) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON7            (*(RoReg  *)0x400A55DCU) /**< \brief (USBC) Pipe Control Register */
#define REG_USBC_UPCON0SET         (*(WoReg  *)0x400A55F0U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON1SET         (*(WoReg  *)0x400A55F4U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON2SET         (*(WoReg  *)0x400A55F8U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON3SET         (*(WoReg  *)0x400A55FCU) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON4SET         (*(WoReg  *)0x400A5600U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON5SET         (*(WoReg  *)0x400A5604U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON6SET         (*(WoReg  *)0x400A5608U) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON7SET         (*(WoReg  *)0x400A560CU) /**< \brief (USBC) Pipe Control Set Register */
#define REG_USBC_UPCON0CLR         (*(WoReg  *)0x400A5620U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON1CLR         (*(WoReg  *)0x400A5624U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON2CLR         (*(WoReg  *)0x400A5628U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON3CLR         (*(WoReg  *)0x400A562CU) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON4CLR         (*(WoReg  *)0x400A5630U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON5CLR         (*(WoReg  *)0x400A5634U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON6CLR         (*(WoReg  *)0x400A5638U) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPCON7CLR         (*(WoReg  *)0x400A563CU) /**< \brief (USBC) Pipe Control Clear Register */
#define REG_USBC_UPINRQ0           (*(RwReg  *)0x400A5650U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ1           (*(RwReg  *)0x400A5654U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ2           (*(RwReg  *)0x400A5658U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ3           (*(RwReg  *)0x400A565CU) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ4           (*(RwReg  *)0x400A5660U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ5           (*(RwReg  *)0x400A5664U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ6           (*(RwReg  *)0x400A5668U) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_UPINRQ7           (*(RwReg  *)0x400A566CU) /**< \brief (USBC) Pipe In Request */
#define REG_USBC_USBCON            (*(RwReg  *)0x400A5800U) /**< \brief (USBC) General Control Register */
#define REG_USBC_USBSTA            (*(RoReg  *)0x400A5804U) /**< \brief (USBC) General Status Register */
#define REG_USBC_USBSTACLR         (*(WoReg  *)0x400A5808U) /**< \brief (USBC) General Status Clear Register */
#define REG_USBC_USBSTASET         (*(WoReg  *)0x400A580CU) /**< \brief (USBC) General Status Set Register */
#define REG_USBC_UVERS             (*(RoReg  *)0x400A5818U) /**< \brief (USBC) IP Version Register */
#define REG_USBC_UFEATURES         (*(RoReg  *)0x400A581CU) /**< \brief (USBC) IP Features Register */
#define REG_USBC_UADDRSIZE         (*(RoReg  *)0x400A5820U) /**< \brief (USBC) IP PB address size Register */
#define REG_USBC_UNAME1            (*(RoReg  *)0x400A5824U) /**< \brief (USBC) IP Name Part One: HUSB */
#define REG_USBC_UNAME2            (*(RoReg  *)0x400A5828U) /**< \brief (USBC) IP Name Part Two: HOST */
#define REG_USBC_USBFSM            (*(RoReg  *)0x400A582CU) /**< \brief (USBC) USB internal finite state machine */
#define REG_USBC_UDESC             (*(RwReg  *)0x400A5830U) /**< \brief (USBC) Endpoint descriptor table */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for USBC peripheral ========== */
#define USBC_EPT_NBR                7
#define USBC_GCLK_NUM               7
#define USBC_HOST_IMPLEMENTED       1
#define USBC_OTG_IMPLEMENTED        
#define USBC_USB_UTMI_IMPLEMENTED   

#endif /* _SAM4L_USBC_INSTANCE_ */
