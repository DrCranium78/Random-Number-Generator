;
;  rng.asm  
; 
;  Version:     1.0.7
;  Last Update: 29.10.2022
;  Author:      Frank Bjørnø
; 
;  Purpose: 
;       A pseudo random number generator
; 
;  License:
;  
;           Copyright (C) 2022 Frank Bjørnø
;
;          1. Permission is hereby granted, free of charge, to any person obtaining a copy 
;          of this software and associated documentation files (the "Software"), to deal 
;          in the Software without restriction, including without limitation the rights 
;          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
;          of the Software, and to permit persons to whom the Software is furnished to do 
;          so, subject to the following conditions:
;        
;          2. The above copyright notice and this permission notice shall be included in all 
;          copies or substantial portions of the Software.
;
;          3. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
;          INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
;          PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
;          HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
;          CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
;          OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;


.586												;  asm instruction rdtsc requires 586 instruction set
.model flat, C


;
;  Constants used in the linear congruential function Xn+1 = (aXn + c) mod m
;
__a			equ	047068445h						;  (.01m < a < .99m, a % 8 = 5)
__c            equ  01016b5h                                ;  3*7*23*37*59 (must have no factors in common with m, and should be odd)

;
;  Make the following functions visible to the linker
;
public randomize, rndmax, set_seed, rnd, rndflt, rndint, rndbin


;
;  This is the data segment where variables are declared and initialized
;
.data
__m			dd	07fffffffh						;  This is a prime number (2^31 - 1). Can't be immediate value in proc rndflt.
                                                            ;  Also notice that A MOD 2^n = A AND (2^n - 1).
__seed		dd	013b3eh							;  Initial seed.


;
;  This is the code segment
;
.code



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  randomize                                                                                                   ;
;              Set a random seed. This is done by getting the system time and using the 31 least significant bits.         ;
;  Input:      void                                                                                                        ;
;  Return:     32 bit integer in EAX                                                                                       ;
;  Registers:  EAX, EDX                                                                                                    ;
;  C function: unsigned int randomize(void);                                                                               ;
;--------------------------------------------------------------------------------------------------------------------------;
randomize			proc
		rdtsc									;  Read Time-Stamp Counter into EDX:EAX. High order 32 bits are loaded into EDX, Low order into EAX
		and		eax, __m							;  mask the most significant bit
		mov		__seed, eax						;  set the seed to a relatively random number, (lsb is somewhat random)		
		ret
randomize			endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  rndmax                                                                                                      ;
;              Get the maximum number that can be generated by the rng.                                                    ;
;  Input:      void                                                                                                        ;
;  Return:     32 bit integer in EAX                                                                                       ;
;  Registers:  EAX                                                                                                         ;
;  C function: unsigned int rndmax(void);                                                                                  ;
;--------------------------------------------------------------------------------------------------------------------------;
rndmax			proc
		mov		EAX, __m
		ret
rndmax			endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  set_seed                                                                                                    ;
;              Sets the seed to a value (32 bit integer) that is passed on the stack.                                      ;
;  Input:      32 bit integer in ESP + 4                                                                                   ;
;  Return:     void (input argument is in EAX when returning)                                                              ;
;  Registers:  EAX                                                                                                         ;
;  C function: void set_seed(unsigned int);                                                                                ;
;                                                                                                                          ;
;  Note:       This may set a seed that the generator can't produce. The highest number produced by this rng is limited    ;
;              by __m = 2^31 - 1 = 2 147 483 647. The input argument may, however, be up to 2^32. This is only a           ;
;              problem if you plan to measure the length of the sequence of numbers produced by the generate procedure     ;
;              and use the set_seed procedure to set the initial seed.                                                     ;
;--------------------------------------------------------------------------------------------------------------------------;
set_seed			proc
		mov		eax, dword ptr[esp + 4]				;  get the seed passed on the stack
		mov		__seed, eax						;  set the seed
		ret
set_seed			endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  rnd                                                                                                         ;
;              Returns raw random numbers generated by the __generate procedure. Note that this function does not return   ;
;              the seed, but a number in which some of the bits have been the shifted around. This is because              ;
;              the least significant bit of the seed is not random, meaning that the seed alternates between even and odd  ;
;  Input:      void                                                                                                        ;
;  Return:     32 bit integer in EAX                                                                                       ;
;  Registers:  EAX                                                                                                         ;
;  C function: unsigned int rnd(void);
;--------------------------------------------------------------------------------------------------------------------------;
rnd       		proc
		call      __generate                        
		ret
