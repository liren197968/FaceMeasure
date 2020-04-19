;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
;* File Name          : startup_cm4.s
;* @author  MCD Application Team
;* Description        : STM32H7xx devices vector table for CortexM4 core for MDK-ARM toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Reset_Handler consists in an infinite loop so to boot CM4 core properly and keep it in a known state.
;*           
;******************************************************************************
;* @attention
;*
;* Copyright (c) 2019 STMicroelectronics.
;* All rights reserved.
;*
;* This software component is licensed by ST under BSD 3-Clause license,
;* the "License"; You may not use this file except in compliance with the
;* License. You may obtain a copy of the License at:
;*                        opensource.org/licenses/BSD-3-Clause
;*
;******************************************************************************


                PRESERVE8
                THUMB

; Vector Table for CM4 Mapped to Address 0x08100000 at Reset
                AREA    RESET_M4, DATA, READONLY
                EXPORT  __Vectors_m4
                EXPORT  __Vectors_m4_End
                EXPORT  __Vectors_m4_Size

                IMPORT  __initial_sp
					
			    KEEP  __Vectors_m4
					
__Vectors_m4    DCD     __initial_sp                   ; Top of Stack
                DCD     Reset_Handler_m4               ; Reset Handler

__Vectors_m4_End

__Vectors_m4_Size  EQU  __Vectors_m4_End - __Vectors_m4

                AREA    |.text|, CODE, READONLY

; Reset handler fro CM4 core: infinite loop
Reset_Handler_m4 PROC
                 EXPORT  Reset_Handler_m4                    [WEAK]
 	 
				 B       .

                 ENDP

                 ALIGN		

			     KEEP Reset_Handler_m4
               

                 END

;************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****
