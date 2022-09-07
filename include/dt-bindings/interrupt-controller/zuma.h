/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * This header provides constants for interrupts on Zuma.
 *
 * Copyright 2021 Google LLC
 *
 */

#ifndef _DT_BINDINGS_INTERRUPT_CONTROLLER_ZUMA_H
#define _DT_BINDINGS_INTERRUPT_CONTROLLER_ZUMA_H

#include <dt-bindings/interrupt-controller/arm-gic.h>

#define IRQ_ALIVE_CUSTOM_EINT0	0
#define IRQ_ALIVE_CUSTOM_EINT1	1
#define IRQ_ALIVE_CUSTOM_EINT2	2
#define IRQ_ALIVE_CUSTOM_EINT3	3
#define IRQ_ALIVE_CUSTOM_EINT4	4
#define IRQ_ALIVE_CUSTOM_EINT5	5
#define IRQ_ALIVE_CUSTOM_EINT6	6
#define IRQ_ALIVE_CUSTOM_EINT7	7
#define IRQ_ALIVE_CUSTOM_EINT8	8
#define IRQ_ALIVE_CUSTOM_EINT9	9
#define IRQ_ALIVE_EINT0	10
#define IRQ_ALIVE_EINT1	11
#define IRQ_ALIVE_EINT2	12
#define IRQ_ALIVE_EINT3	13
#define IRQ_ALIVE_EINT4	14
#define IRQ_ALIVE_EINT5	15
#define IRQ_ALIVE_EINT6	16
#define IRQ_ALIVE_EINT7	17
#define IRQ_ALIVE_EINT8	18
#define IRQ_ALIVE_EINT9	19
#define IRQ_ALIVE_EINT10	20
#define IRQ_ALIVE_EINT11	21
#define IRQ_ALIVE_EINT12	22
#define IRQ_ALIVE_EINT13	23
#define IRQ_ALIVE_EINT14	24
#define IRQ_ALIVE_EINT15	25
#define IRQ_ALIVE_EINT16	26
#define IRQ_ALIVE_EINT17	27
#define IRQ_ALIVE_EINT18	28
#define IRQ_ALIVE_EINT19	29
#define IRQ_ALIVE_EINT20	30
#define IRQ_ALIVE_EINT21	31
#define IRQ_ALIVE_EINT22	32
#define IRQ_ALIVE_EINT23	33
#define IRQ_ALIVE_EINT24	34
#define IRQ_ALIVE_EINT25	35
#define IRQ_ALIVE_EINT26	36
#define IRQ_ALIVE_EINT27	37
#define IRQ_ALIVE_EINT28	38
#define IRQ_ALIVE_EINT29	39
#define IRQ_ALIVE_EINT30	40
#define IRQ_ALIVE_EINT31	41
#define IRQ_ALIVE_EINT32	42
#define IRQ_ALIVE_EINT33	43
#define IRQ_ALIVE_EINT34	44
#define IRQ_ALIVE_EINT35	45
#define IRQ_ALIVE_EINT36	46
#define IRQ_ALIVE_EINT37	47
#define IRQ_ALIVE_EINT38	48
#define IRQ_ALIVE_EINT39	49
#define IRQ_ALIVE_EINT40	50
#define IRQ_ALIVE_EINT41	51
#define IRQ_ALIVE_EINT42	52
#define IRQ_ALIVE_EINT43	53
#define IRQ_ALIVE_EINT44	54
#define IRQ_ALIVE_EINT45	55
#define IRQ_ALIVE_EINT46	56
#define IRQ_ALIVE_EINT47	57
#define IRQ_ALIVE_EINT48	58
#define IRQ_ALIVE_EINT49	59
#define IRQ_ALIVE_EINT50	60
#define IRQ_ALIVE_EINT51	61
#define IRQ_ALIVE_EINT52	62
#define IRQ_ALIVE_EINT53	63
#define IRQ_ALIVE_EINT54	64
#define IRQ_ALIVE_EINT55	65
#define IRQ_ALIVE_EINT56	66
#define IRQ_ALIVE_EINT57	67
#define IRQ_APM_I3C_PMIC_ALIVE	68
#define IRQ_APM_USI0_UART_ALIVE	69
#define IRQ_APM_USI0_USI_ALIVE	70
#define IRQ_APM_USI1_UART_INT_ALIVE	71
#define IRQ_COMB_SFI_CE_NONSECURE_SYSREG_APM_ALIVE	72
#define IRQ_COMB_SFI_UCE_NONSECURE_SYSREG_APM_ALIVE	73
#define IRQ_INTCOMB_VGPIO2AP_ALIVE	74
#define IRQ_IRQB_M_ALIVE	75
#define IRQ_IRQB_S_ALIVE	76
#define IRQ_MAILBOX_AOCA322AP_ALIVE	77
#define IRQ_MAILBOX_AOCF12AP_ALIVE	78
#define IRQ_MAILBOX_AOCP62AP_ALIVE	79
#define IRQ_MAILBOX_APM2AP_ALIVE	80
#define IRQ_MAILBOX_AURCORE02AP_ALIVE	81
#define IRQ_MAILBOX_AURCORE12AP_ALIVE	82
#define IRQ_MAILBOX_AURCORE22AP_ALIVE	83
#define IRQ_MAILBOX_AURMCUNS02AP_ALIVE	84
#define IRQ_MAILBOX_AURMCUNS12AP_ALIVE	85
#define IRQ_MAILBOX_AURMCUNS22AP_ALIVE	86
#define IRQ_MAILBOX_AURMCUNS32AP_ALIVE	87
#define IRQ_MAILBOX_AURMCUNS42AP_ALIVE	88
#define IRQ_MAILBOX_AURMCUTZ2AP_ALIVE	89
#define IRQ_MAILBOX_DBGCORE2AP_ALIVE	90
#define IRQ_OCP_WARN_CPUCL1_ALIVE	91
#define IRQ_OCP_WARN_CPUCL2_ALIVE	92
#define IRQ_OCP_WARN_GPU_ALIVE	93
#define IRQ_OCP_WARN_TPU_ALIVE	94
#define IRQ_ONOB_ALIVE	95
#define IRQ_RTC_ALARM_INT_ALIVE	96
#define IRQ_RTC_TIC_INT_0_ALIVE	97
#define IRQ_SECURE_ALIVE_JTAG_INT_ALIVE	98
#define IRQ_SMPL_WARN_ALIVE	99
#define IRQ_SOFT_OCP_WARN_CPUCL1_ALIVE	100
#define IRQ_SOFT_OCP_WARN_CPUCL2_ALIVE	101
#define IRQ_SOFT_OCP_WARN_GPU_ALIVE	102
#define IRQ_SOFT_OCP_WARN_TPU_ALIVE	103
#define IRQ_TRTC_ALARM_INT_ALIVE	104
#define IRQ_TRTC_TIC_INT_0_ALIVE	105
#define IRQ_VDROOP1_ALIVE	106
#define IRQ_VDROOP2_ALIVE	107
#define IRQ_WDT_APM_ALIVE	108
#define IRQ_WDT_DBGCORE_ALIVE	109
#define NONSEQINT__UASC_APM_ALIVE	110
#define NONSEQINT__UASC_APM_DMA_ALIVE	111
#define NONSEQINT__UASC_DBGCORE_ALIVE	112
#define NONSEQINT__UASC_IG_SWD_ALIVE	113
#define NONSEQINT__UASC_LP_AOC_ALIVE_ALIVE	114
#define NONSEQINT__UASC_P_ALIVE_ALIVE	115
#define NONSEQINT__UASC_PMU_ALIVE	116
#define O_INTERRUPT_STAGE2_NONSEC__SYSMMU_S0_ALIVE_ALIVE	117
#define O_INTERRUPT_STAGE2_SECURE__SYSMMU_S0_ALIVE_ALIVE	118
#define TZINT__UASC_APM_ALIVE	119
#define TZINT__UASC_APM_DMA_ALIVE	120
#define TZINT__UASC_DBGCORE_ALIVE	121
#define TZINT__UASC_IG_SWD_ALIVE	122
#define TZINT__UASC_LP_AOC_ALIVE_ALIVE	123
#define TZINT__UASC_P_ALIVE_ALIVE	124
#define TZINT__UASC_PMU_ALIVE	125
#define INTR_AOC_PPMU_AOC	126
#define INTR_AOCPCIE_PPMU_AOC	127
#define INTR_AOCUSB_PPMU_AOC	128
#define IRQ_AOC_TIMER0_AOC	129
#define IRQ_AOC_Watchdog_CPU_AOC	130
#define INTREQ_AOCPLL_LOCK_STATUS_AOC	131
#define IRQ_SYSMMU_STAGE1_NS_AOC	132
#define IRQ_SYSMMU_STAGE1_S_AOC	133
#define IRQ_SYSMMU_STAGE2_NS_AOC	134
#define IRQ_SYSMMU_STAGE2_S_AOC	135
#define IRQ_UASC_NS_AOC_AOC	136
#define IRQ_UASC_S_AOC_AOC	137
#define INTREG__ADD_APBIF_AUR_AUR	138
#define IRQ_AUR_IDMA_IRQ_0_AUR	139
#define IRQ_AUR_IDMA_IRQ_1_AUR	140
#define IRQ_AUR_IDMA_IRQ_2_AUR	141
#define IRQ_AUR_IDMA_IRQ_3_AUR	142
#define IRQ_AUR_IDMA_IRQ_4_AUR	143
#define IRQ_AUR_IDMA_IRQ_5_AUR	144
#define IRQ_AUR_IDMA_IRQ_6_AUR	145
#define IRQ_AUR_IDMA_IRQ_7_AUR	146
#define IRQ_AUR_IDMA_IRQ_8_AUR	147
#define IRQ_AUR_IDMA_IRQ_9_AUR	148
#define IRQ_AUR_IDMA_IRQ_10_AUR	149
#define IRQ_AUR_IDMA_IRQ_11_AUR	150
#define IRQ_AUR_PG_REQ_AUR	151
#define IRQ_AUR_WDOG_AP_IRQ_AUR	152
#define IRQ_AUR_WDOG_APM_IRQ_AUR	153
#define IRQ_PPMU_AUR0_UPPER_OR_NORMAL_AUR	154
#define IRQ_PPMU_AUR1_UPPER_OR_NORMAL_AUR	155
#define IRQ_SYSMMU_S1_NS_AUR_AUR	156
#define IRQ_SYSMMU_S1_S_AUR_AUR	157
#define IRQ_SYSMMU_S2_NS_AUR_AUR	158
#define IRQ_SYSMMU_S2_S_AUR_AUR	159
#define IRQ_UASC_NS_P0_AUR_AUR	160
#define IRQ_UASC_NS_P1_AUR_AUR	161
#define IRQ_UASC_S_P0_AUR_AUR	162
#define IRQ_UASC_S_P1_AUR_AUR	163
#define IRQ_BW_XINT_BW	164
#define IRQ_PPMU_UPPER_OR_NORMAL_BW	165
#define IRQ_SYSMMU_S1_NS_BW_BW	166
#define IRQ_SYSMMU_S1_S_BW_BW	167
#define IRQ_SYSMMU_S2_NS_BW_BW	168
#define IRQ_SYSMMU_S2_S_BW_BW	169
#define IRQ_UASC_NS_BW_BW	170
#define IRQ_UASC_S_BW_BW	171
#define IRQ_CPUCL0_ADD0_CTRL_CPUCL0	172
#define IRQ_CPUCL0_ADD1_CTRL_CPUCL0	173
#define IRQ_CPUCL0_ADD2_CTRL_CPUCL0	174
#define IRQ_CPUCL0_CLUSTERCRITIRQ_CPUCL0	175
#define IRQ_CPUCL0_CLUSTERERRIRQ_CPUCL0	176
#define IRQ_CPUCL0_CLUSTERFAULTIRQ_CPUCL0	177
#define IRQ_CPUCL0_CLUSTERPMUIRQ_CPUCL0	178
#define IRQ_CPUCL0_COMPLEXERRIRQ_CPUCL0	179
#define IRQ_CPUCL0_COMPLEXERRIRQ_0_CPUCL0	180
#define IRQ_CPUCL0_COMPLEXFAULTIRQ_CPUCL0	181
#define IRQ_CPUCL0_COMPLEXFAULTIRQ_0_CPUCL0	182
#define IRQ_CPUCL0_ERRIRQ_0_CPUCL0	183
#define IRQ_CPUCL0_ERRIRQ_1_CPUCL0	184
#define IRQ_CPUCL0_ERRIRQ_2_CPUCL0	185
#define IRQ_CPUCL0_ERRIRQ_3_CPUCL0	186
#define IRQ_CPUCL0_ERRIRQ_4_CPUCL0	187
#define IRQ_CPUCL0_ERRIRQ_5_CPUCL0	188
#define IRQ_CPUCL0_ERRIRQ_6_CPUCL0	189
#define IRQ_CPUCL0_CTIIRQ_9		190
#define IRQ_CPUCL0_ERRIRQ_7_CPUCL0	191
#define IRQ_CPUCL0_ERRIRQ_8_CPUCL0	192
#define IRQ_CPUCL0_FAULTIRQ_0_CPUCL0	193
#define IRQ_CPUCL0_FAULTIRQ_1_CPUCL0	194
#define IRQ_CPUCL0_FAULTIRQ_2_CPUCL0	195
#define IRQ_CPUCL0_FAULTIRQ_3_CPUCL0	196
#define IRQ_CPUCL0_FAULTIRQ_4_CPUCL0	197
#define IRQ_CPUCL0_FAULTIRQ_5_CPUCL0	198
#define IRQ_CPUCL0_FAULTIRQ_6_CPUCL0	199
#define IRQ_CPUCL0_FAULTIRQ_7_CPUCL0	200
#define IRQ_CPUCL0_FAULTIRQ_8_CPUCL0	201
#define IRQ_CPUCL0_DONE_ADD0_CTRL_CPUCL0	202
#define IRQ_CPUCL0_DONE_ADD1_CTRL_CPUCL0	203
#define IRQ_CPUCL0_DONE_ADD2_CTRL_CPUCL0	204
#define IRQ_CPUCL0_DROOP_ADD0_CTRL_CPUCL0	205
#define IRQ_CPUCL0_DROOP_ADD1_CTRL_CPUCL0	206
#define IRQ_CPUCL0_DROOP_ADD2_CTRL_CPUCL0	207
#define IRQ_CPUCL0_FAIL_ADD0_CTRL_CPUCL0	208
#define IRQ_CPUCL0_FAIL_ADD1_CTRL_CPUCL0	209
#define IRQ_CPUCL0_FAIL_ADD2_CTRL_CPUCL0	210
#define IRQ_CPUCL0_INTREQERRNS_NID68_CPUCL0	211
#define IRQ_CPUCL0_INTREQERRS_NID68_CPUCL0	212
#define IRQ_CPUCL0_INTREQFAULTNS_NID68_CPUCL0	213
#define IRQ_CPUCL0_INTREQFAULTS_NID68_CPUCL0	214
#define IRQ_CPUCL0_INTREQMPAMERRNS_NID68_CPUCL0	215
#define IRQ_CPUCL0_INTREQMPAMERRS_NID68_CPUCL0	216
#define IRQ_CPUCL0_INTREQPMU_NID68_CPUCL0	217
#define IRQ_CPUCL0_INTREQPPU_NID68_CPUCL0	218
#define IRQ_CPUCL0_MPAMNSIRQ_CPUCL0	219
#define IRQ_CPUCL0_MPAMSIRQ_CPUCL0	220
#define IRQ_CPUCL0_PPC_INSTRRET_CLUSTER0_0_CPUCL0	221
#define INTREQ__SECURE_LOG				224
#define IRQ_CPUCL0_PPC_INSTRRET_CLUSTER0_1_CPUCL0	230
#define IRQ_CPUCL0_PPC_INSTRRUN_CLUSTER0_0_CPUCL0	231
#define IRQ_CPUCL0_PPC_INSTRRUN_CLUSTER0_1_CPUCL0	232
#define IRQ_CPUCL0_PPMU_CPUCL0_D0_UPPER_NORMAL_CPUCL0	233
#define IRQ_CPUCL0_PPMU_CPUCL0_D1_UPPER_NORMAL_CPUCL0	234
#define IRQ_CPUCL0_PPMU_CPUCL0_D2_UPPER_NORMAL_CPUCL0	235
#define IRQ_CPUCL0_PPMU_CPUCL0_D3_UPPER_NORMAL_CPUCL0	236
#define INTREQ_S2MPU_S0_CPUCL0_NS_CPUCL0	237
#define INTREQ_S2MPU_S0_CPUCL0_S_CPUCL0	238
#define IRQ_DPUB_DECON0_DQE_DIMMING_END_DPUB	239
#define IRQ_DPUB_DECON0_DQE_DIMMING_START_DPUB	240
#define IRQ_DPUB_DECON0_EXTRA_DPUB	241
#define IRQ_DPUB_DECON0_FRAME_DONE_DPUB	242
#define IRQ_DPUB_DECON0_FRAME_START_DPUB	243
#define IRQ_DPUB_DECON1_DQE_DIMMING_END_DPUB	244
#define IRQ_DPUB_DECON1_DQE_DIMMING_START_DPUB	245
#define IRQ_DPUB_DECON1_EXTRA_DPUB	246
#define IRQ_DPUB_DECON1_FRAME_DONE_DPUB	247
#define IRQ_DPUB_DECON1_FRAME_START_DPUB	248
#define IRQ_DPUB_DECON2_EXTRA_DPUB	249
#define IRQ_DPUB_DECON2_FRAME_DONE_DPUB	250
#define IRQ_DPUB_DECON2_FRAME_START_DPUB	251
#define IRQ_DPUB_DSIM0_DPUB	252
#define IRQ_DPUB_DSIM1_DPUB	253
#define IRQ_DPUF0_DMA_BLUR0_DPUF0	254
#define IRQ_DPUF0_DMA_CGCTRL0_DPUF0	255
#define IRQ_DPUF0_DMA_L0_DPUF0	256
#define IRQ_DPUF0_DMA_L1_DPUF0	257
#define IRQ_DPUF0_DMA_L2_DPUF0	258
#define IRQ_DPUF0_DMA_L3_DPUF0	259
#define IRQ_DPUF0_DMA_L4_DPUF0	260
#define IRQ_DPUF0_DMA_L5_DPUF0	261
#define IRQ_DPUF0_DMA_L6_DPUF0	262
#define IRQ_DPUF0_DMA_RCDISP0_DPUF0	263
#define IRQ_DPUF0_DMA_WB0_DPUF0	264
#define IRQ_DPUF0_DPP_L0_DPUF0	265
#define IRQ_DPUF0_DPP_L1_DPUF0	266
#define IRQ_DPUF0_DPP_L2_DPUF0	267
#define IRQ_DPUF0_DPP_L3_DPUF0	268
#define IRQ_DPUF0_DPP_L4_DPUF0	269
#define IRQ_DPUF0_DPP_L5_DPUF0	270
#define IRQ_DPUF0_DPP_L6_DPUF0	271
#define IRQ_DPUF0_SRAMCON_D0_DPUF0	272
#define IRQ_DPUF0_SRAMCON_D1_DPUF0	273
#define IRQ_DPUF0_SRAMCON_D2_DPUF0	274
#define IRQ_PPMU_D0_DPUF0_UPPER_OR_NORMAL_DPUF0	275
#define IRQ_PPMU_D1_DPUF0_UPPER_OR_NORMAL_DPUF0	276
#define IRQ_SYSMMU_S0_DPUF0_S1_NS_DPUF0	277
#define IRQ_SYSMMU_S0_DPUF0_S1_S_DPUF0	278
#define IRQ_SYSMMU_S0_DPUF0_S2_NS_DPUF0	279
#define IRQ_SYSMMU_S0_DPUF0_S2_S_DPUF0	280
#define IRQ_DPUF1_DMA_BLUR0_DPUF1	281
#define IRQ_DPUF1_DMA_CGCTRL0_DPUF1	282
#define IRQ_DPUF1_DMA_L0_DPUF1	283
#define IRQ_DPUF1_DMA_L1_DPUF1	284
#define IRQ_DPUF1_DMA_L2_DPUF1	285
#define IRQ_DPUF1_DMA_L3_DPUF1	286
#define IRQ_DPUF1_DMA_L4_DPUF1	287
#define IRQ_DPUF1_DMA_L5_DPUF1	288
#define IRQ_DPUF1_DMA_L6_DPUF1	289
#define IRQ_DPUF1_DMA_RCDISP0_DPUF1	290
#define IRQ_DPUF1_DMA_WB0_DPUF1	291
#define IRQ_DPUF1_DPP_L0_DPUF1	292
#define IRQ_DPUF1_DPP_L1_DPUF1	293
#define IRQ_DPUF1_DPP_L2_DPUF1	294
#define IRQ_DPUF1_DPP_L3_DPUF1	295
#define IRQ_DPUF1_DPP_L4_DPUF1	296
#define IRQ_DPUF1_DPP_L5_DPUF1	297
#define IRQ_DPUF1_DPP_L6_DPUF1	298
#define IRQ_DPUF1_SRAMCON_D0_DPUF1	299
#define IRQ_DPUF1_SRAMCON_D1_DPUF1	300
#define IRQ_DPUF1_SRAMCON_D2_DPUF1	301
#define IRQ_PPMU_D0_DPUF1_UPPER_OR_NORMAL_DPUF1	302
#define IRQ_PPMU_D1_DPUF1_UPPER_OR_NORMAL_DPUF1	303
#define IRQ_SYSMMU_S0_DPUF1_S1_NS_DPUF1	304
#define IRQ_SYSMMU_S0_DPUF1_S1_S_DPUF1	305
#define IRQ_SYSMMU_S0_DPUF1_S2_NS_DPUF1	306
#define IRQ_SYSMMU_S0_DPUF1_S2_S_DPUF1	307
#define IRQ_EH_0_EH	308
#define IRQ_EH_1_EH	309
#define IRQ_EH_2_EH	310
#define IRQ_EH_3_EH	311
#define IRQ_EH_4_EH	312
#define IRQ_EH_5_EH	313
#define IRQ_EH_6_EH	314
#define IRQ_EH_7_EH	315
#define IRQ_EH_8_EH	316
#define IRQ_EH_9_EH	317
#define IRQ_EH_10_EH	318
#define IRQ_EH_11_EH	319
#define IRQ_EH_12_EH	320
#define IRQ_EH_13_EH	321
#define IRQ_EH_14_EH	322
#define IRQ_EH_15_EH	323
#define IRQ_EH_16_EH	324
#define IRQ_EH_17_EH	325
#define IRQ_EH_18_EH	326
#define IRQ_EH_19_EH	327
#define IRQ_PPC_EH_CYCLE_O_interrupt_upper_or_normal_EH	328
#define IRQ_PPC_EH_EVENT_O_interrupt_upper_or_normal_EH	329
#define IRQ_PPMU_UPPER_OR_NORMAL_EH	330
#define IRQ_SYSMMU_S1_NS_EH_EH	331
#define IRQ_SYSMMU_S1_S_EH_EH	332
#define IRQ_SYSMMU_S2_NS_EH_EH	333
#define IRQ_SYSMMU_S2_S_EH_EH	334
#define IRQ_UASC_NS_EH_EH	335
#define IRQ_UASC_S_EH_EH	336
#define IRQ_G2D_G2D	337
#define IRQ_JPEG_G2D	338
#define IRQ_PPMU_D0_G2D_interrupt_upper_or_normal_G2D	339
#define IRQ_PPMU_D1_G2D_interrupt_upper_or_normal_G2D	340
#define IRQ_PPMU_D2_G2D_interrupt_upper_or_normal_G2D	341
#define IRQ_SYSMMU_S0_G2D_STAGE1_NONSEC_G2D	342
#define IRQ_SYSMMU_S0_G2D_STAGE1_SECURE_G2D	343
#define IRQ_SYSMMU_S0_G2D_STAGE2_NONSEC_G2D	344
#define IRQ_SYSMMU_S0_G2D_STAGE2_SECURE_G2D	345
#define IRQ_G3D_IRQGPU_G3D	346
#define IRQ_G3D_IRQJOB_G3D	347
#define IRQ_G3D_IRQMMU_G3D	348
#define IRQ_PPCFW_G3Dx_G3D_G3D	349
#define IRQ_PPMU_D0_G3D_G3D	350
#define IRQ_PPMU_D1_G3D_G3D	351
#define IRQ_PPMU_D2_G3D_G3D	352
#define IRQ_PPMU_D3_G3D_G3D	353
#define IRQ_SYSMMU_STAGE2_NONSEC_G3D_G3D	354
#define IRQ_SYSMMU_STAGE2_SECURE_G3D_G3D	355
#define IRQ_UASC_NS_G3D_G3D	356
#define IRQ_UASC_S_G3D_G3D	357
#define O_ADD_APBIF_G3D_FLAG_IRQ_PEND_G3D	358
#define IRQ_GDC0_IRQ_0_GDC	359
#define IRQ_GDC0_IRQ_1_GDC	360
#define IRQ_GDC1_IRQ_0_GDC	361
#define IRQ_GDC1_IRQ_1_GDC	362
#define IRQ_LME_IntReq_GDC	363
#define IRQ_PPMU_D0_GDC0_UPPER_OR_NORMAL_GDC	364
#define IRQ_PPMU_D0_GDC1_UPPER_OR_NORMAL_GDC	365
#define IRQ_PPMU_D2_GDC0_UPPER_OR_NORMAL_GDC	366
#define IRQ_PPMU_D2_GDC1_UPPER_OR_NORMAL_GDC	367
#define IRQ_PPMU_D4_GDC0_UPPER_OR_NORMAL_GDC	368
#define IRQ_PPMU_D4_GDC1_UPPER_OR_NORMAL_GDC	369
#define IRQ_PPMU_D_LME_UPPER_OR_NORMAL_GDC	370
#define IRQ_SYSMMU_S0_GDC_STAGE1_NONSEC_GDC	371
#define IRQ_SYSMMU_S0_GDC_STAGE1_SECURE_GDC	372
#define IRQ_SYSMMU_S0_GDC_STAGE2_NONSEC_GDC	373
#define IRQ_SYSMMU_S0_GDC_STAGE2_SECURE_GDC	374
#define IRQ_MAILBOX_GSA2NONTZ_GSA	375
#define IRQ_MAILBOX_GSA2TZ_GSA	376
#define IRQ_SYSMMU_S0_GSA_S1_NS_GSA	377
#define IRQ_SYSMMU_S0_GSA_S1_S_GSA	378
#define IRQ_SYSMMU_S0_GSA_S2_NS_GSA	379
#define IRQ_SYSMMU_S0_GSA_S2_S_GSA	380
#define IRQ_GSE_NonSecu_IRQ_GSE	381
#define IRQ_PPMU_D0_GSE_UPPER_OR_NORMAL_GSE	382
#define IRQ_PPMU_D1_GSE_UPPER_OR_NORMAL_GSE	383
#define IRQ_PPMU_D2_GSE_UPPER_OR_NORMAL_GSE	384
#define IRQ_SYSMMU_S0_GSE_S1_NS_GSE	385
#define IRQ_SYSMMU_S0_GSE_S1_S_GSE	386
#define IRQ_SYSMMU_S0_GSE_S2_NS_GSE	387
#define IRQ_SYSMMU_S0_GSE_S2_S_GSE	388
#define IRQ_DP_LINK_HSI0	389
#define IRQ_I3C2_HSI0_HSI0	390
#define IRQ_I3C3_HSI0_HSI0	391
#define IRQ_PPMU_HSI0_UPPER_OR_NORMAL_HSI0	392
#define IRQ_SYSMMU_USB_S1_NS_HSI0	393
#define IRQ_SYSMMU_USB_S1_S_HSI0	394
#define IRQ_SYSMMU_USB_S2_NS_HSI0	395
#define IRQ_SYSMMU_USB_S2_S_HSI0	396
#define IRQ_USB2_REMOTE_CONNECT_GIC_HSI0	397
#define IRQ_USB2_REMOTE_TIMER_GIC_HSI0	398
#define IRQ_USB2_REMOTE_WAKEUP_GIC_HSI0	399
#define IRQ_USB32DRD_ABOX_0_HSI0	400
#define IRQ_USB32DRD_ABOX_1_HSI0	401
#define IRQ_USB32DRD_GIC_0_HSI0	402
#define IRQ_USB32DRD_GIC_1_HSI0	403
#define IRQ_USB32DRD_REWA_WAKEUP_REQ_HSI0	404
#define IRQ_USBDPPHY_TCA_HSI0	405
#define IRQ_USI0_HSI0_HSI0	406
#define IRQ_USI1_HSI0_HSI0	407
#define IRQ_USI2_HSI0_HSI0	408
#define IRQ_USI3_HSI0_HSI0	409
#define IRQ_USI4_HSI0_HSI0	410
#define NONSEQINT__UASC_HSI0_LINK_HSI0	411
#define TZINT__UASC_HSI0_LINK_HSI0	412
#define IRQ_GPIO_HSI1_HSI1	413
#define IRQ_PCIE_GEN3_2L_HSI1	414
#define IRQ_PCIE_GEN3_2L_DOORBELL_HSI1	415
#define IRQ_PCIE_GEN3_2L_MSI_0_HSI1	416
#define IRQ_PCIE_GEN3_2L_MSI_1_HSI1	417
#define IRQ_PCIE_GEN3_2L_MSI_2_HSI1	418
#define IRQ_PCIE_GEN3_2L_MSI_3_HSI1	419
#define IRQ_PCIE_GEN3_2L_MSI_4_HSI1	420
#define IRQ_PCIE_GEN3_2L_PCS_HSI1	421
#define IRQ_PCIE_IA_GEN3A_0_HSI1	422
#define IRQ_PPMU_HSI1_UPPER_OR_NORMAL_HSI1	423
#define IRQ_SYSMMU_STAGE1_NS_HSI1	424
#define IRQ_SYSMMU_STAGE1_S_HSI1	425
#define IRQ_SYSMMU_STAGE2_NS_HSI1	426
#define IRQ_SYSMMU_STAGE2_S_HSI1	427
#define IRQ_UASC_NS_PCIE_GEN3A_DBI_0_HSI1	428
#define IRQ_UASC_NS_PCIE_GEN3A_SLV_0_HSI1	429
#define IRQ_UASC_S_PCIE_GEN3A_DBI_0_HSI1	430
#define IRQ_UASC_S_PCIE_GEN3A_SLV_0_HSI1	431
#define IRQ_GPIO_HSI2_HSI2	432
#define IRQ_GPIO_HSI2UFS_HSI2	433
#define IRQ_MMC_CARD_HSI2	434
#define IRQ_NONSEQINT_PCIE_GEN3A_DBI_1_HSI2	435
#define IRQ_NONSEQINT_PCIE_GEN3A_SLV_1_HSI2	436
#define IRQ_NONSEQINT_PCIE_GEN3B_DBI_1_HSI2	437
#define IRQ_NONSEQINT_PCIE_GEN3B_SLV_1_HSI2	438
#define IRQ_PCIE_GEN3A_1_HSI2	439
#define IRQ_PCIE_GEN3A_1_DOORBELL_HSI2	440
#define IRQ_PCIE_GEN3A_1_MSI_0_HSI2	441
#define IRQ_PCIE_GEN3A_1_MSI_1_HSI2	442
#define IRQ_PCIE_GEN3A_1_MSI_2_HSI2	443
#define IRQ_PCIE_GEN3A_1_MSI_3_HSI2	444
#define IRQ_PCIE_GEN3A_1_MSI_4_HSI2	445
#define IRQ_PCIE_GEN3A_1_PCS_HSI2	446
#define IRQ_PCIE_GEN3B_1_HSI2	447
#define IRQ_PCIE_GEN3B_1_DOORBELL_HSI2	448
#define IRQ_PCIE_GEN3B_1_MSI_0_HSI2	449
#define IRQ_PCIE_GEN3B_1_MSI_1_HSI2	450
#define IRQ_PCIE_GEN3B_1_MSI_2_HSI2	451
#define IRQ_PCIE_GEN3B_1_MSI_3_HSI2	452
#define IRQ_PCIE_GEN3B_1_MSI_4_HSI2	453
#define IRQ_PCIE_GEN3B_1_PCS_HSI2	454
#define IRQ_PCIE_IA_GEN3A_1_HSI2	455
#define IRQ_PCIE_IA_GEN3B_1_HSI2	456
#define IRQ_PPMU_HSI2_UPPER_OR_NORMAL_HSI2	457
#define IRQ_SYSMMU_STAGE1_NS_HSI2	458
#define IRQ_SYSMMU_STAGE1_S_HSI2	459
#define IRQ_SYSMMU_STAGE2_NS_HSI2	460
#define IRQ_SYSMMU_STAGE2_S_HSI2	461
#define IRQ_TZINT_PCIE_GEN3A_DBI_1_HSI2	462
#define IRQ_TZINT_PCIE_GEN3A_SLV_1_HSI2	463
#define IRQ_TZINT_PCIE_GEN3B_DBI_1_HSI2	464
#define IRQ_TZINT_PCIE_GEN3B_SLV_1_HSI2	465
#define IRQ_UFS_EMBD_HSI2	466
#define IRQ_CSIS0_ISPFE	467
#define IRQ_CSIS1_ISPFE	468
#define IRQ_CSIS2_ISPFE	469
#define IRQ_CSIS3_ISPFE	470
#define IRQ_CSIS4_ISPFE	471
#define IRQ_CSIS5_ISPFE	472
#define IRQ_CSIS6_ISPFE	473
#define IRQ_CSIS7_ISPFE	474
#define IRQ_CSIS8_ISPFE	475
#define IRQ_CSIS9_ISPFE	476
#define IRQ_CSIS10_ISPFE	477
#define IRQ_CSIS11_ISPFE	478
#define IRQ_ISPFE_0_ISPFE	479
#define IRQ_ISPFE_1_ISPFE	480
#define IRQ_ISPFE_2_ISPFE	481
#define IRQ_ISPFE_3_ISPFE	482
#define IRQ_ISPFE_4_ISPFE	483
#define IRQ_PPMU_D0_ISPFE_UPPER_OR_NORMAL_ISPFE	484
#define IRQ_PPMU_D1_ISPFE_UPPER_OR_NORMAL_ISPFE	485
#define IRQ_PPMU_D2_ISPFE_UPPER_OR_NORMAL_ISPFE	486
#define IRQ_PPMU_D3_ISPFE_UPPER_OR_NORMAL_ISPFE	487
#define IRQ_SYSMMU_S0_ISPFE_S1_NONSECURE_ISPFE	488
#define IRQ_SYSMMU_S0_ISPFE_S1_SECURE_ISPFE	489
#define IRQ_SYSMMU_S0_ISPFE_S2_NONSECURE_ISPFE	490
#define IRQ_SYSMMU_S0_ISPFE_S2_SECURE_ISPFE	491
#define IRQ_SYSMMU_S1_ISPFE_S1_NONSECURE_ISPFE	492
#define IRQ_SYSMMU_S1_ISPFE_S1_SECURE_ISPFE	493
#define IRQ_SYSMMU_S1_ISPFE_S2_NONSECURE_ISPFE	494
#define IRQ_SYSMMU_S1_ISPFE_S2_SECURE_ISPFE	495
#define IRQ_SYSMMU_S2_ISPFE_S1_NONSECURE_ISPFE	496
#define IRQ_SYSMMU_S2_ISPFE_S1_SECURE_ISPFE	497
#define IRQ_SYSMMU_S2_ISPFE_S2_NONSECURE_ISPFE	498
#define IRQ_SYSMMU_S2_ISPFE_S2_SECURE_ISPFE	499
#define IRQ_UASC_ISPFE_NONSEQ_ISPFE	500
#define IRQ_UASC_ISPFE_TZ_ISPFE	501
#define IRQ_MCSC_0_MCSC	502
#define IRQ_MCSC_1_MCSC	503
#define IRQ_PPMU_D0_MCSC_UPPER_OR_NORMAL_MCSC	504
#define IRQ_PPMU_D1_MCSC_UPPER_OR_NORMAL_MCSC	505
#define IRQ_PPMU_D2_MCSC_UPPER_OR_NORMAL_MCSC	506
#define IRQ_PPMU_D3_MCSC_UPPER_OR_NORMAL_MCSC	507
#define IRQ_PPMU_D4_MCSC_UPPER_OR_NORMAL_MCSC	508
#define IRQ_PPMU_D5_MCSC_UPPER_OR_NORMAL_MCSC	509
#define IRQ_PPMU_D6_MCSC_UPPER_OR_NORMAL_MCSC	510
#define IRQ_SYSMMU_S0_MCSC_S1_NONSECURE_MCSC	511
#define IRQ_SYSMMU_S0_MCSC_S1_SECURE_MCSC	512
#define IRQ_SYSMMU_S0_MCSC_S2_NONSECURE_MCSC	513
#define IRQ_SYSMMU_S0_MCSC_S2_SECURE_MCSC	514
#define IRQ_MFC_MFC	515
#define IRQ_PPMU_D0_MFC_interrupt_upper_or_normal_MFC	516
#define IRQ_PPMU_D1_MFC_interrupt_upper_or_normal_MFC	517
#define IRQ_SYSMMU_S0_MFC_STAGE1_NONSEC_MFC	518
#define IRQ_SYSMMU_S0_MFC_STAGE1_SECURE_MFC	519
#define IRQ_SYSMMU_S0_MFC_STAGE2_NONSEC_MFC	520
#define IRQ_SYSMMU_S0_MFC_STAGE2_SECURE_MFC	521
#define IRQ_PPC_MIF_MIF0	522
#define IRQ_SMC_APBACCESSINT_MIF0	523
#define IRQ_SMC_ECC_CORERR_MIF0	524
#define IRQ_SMC_ECC_UNCORERR_MIF0	525
#define IRQ_SMC_PPMPINT_MIF0	526
#define IRQ_SMC_TEMPERR_MIF0	527
#define IRQ_SMC_TEMPHOT_MIF0	528
#define IRQ_SMC_TZCINT_MIF0	529
#define IRQ_PPC_MIF_MIF1	530
#define IRQ_SMC_APBACCESSINT_MIF1	531
#define IRQ_SMC_ECC_CORERR_MIF1	532
#define IRQ_SMC_ECC_UNCORERR_MIF1	533
#define IRQ_SMC_PPMPINT_MIF1	534
#define IRQ_SMC_TEMPERR_MIF1	535
#define IRQ_SMC_TEMPHOT_MIF1	536
#define IRQ_SMC_TZCINT_MIF1	537
#define IRQ_PPC_MIF_MIF2	538
#define IRQ_SMC_APBACCESSINT_MIF2	539
#define IRQ_SMC_ECC_CORERR_MIF2	540
#define IRQ_SMC_ECC_UNCORERR_MIF2	541
#define IRQ_SMC_PPMPINT_MIF2	542
#define IRQ_SMC_TEMPERR_MIF2	543
#define IRQ_SMC_TEMPHOT_MIF2	544
#define IRQ_SMC_TZCINT_MIF2	545
#define IRQ_PPC_MIF_MIF3	546
#define IRQ_SMC_APBACCESSINT_MIF3	547
#define IRQ_SMC_ECC_CORERR_MIF3	548
#define IRQ_SMC_ECC_UNCORERR_MIF3	549
#define IRQ_SMC_PPMPINT_MIF3	550
#define IRQ_SMC_TEMPERR_MIF3	551
#define IRQ_SMC_TEMPHOT_MIF3	552
#define IRQ_SMC_TZCINT_MIF3	553
#define IRQ_LD_SLC_CH0_O_APC_NS_IRQ_NOCL0	554
#define IRQ_LD_SLC_CH0_O_APC_S_IRQ_NOCL0	555
#define IRQ_LD_SLC_CH0_O_PPMPU_IRQ_NOCL0	556
#define IRQ_LD_SLC_CH0_O_UASC_NS_IRQ_NOCL0	557
#define IRQ_LD_SLC_CH0_O_UASC_TZ_IRQ_NOCL0	558
#define IRQ_LD_SLC_CH1_O_APC_NS_IRQ_NOCL0	559
#define IRQ_LD_SLC_CH1_O_APC_S_IRQ_NOCL0	560
#define IRQ_LD_SLC_CH1_O_PPMPU_IRQ_NOCL0	561
#define IRQ_LD_SLC_CH1_O_UASC_NS_IRQ_NOCL0	562
#define IRQ_LD_SLC_CH1_O_UASC_TZ_IRQ_NOCL0	563
#define IRQ_LD_SLC_CH2_O_APC_NS_IRQ_NOCL0	564
#define IRQ_LD_SLC_CH2_O_APC_S_IRQ_NOCL0	565
#define IRQ_LD_SLC_CH2_O_PPMPU_IRQ_NOCL0	566
#define IRQ_LD_SLC_CH2_O_UASC_NS_IRQ_NOCL0	567
#define IRQ_LD_SLC_CH2_O_UASC_TZ_IRQ_NOCL0	568
#define IRQ_LD_SLC_CH3_O_APC_NS_IRQ_NOCL0	569
#define IRQ_LD_SLC_CH3_O_APC_S_IRQ_NOCL0	570
#define IRQ_LD_SLC_CH3_O_PPMPU_IRQ_NOCL0	571
#define IRQ_LD_SLC_CH3_O_UASC_NS_IRQ_NOCL0	572
#define IRQ_LD_SLC_CH3_O_UASC_TZ_IRQ_NOCL0	573
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_CPUCL0_EVENT_NOCL0	574
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_IO_EVENT_NOCL0	575
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_NOCL1A_EVENT_NOCL0	576
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_CPUCL0_EVENT_NOCL0	577
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_IO_EVENT_NOCL0	578
#define IRQ_NOCL0_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_NOCL1A_EVENT_NOCL0	579
#define IRQ_PPC_CPUCL0_D0_CYCLE_O_interrupt_upper_or_normal_NOCL0	580
#define IRQ_PPC_DEBUG_O_interrupt_lower_NOCL0	581
#define IRQ_PPC_DEBUG_O_interrupt_upper_or_normal_NOCL0	582
#define IRQ_PPC_NOCL0_IO0_CYCLE_O_interrupt_upper_or_normal_NOCL0	583
#define IRQ_PPC_NOCL1A_M0_CYCLE_O_interrupt_upper_or_normal_NOCL0	584
#define IRQ_PPC_NOCL1B_M0_CYCLE_O_interrupt_upper_or_normal_NOCL0	585
#define IRQ_PPC_NOCL1B_M0_EVENT_O_interrupt_upper_or_normal_NOCL0	586
#define IRQ_PPMU_NOCL0_ALIVE_P_O_interrupt_upper_or_normal_NOCL0	587
#define IRQ_PPMU_NOCL0_CPUCL0_P_O_interrupt_upper_or_normal_NOCL0	588
#define IRQ_PPMU_NOCL0_DP_O_interrupt_upper_or_normal_NOCL0	589
#define IRQ_PPMU_NOCL0_IOC0_O_interrupt_upper_or_normal_NOCL0	590
#define IRQ_PPMU_NOCL0_IOC1_O_interrupt_upper_or_normal_NOCL0	591
#define IRQ_PPMU_NOCL0_S0_O_interrupt_upper_or_normal_NOCL0	592
#define IRQ_PPMU_NOCL0_S1_O_interrupt_upper_or_normal_NOCL0	593
#define IRQ_PPMU_NOCL0_S2_O_interrupt_upper_or_normal_NOCL0	594
#define IRQ_PPMU_NOCL0_S3_O_interrupt_upper_or_normal_NOCL0	595
#define IRQ_TREX_D_NOCL0_debugInterrupt_NOCL0	596
#define IRQ_TREX_P_NOCL0_debugInterrupt_NOCL0	597
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_AUR_EVENT_NOCL1A	598
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_G3D_EVENT_NOCL1A	599
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_NOCL2AA_EVENT_NOCL1A	600
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_NOCL2AB_EVENT_NOCL1A	601
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_AND_PPC_TPU_EVENT_NOCL1A	602
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_AUR_EVENT_NOCL1A	603
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_G3D_EVENT_NOCL1A	604
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_NOCL2AA_EVENT_NOCL1A	605
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_NOCL2AB_EVENT_NOCL1A	606
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_OR_PPC_TPU_EVENT_NOCL1A	607
#define IRQ_NOCL1A_PPC_CON_O_interrupt_upper_or_normal_PPC_BW_EVENT_NOCL1A	608
#define IRQ_PPC_AUR_D0_CYCLE_O_interrupt_upper_or_normal_NOCL1A	609
#define IRQ_PPC_BW_D_CYCLE_O_interrupt_upper_or_normal_NOCL1A	610
#define IRQ_PPC_G3D_D0_CYCLE_O_interrupt_upper_or_normal_NOCL1A	611
#define IRQ_PPC_NOCL2AA_S0_CYCLE_O_interrupt_upper_or_normal_NOCL1A	612
#define IRQ_PPC_NOCL2AB_S0_CYCLE_O_interrupt_upper_or_normal_NOCL1A	613
#define IRQ_PPC_TPU_D0_CYCLE_O_interrupt_upper_or_normal_NOCL1A	614
#define IRQ_PPMU_NOCL1A_M0_O_interrupt_upper_or_normal_NOCL1A	615
#define IRQ_PPMU_NOCL1A_M1_O_interrupt_upper_or_normal_NOCL1A	616
#define IRQ_PPMU_NOCL1A_M2_O_interrupt_upper_or_normal_NOCL1A	617
#define IRQ_PPMU_NOCL1A_M3_O_interrupt_upper_or_normal_NOCL1A	618
#define IRQ_TREX_D_NOCL1A_debugInterrupt_NOCL1A	619
#define IRQ_TREX_P_NOCL1A_debugInterrupt_NOCL1A	620
#define IRQ_PPC_AOC_CYCLE_O_interrupt_upper_or_normal_NOCL1B	621
#define IRQ_PPC_AOC_EVENT_O_interrupt_upper_or_normal_NOCL1B	622
#define IRQ_PPMU_NOCL1B_M0_O_interrupt_upper_or_normal_NOCL1B	623
#define IRQ_TREX_D_NOCL1B_debugInterrupt_NOCL1B	624
#define IRQ_TREX_P_NOCL1B_debugInterrupt_NOCL1B	625
#define IRQ_PPMU_NOCL2AA_M0_O_interrupt_upper_or_normal_NOCL2AA	626
#define IRQ_PPMU_NOCL2AA_M1_O_interrupt_upper_or_normal_NOCL2AA	627
#define IRQ_TREX_D_NOCL2AA_debugInterrupt_NOCL2AA	628
#define IRQ_TREX_P_NOCL2AA_debugInterrupt_NOCL2AA	629
#define IRQ_PPMU_NOCL2AB_M0_O_interrupt_upper_or_normal_NOCL2AB	630
#define IRQ_PPMU_NOCL2AB_M1_O_interrupt_upper_or_normal_NOCL2AB	631
#define IRQ_TREX_D_NOCL2AB_debugInterrupt_NOCL2AB	632
#define IRQ_TREX_P_NOCL2AB_debugInterrupt_NOCL2AB	633
#define IRQ_GPIO_PERIC0_PERIC0	634
#define IRQ_I3C1_PERIC0	635
#define IRQ_I3C2_PERIC0	636
#define IRQ_I3C3_PERIC0	637
#define IRQ_I3C4_PERIC0	638
#define IRQ_I3C5_PERIC0	639
#define IRQ_I3C6_PERIC0	640
#define IRQ_USI0_UART_PERIC0	641
#define IRQ_USI1_USI_PERIC0	642
#define IRQ_USI2_USI_PERIC0	643
#define IRQ_USI3_USI_PERIC0	644
#define IRQ_USI4_USI_PERIC0	645
#define IRQ_USI5_USI_PERIC0	646
#define IRQ_USI6_USI_PERIC0	647
#define IRQ_USI14_USI_PERIC0	648
#define IRQ_GPIO_PERIC1_PERIC1	649
#define IRQ_I3C0_PERIC1	650
#define IRQ_PWM0_PERIC1	651
#define IRQ_PWM1_PERIC1	652
#define IRQ_PWM2_PERIC1	653
#define IRQ_PWM3_PERIC1	654
#define IRQ_PWM4_PERIC1	655
#define IRQ_USI0_USI_PERIC1	656
#define IRQ_USI9_USI_PERIC1	657
#define IRQ_USI10_USI_PERIC1	658
#define IRQ_USI11_USI_PERIC1	659
#define IRQ_USI12_USI_PERIC1	660
#define IRQ_USI13_USI_PERIC1	661
#define IRQ_USI15_USI_PERIC1	662
#define IRQ_MCFP_INTREQ_0_RGBP	663
#define IRQ_MCFP_INTREQ_1_RGBP	664
#define IRQ_PPMU_D0_MCFP_UPPER_OR_NORMAL_RGBP	665
#define IRQ_PPMU_D0_RGBP_UPPER_OR_NORMAL_RGBP	666
#define IRQ_PPMU_D1_RGBP_UPPER_OR_NORMAL_RGBP	667
#define IRQ_PPMU_D2_MCFP_UPPER_OR_NORMAL_RGBP	668
#define IRQ_PPMU_D2_RGBP_UPPER_OR_NORMAL_RGBP	669
#define IRQ_PPMU_D3_MCFP_UPPER_OR_NORMAL_RGBP	670
#define IRQ_PPMU_D4_MCFP_UPPER_OR_NORMAL_RGBP	671
#define IRQ_PPMU_D5_MCFP_UPPER_OR_NORMAL_RGBP	672
#define IRQ_RGBP_INTREQ_0_RGBP	673
#define IRQ_RGBP_INTREQ_1_RGBP	674
#define IRQ_SYSMMU_S0_RGBP_STAGE1_NONSEC_RGBP	675
#define IRQ_SYSMMU_S0_RGBP_STAGE1_SECURE_RGBP	676
#define IRQ_SYSMMU_S0_RGBP_STAGE2_NONSEC_RGBP	677
#define IRQ_SYSMMU_S0_RGBP_STAGE2_SECURE_RGBP	678
#define IRQ_SYSMMU_S1_RGBP_STAGE1_NONSEC_RGBP	679
#define IRQ_SYSMMU_S1_RGBP_STAGE1_SECURE_RGBP	680
#define IRQ_SYSMMU_S1_RGBP_STAGE2_NONSEC_RGBP	681
#define IRQ_SYSMMU_S1_RGBP_STAGE2_SECURE_RGBP	682
#define IRQ_SYSMMU_S0_TNR_S1_NONSEC_TNR	683
#define IRQ_SYSMMU_S0_TNR_S1_SECURE_TNR	684
#define IRQ_SYSMMU_S0_TNR_S2_NONSEC_TNR	685
#define IRQ_SYSMMU_S0_TNR_S2_SECURE_TNR	686
#define IRQ_SYSMMU_S1_TNR_S1_NONSEC_TNR	687
#define IRQ_SYSMMU_S1_TNR_S1_SECURE_TNR	688
#define IRQ_SYSMMU_S1_TNR_S2_NONSEC_TNR	689
#define IRQ_SYSMMU_S1_TNR_S2_SECURE_TNR	690
#define IRQ_SYSMMU_S2_TNR_S1_NONSEC_TNR	691
#define IRQ_SYSMMU_S2_TNR_S1_SECURE_TNR	692
#define IRQ_SYSMMU_S2_TNR_S2_NONSEC_TNR	693
#define IRQ_SYSMMU_S2_TNR_S2_SECURE_TNR	694
#define IRQ_TNR_0_TNR	695
#define IRQ_TNR_1_TNR	696
#define IRQ_TNR_A_TNR	697
#define IRQ_TNR_PPMU_D0_TNR	698
#define IRQ_TNR_PPMU_D1_TNR	699
#define IRQ_TNR_PPMU_D2_TNR	700
#define IRQ_TNR_PPMU_D3_TNR	701
#define IRQ_TNR_PPMU_D4_TNR	702
#define IRQ_TNR_PPMU_D5_TNR	703
#define IRQ_TNR_PPMU_D6_TNR	704
#define IRQ_TNR_PPMU_D7_TNR	705
#define IRQ_TNR_PPMU_D8_TNR	706
#define IRQ_TNR_PPMU_D9_TNR	707
#define IRQ_TNR_PPMU_D10_TNR	708
#define IRQ_TNR_PPMU_D11_TNR	709
#define INTREG__ADD_APBIF_TPU_TPU	710
#define IRQ_MAILBOX_TPU2AP_NS_TPU_0_TPU	711
#define IRQ_MAILBOX_TPU2AP_NS_TPU_1_TPU	712
#define IRQ_MAILBOX_TPU2AP_NS_TPU_2_TPU	713
#define IRQ_MAILBOX_TPU2AP_NS_TPU_3_TPU	714
#define IRQ_MAILBOX_TPU2AP_NS_TPU_4_TPU	715
#define IRQ_MAILBOX_TPU2AP_NS_TPU_5_TPU	716
#define IRQ_MAILBOX_TPU2AP_NS_TPU_6_TPU	717
#define IRQ_MAILBOX_TPU2AP_NS_TPU_7_TPU	718
#define IRQ_MAILBOX_TPU2AP_NS_TPU_8_TPU	719
#define IRQ_MAILBOX_TPU2AP_NS_TPU_9_TPU	720
#define IRQ_MAILBOX_TPU2AP_NS_TPU_10_TPU	721
#define IRQ_MAILBOX_TPU2AP_NS_TPU_11_TPU	722
#define IRQ_MAILBOX_TPU2AP_NS_TPU_12_TPU	723
#define IRQ_MAILBOX_TPU2AP_NS_TPU_13_TPU	724
#define IRQ_MAILBOX_TPU2AP_NS_TPU_14_TPU	725
#define IRQ_MAILBOX_TPU2AP_NS_TPU_15_TPU	726
#define IRQ_MAILBOX_TPU2AP_NS_TPU_EL_TPU	727
#define IRQ_MAILBOX_TPU2AP_S_TPU_0_TPU	728
#define IRQ_MAILBOX_TPU2AP_S_TPU_1_TPU	729
#define IRQ_MAILBOX_TPU2AP_S_TPU_EL_TPU	730
#define IRQ_PPMU_TPU0_UPPER_OR_NORMAL_TPU	731
#define IRQ_PPMU_TPU1_UPPER_OR_NORMAL_TPU	732
#define IRQ_SYSMMU_S1_NS_TPU_TPU	733
#define IRQ_SYSMMU_S1_S_TPU_TPU	734
#define IRQ_SYSMMU_S2_NS_TPU_TPU	735
#define IRQ_SYSMMU_S2_S_TPU_TPU	736
#define IRQ_PPMU_D0_YUVP_UPPER_OR_NORMAL_YUVP	737
#define IRQ_PPMU_D1_YUVP_UPPER_OR_NORMAL_YUVP	738
#define IRQ_PPMU_D4_YUVP_UPPER_OR_NORMAL_YUVP	739
#define IRQ_SYSMMU_S0_YUVP_S1_NONSECURE_YUVP	740
#define IRQ_SYSMMU_S0_YUVP_S1_SECURE_YUVP	741
#define IRQ_SYSMMU_S0_YUVP_S2_NONSECURE_YUVP	742
#define IRQ_SYSMMU_S0_YUVP_S2_SECURE_YUVP	743
#define IRQ_YUVP_0_YUVP	744
#define IRQ_YUVP_1_YUVP	745
#define IRQ_OTP_CON_TOP_MISC	753
#define IRQ_MCT_G0_MISC	754
#define IRQ_MCT_G1_MISC	755
#define IRQ_MCT_G2_MISC	756
#define IRQ_MCT_G3_MISC	757
#define IRQ_MCT_L0_MISC	758
#define IRQ_MCT_L1_MISC	759
#define IRQ_MCT_L2_MISC	760
#define IRQ_MCT_L3_MISC	761
#define IRQ_MCT_L4_MISC	762
#define IRQ_MCT_L5_MISC	763
#define IRQ_MCT_L6_MISC	764
#define IRQ_MCT_L7_MISC	765
#define IRQ_MCT_L8_MISC	766
#define IRQ_WDT_CLUSTER0_MISC	767
#define IRQ_WDT_CLUSTER1_MISC	768
#define IRQ_OTP_CON_BISR_MISC	769
#define IRQ_OTP_CON_BIRA_MISC	770
#define IRQ_TMU_TOP_INTPROBE_15_MISC	771
#define IRQ_TMU_TOP_INTPROBE_14_MISC	772
#define IRQ_TMU_TOP_INTPROBE_13_MISC	773
#define IRQ_TMU_TOP_INTPROBE_12_MISC	774
#define IRQ_TMU_TOP_INTPROBE_11_MISC	775
#define IRQ_TMU_TOP_INTPROBE_10_MISC	776
#define IRQ_TMU_TOP_INTPROBE_9_MISC	777
#define IRQ_TMU_TOP_INTPROBE_8_MISC	778
#define IRQ_TMU_TOP_INTPROBE_7_MISC	779
#define IRQ_TMU_TOP_INTPROBE_6_MISC	780
#define IRQ_TMU_TOP_INTPROBE_5_MISC	781
#define IRQ_TMU_TOP_INTPROBE_4_MISC	782
#define IRQ_TMU_TOP_INTPROBE_3_MISC	783
#define IRQ_TMU_TOP_INTPROBE_2_MISC	784
#define IRQ_TMU_TOP_INTPROBE_1_MISC	785
#define IRQ_TMU_TOP_INTPROBE_0_MISC	786
#define IRQ_TMU_TOP_INTREQ_MISC	787
#define IRQ_TMU_SUB_INTPROBE_15_MISC	788
#define IRQ_TMU_SUB_INTPROBE_14_MISC	789
#define IRQ_TMU_SUB_INTPROBE_13_MISC	790
#define IRQ_TMU_SUB_INTPROBE_12_MISC	791
#define IRQ_TMU_SUB_INTPROBE_11_MISC	792
#define IRQ_TMU_SUB_INTPROBE_10_MISC	793
#define IRQ_TMU_SUB_INTPROBE_9_MISC	794
#define IRQ_TMU_SUB_INTPROBE_8_MISC	795
#define IRQ_TMU_SUB_INTPROBE_7_MISC	796
#define IRQ_TMU_SUB_INTPROBE_6_MISC	797
#define IRQ_TMU_SUB_INTPROBE_5_MISC	798
#define IRQ_TMU_SUB_INTPROBE_4_MISC	799
#define IRQ_TMU_SUB_INTPROBE_3_MISC	800
#define IRQ_TMU_SUB_INTPROBE_2_MISC	801
#define IRQ_TMU_SUB_INTPROBE_1_MISC	802
#define IRQ_TMU_SUB_INTPROBE_0_MISC	803
#define IRQ_TMU_SUB_INTREQ_MISC	804
#define IRQ_SPDMA1_MISC	805
#define IRQ_PDMA1_MISC	806
#define IRQ_SPDMA0_MISC	807
#define IRQ_PDMA0_MISC	808
#define IRQ_SC_KMERRINT_MISC	809
#define IRQ_SC_SCERRINT_MISC	810
#define IRQ_SC_KMINT_MISC	811
#define IRQ_SC_SCDMAERRINT_MISC	812
#define IRQ_SC_SCDMAINT_MISC	813
#define IRQ_SC_SCINT_MISC	814
#define IRQ_PUF_MERGED_INT_MISC	815
#define IRQ_RTIC_MISC	816
#define IRQ_DIT_TxDst2_MISC	817
#define IRQ_DIT_TxDst1_MISC	818
#define IRQ_DIT_TxDst0_MISC	819
#define IRQ_DIT_TxSrc2_MISC	820
#define IRQ_DIT_TxSrc1_MISC	821
#define IRQ_DIT_TxSrc0_MISC	822
#define IRQ_DIT_RxDst2_MISC	823
#define IRQ_DIT_RxDst1_MISC	824
#define IRQ_DIT_RxDst03_MISC	825
#define IRQ_DIT_RxDst02_MISC	826
#define IRQ_DIT_RxDst01_MISC	827
#define IRQ_DIT_RxDst00_MISC	828
#define IRQ_DIT_RxSrc2_MISC	829
#define IRQ_DIT_RxSrc1_MISC	830
#define IRQ_DIT_RxSrc0_MISC	831
#define IRQ_PPMU_MISC_UPPER_OR_NORMAL_MISC	832
#define IRQ_SYSMMU_S0_MISC_S2_NONSEC_MISC	833
#define IRQ_SYSMMU_S0_MISC_S2_SECURE_MISC	834
#define IRQ_SYSMMU_S0_MISC_S1_NONSEC_MISC	835
#define IRQ_SYSMMU_S0_MISC_S1_SECURE_MISC	836
#define IRQ_GIC_FAULT_MISC	837
#define IRQ_GIC_ERR_MISC	838
#define IRQ_GIC_PMU_MISC	839

#endif  /* _DT_BINDINGS_INTERRUPT_CONTROLLER_ZUMA_H */