rnd                 endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  rndfl                                                                                                       ;
;              Get a random number in the interval [0.0, 1.0]                                                              ;
;  Input:      void                                                                                                        ;
;  Return:     Double precision number in ST(0)                                                                            ;
;  Registers:  EAX                                                                                                         ;
;  C function: double rndflt(void);                                                                                        ;
;--------------------------------------------------------------------------------------------------------------------------;
rndflt			proc
		call 	__generate						;  generate a random number on interval [0, __m]
		push	eax									;  push on stack
		fild	dword ptr[esp]							;  load integer from stack on st(0)
		fidiv	__m								;  divide seed by the value of m to get a number in the interval [0, 1]
		add		esp, 4							;  "pop" the stack, that is, add 4 bytes to esp
		ret										;  return control to caller
rndflt			endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  rndint                                                                                                      ;   
;              Produces a random integer in the interval [A, B]                                                            ;
;  Input:      A: 32 bit int, B: 32 bit int. A < B. A > -__m, and B < __m                                                  ;
;  Return:     Int  in EAX                                                                                                 ;
;  Registers:  EAX, EDX                                                                                                    ;
;  C function: int rndint(int A, int B);                                                                                   ;
;                                                                                                                          ;
;  Note:       No error checking of any kind will be performed. If input conditions are not met, behaviour is undefined.   ;
;--------------------------------------------------------------------------------------------------------------------------;
rndint       proc
	call    __generate								;  call __generate. __seed now in EAX
	push    ebx									;  save EBX on stack

;  calculate interval width in ebx	
	mov     ebx, dword ptr[esp + 12]					;  load high limit into EBX
	sub     ebx, dword ptr[esp + 8]					;  subtract low limit from EBX
	inc     ebx									;  add 1
	
;  scale number
	xor     edx, edx
	div     ebx									;  divide __seed by interval width. remainder in EDX
	add     edx, dword ptr[esp + 8]					;  add low limit to remainder	
	
	mov     eax, edx								;  copy remainder into EAX
	pop     ebx									;  load EBX
	ret											;  return control to caller
rndint       endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  rndbin                                                                                                      ;   
;              Produces a 0 or 1 at random.                                                                                ;
;  Input:      void                                                                                                        ;
;  Return:     Integer (0 or 1) in EAX                                                                                     ;
;  Registers:  EAX, EDX                                                                                                    ;
;  C function: unsigned int rndbin(void);                                                                                  ;
;                                                                                                                          ;
;  Note:       Both the Wald-Wolfowitz runs test and a chi square goodness of fit test show that the __generate            ;
;              procedure produces (pseudo) random strings of numbers above and below the mean. Whether a number            ;
;              is above or below the mean is determined by bit 31. Therefore, we can produce a binary value that           ;
;              acts fairly random by isolating and returning this bit.                                                     ;
;--------------------------------------------------------------------------------------------------------------------------;
rndbin       proc	 
	call    __generate								;  generate a random number in interval [0, __m]	
	and       eax, 1								;  isolate lsb	
	ret											;  return control to caller	
rndbin       endp



;--------------------------------------------------------------------------------------------------------------------------;
;  Procedure:  generate                                                                                                    ;
;              This procedure generates a pseudo random seed and is the basis for generating random numbers.               ;
;  Input:      void                                                                                                        ;
;  Return:     32 bit integer in EAX                                                                                       ;
;  Registers:  EAX, EDX                                                                                                    ;
;  C function: unsigned int __generate(void);                                                                              ;
;--------------------------------------------------------------------------------------------------------------------------;
__generate		proc
  ; prepare registers
		push	ebx							;  save EBX on stack in accordance with the C calling convention.
		xor	edx, edx						;  reset EDX so it won't interfere with multiplication
		mov	eax, __a						;  copy a into EAX
		mov	ebx, __seed					;  copy seed, Xn, into EBX

  ; calculate
		mul	   ebx						;  this is the multiplication aXn
		mov	   edx, __c					;  copy c into EDX
		add	   eax, edx					;  this is the addition aXn + c
		and	   eax, __m					;  this is the modulus operation (aXn + c) mod m
		
  ; save seed
		mov	__seed, eax					;  set seed Xn+1 = (aXn + c) mod m										
  
  ; hash
		mov	   ecx, 4						;  shift 4 bits
		xor	   edx, edx					;  reset edx
@@rotate:
		rcl	   eax, 1						;  shift lsb into msb and rotate msb into edx
		rcr	    dl, 1
		loop	        @@rotate		
		shr	    dl, 4
		or	    al, dl					;  copy msb into ax
		shr	   eax, 1						;  shift one bit
		
  ; clean up and return		
		pop		ebx						;  restore EBX
		ret
__generate		endp

end