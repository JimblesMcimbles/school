TITLE Designing low-level I/O procedures (assignment6.asm)

; Assignment 6 (Option A)
; Description: Assignment 6A for CS271-400; designing low-level I/O procedures.
;	1. Implement and test your own readVal and writeVal procedures for
;		unsigned integers.
;	2. Implement macros getString and displayString. The macros may use
;		Irvine's ReadString to get input from the user, and WriteString
;		to display output.
;		a. getString should display a prompt, then get the user's
;			keyboard input into a memory location.
;		b. displayString should display the string stored in a specific
;			memory location.
;		c. readVal should invoke the getString macro to get the user's
;			string of digits. It should then convert the digit
;			string to numeric, while validating the user's input.
;		d. writeVal should convert a numeric value to a string of
;			digits, and invoke the displayString macro to produce
;			the output.
;	3. Write a small test program that gets 10 valid integers from the user
;		and stores the numeric values in an array. The program then
;		displays the integers, their sum, and their average (can be
;		rounded down).

INCLUDE Irvine32.inc

;------------------------------------------------------------------------------
; getString
; Displays a prompt to the user, then gets the user's keyboard input into a
;	memory location.
; Receives: prompt to display (reference), string buffer (reference), length of
;	string buffer (value)
; Returns: none
; Preconditions: prompt string and string buffer are passed as offset, and
;	length of string buffer is passed as value to the macro in the order
;	specified in the receives field
; Postconditions: the supplied prompt is displayed to the user, and the user's
;	keyboard input is stored as a string at the address of string buffer.
; Registers changed: edx, ecx
;------------------------------------------------------------------------------
getString	MACRO	promptAddr, buffer, buffer_size
	push	edx
	push	ecx
	mov	edx, promptAddr		; display the supplied prompt
	call	WriteString
	mov	edx, buffer
	mov	ecx, buffer_size
	call	ReadString
	pop	ecx
	pop	edx
ENDM

;------------------------------------------------------------------------------
; displayString
; Displays the string stored in a specific memory location.
; Receives: string to display (address)
; Returns: none
; Preconditions: string to display must be passed as offset
; Postconditions: the supplied string is displayed to the user
; Registers changed: edx
;------------------------------------------------------------------------------
displayString	MACRO	strAddress
	push	edx
	mov	edx, strAddress
	call	WriteString
	pop	edx
ENDM

.data
programTitle	BYTE	"PROGRAMMING ASSIGNMENT 6: Designing low-level "
		BYTE	"I/O procedures", 13, 10
		BYTE	"Written by: Sharon Kuo", 0
instructions	BYTE	"Please provide 10 unsigned decimal integers.", 13, 10
		BYTE	"Each number needs to be small enough to fit inside a "
		BYTE	"32 bit register.", 13, 10
		BYTE	"After you have finished inputting the raw numbers I "
		BYTE	"will display a list", 13, 10
		BYTE	"of the integers, their sum, and their average "
		BYTE	"value.", 0
numPrompt	BYTE	"Please enter an unsigned integer: ", 0
errorMsg	BYTE	"ERROR: You did not enter an unsigned number or your "
		BYTE	"number was too big.", 0
tryAgain	BYTE	"Please try again: ", 0
enteredMsg	BYTE	"You entered the following numbers:", 0
commaSpace	BYTE	", ", 0
sumMsg		BYTE	"The sum of these numbers is: ", 0
avgMsg		BYTE	"The average is: ", 0
thanksMsg	BYTE	"Thanks for playing!", 0

array		DWORD	10 DUP(?)

.code
main	PROC
	push	OFFSET programTitle
	push	OFFSET instructions
	call	introduction

	; getData controls the input loop
	push	OFFSET array
	push	LENGTHOF array
	push	OFFSET numPrompt
	push	OFFSET tryAgain
	push	OFFSET errorMsg
	call	getData

	push	OFFSET array
	push	LENGTHOF array
	push	OFFSET enteredMsg
	push	OFFSET commaSpace
	call	displayList

	push	OFFSET array
	push	LENGTHOF array
	push	OFFSET sumMsg
	push	OFFSET avgMsg
	call	displaySumAvg

	push	OFFSET thanksMsg
	call	displayEnd

	exit
main	ENDP

;------------------------------------------------------------------------------
; introduction
; Displays the program introduction to the user.
; Receives: programTitle (reference), instructions (reference)
; Returns: none
; Preconditions: all parameters must be passed as offset, and are pushed
;	to the stack in the order specified in the Receives field
; Postconditions: prints program name, programmer name, and instructions to
;	console
; Registers changed: ebp (saved by stack frame setup), edx
;------------------------------------------------------------------------------
introduction	PROC	USES	edx
	; set up stack frame
		push	ebp
		mov	ebp, esp
	; display program title: programTitle = [ebp + 16]
		mov	edx, [ebp + 16]
		displayString	edx
		call	Crlf
		call	Crlf
	; display program instructions: instructions = [ebp + 12]
		mov	edx, [ebp + 12]
		displayString	edx
		call	Crlf
		call	Crlf

	pop	ebp
	ret	8
introduction	ENDP

;------------------------------------------------------------------------------
; getData
; Gets user input from the keyboard to fill an array. Calls on readVal to
;	read input, which in turn calls on validate to validate that input is
;	digits, which calls on convertNum to convert the number to an integer
;	and verify that it fits in a 32-bit register.
; Receives: array (reference), LENGTHOF array (value), numPrompt (reference),
;	tryAgain (reference), errorMsg (reference)
; Returns: none
; Preconditions: array, numPrompt, tryAgain, and errorMsg are passed as offset
;	and LENGTHOF array is passed as value, and are pushed to the stack in
;	the order specified in the Receives field
; Postconditions: array is filled with validated unsigned integers
; Registers changed: ebp (saved by stack frame setup), esi, ecx, eax
;------------------------------------------------------------------------------
getData		PROC	USES	esi ecx eax
	; set up stack frame
		push	ebp
		mov	ebp, esp
	; fill array: array = [ebp + 36]
		mov	esi, [ebp + 36]
		mov	ecx, [ebp + 32]

	fillArray:
		mov	eax, [ebp + 28]		; numPrompt = [ebp + 28]
		push	eax
		push	[ebp + 24]		; tryAgain = [ebp + 24]
		push	[ebp + 20]		; errorMsg = [ebp + 20]
		call	readVal
		pop	[esi]		; store converted number in array
		add	esi, 4
		loop	fillArray

	pop	ebp
	ret	20
getData			ENDP

;------------------------------------------------------------------------------
; readVal
; Reads user input from the keyboard, calls on validate to verify that input is
;	made of digits, which calls on convertNum to convert the string input
;	to an integer and verifies that the input fits in a 32-bit register.
;	Returns a validated unsigned integer on the system stack for getVal.
;	Repeats user input if entered string is invalid (based on valid, which
;	is affected by validate and tooLarge from convertNum).
; Receives: numPrompt (reference), tryAgain (reference), errorMsg (reference)
; Returns: validated user input unsigned integer on top of the stack (as value)
; Preconditions: all parameters must be passed as offset, and are pushed to the
;	stack in the order specified in the Receives field
; Postconditions: returns a validated unsigned integer on the top of the stack
;	as a value
; Registers changed: ebp (saved by LOCAL), eax, ebx; also saves and restores
;	esi and ecx
;------------------------------------------------------------------------------
readVal		PROC	USES	eax ebx
	LOCAL	inputNum[15]:BYTE, valid:DWORD

	; save esi and ecx and set up strings for getString
		push	esi
		push	ecx
		mov	eax, [ebp + 16]		; numPrompt = [ebp + 16]
		lea	ebx, inputNum
	inputLoop:
		getString	eax, ebx, LENGTHOF inputNum
		mov	ebx, [ebp + 8]		; load errorMsg for validate
		push	ebx
		lea	eax, valid
		push	eax
		lea	eax, inputNum
		push	eax
		push	LENGTHOF inputNum
		call	validate
		pop	edx
		mov	[ebp + 16], edx	; store converted number in [ebp + 16]
		mov	eax, valid	; determine if input number is valid
		cmp	eax, 1
		mov	eax, [ebp + 12]
		lea	ebx, inputNum
		jne	inputLoop

	pop	ecx
	pop	esi
	ret	8	; do not clean up [ebp + 16], as it stores the number
readVal		ENDP

;------------------------------------------------------------------------------
; validate
; Validates that a user input string is an unsigned integer. Calls on
;	convertNum to verify that user input integer fits in a 32-bit register.
;	Returns a number on top of the stack as a value. Changes the value of
;	valid (from readVal) accordingly.
; Receives: errorMsg (reference), valid (reference), inputNum (address),
;	LENGTHOF inputNum (value)
; Returns: a number on top of the stack, and changes the value of valid to 0 if
;	the string is invalid (if valid = 0, input is repeated in readVal, so
;	number is discarded)
; Preconditions: errorMsg, valid, and inputNum are passed as offset and
;	LENGTHOF inputNum is passed as value, and are pushed to the stack in
;	the order specified in the Receives field
; Postconditions: number is validated, valid is changed to 0 if number is
;	invalid, number is top of the stack
; Registers changed: ebp (saved by LOCAL), esi, ecx, eax, edx
;------------------------------------------------------------------------------
validate	PROC	USES	esi ecx eax edx
	LOCAL	tooLarge:DWORD

	; set up source index and loop counter
		mov	esi, [ebp + 12]
		mov	ecx, [ebp + 8]
		cld

	; load in string byte by byte and verify if they are digits
	loadStr:
		lodsb
		cmp	al, 0
		je	nullChar
		cmp	al, 48
		jl	invalid
		cmp	al, 57
		ja	invalid
		loop	loadStr

	; set valid to 0
	invalid:
		mov	edx, [ebp + 20]		; errorMsg = [ebp + 20]
		displayString	edx
		call	Crlf
		mov	edx, [ebp + 16]		; set valid to false
		mov	eax, 0
		mov	[edx], eax
		jmp	endVal

	; converts string to integer
	; uses convertNum and tooLarge to check if number is too large
	nullChar:
		mov	edx, [ebp + 8]	; check if anything was entered
		cmp	ecx, edx	; if ecx is the same as LENGTHOF array
		je	invalid		; 	null was entered
		lea	eax, tooLarge
		mov	edx, 0
		mov	[eax], edx
		push	[ebp + 12]
		push	[ebp + 8]
		lea	edx, tooLarge
		push	edx
		call	convertToNum
		mov	edx, tooLarge
		cmp	edx, 1
		je	invalid
		mov	edx, [ebp + 16]
		mov	eax, 1		; set valid to true
		mov	[edx], eax

	endVal:
		pop	edx	; pop whatever was returned from convertNum
		mov	[ebp + 20], edx	; store converted number in [ebp + 20]
		ret	12		; do not clean up [ebp + 20]
validate	ENDP

;------------------------------------------------------------------------------
; convertToNum
; Converts a string input of digits to an integer number. Reads in digit by
;	digit, multiplies by 10 if there is another digit, and adds that new
;	digit to the converted number. Verifies that number is not too large
;	for 32-bit registers (changes value of tooLarge accordingly).
;	Returns some number onto the stack, regardless of if number is too
;	large or not. If the number is too large, 0 will be passed back on the
;	stack, and tooLarge will be changed to 1, which is distinguishable from
;	a valid entry of 0, where tooLarge will be 0.
; Receives: inputNum (address), LENGTHOF inputNum (value), tooLarge (address)
; Returns: converted inputNum on top of stack (as value) (or 0 that
;	is discarded later by readVal) and changes tooLarge to 1 if input
;	number does not fit in a 32-bit register
; Preconditions: inputNum and tooLarge are passed as offset/address, and
;	LENGTHOF inputNum is passed as value in the order specified in the
;	Receives field, a string of digits has been input earlier.
; Postconditions: converted inputNum from string to integer is returned at the
;	top of the stack (as value), tooLarge is set to 1 if input is too large
; Registers changed: ebp (saved by LOCAL), esi, ecx, eax, ebx, edx
;------------------------------------------------------------------------------
convertToNum	PROC	USES	esi ecx eax ebx edx
	LOCAL	number:DWORD

	; set up esi, ecx loop counter, number, eax, ebx, and edx
		mov	esi, [ebp + 16]
		mov	ecx, [ebp + 12]
		lea	eax, number
		xor	ebx, ebx
		mov	[eax], ebx
		xor	eax, eax
		xor	edx, eax	; also clears overflow and carry flags
		cld

	; load in string bytes one by one and add to number
	loadDigits:
		lodsb
		cmp	eax, 0
		je	endLoad
		sub	eax, 48
		mov	ebx, eax
		mov	eax, number
		mov	edx, 10
		mul	edx
		jc	numTooLarge	; check for carry after multiply
		add	eax, ebx
		jc	numTooLarge	; check for carry after adding
		mov	number, eax	; store temporary number from eax
		mov	eax, 0		; reset eax
		loop	loadDigits

	endLoad:
		mov	eax, number
		mov	[ebp + 16], eax	; move converted number to stack
		jmp	finish

	; change tooLarge if number does not fit in 32-bit register
	numTooLarge:
		mov	ebx, [ebp + 8]	; tooLarge = [ebp + 8]
		mov	eax, 1		; set tooLarge to true
		mov	[ebx], eax
		mov	eax, 0
		mov	[ebp + 16], eax	; move placeholder 0 to stack

	finish:
		ret	8
convertToNum	ENDP

;------------------------------------------------------------------------------
; displayList
; Uses register indirect addressing to display an array of numbers of a given
;	size. Uses code from Lecture #20. Converts the integers to strings
;	before displaying, then uses the displayString macro.
; Receives: array (reference), LENGTHOF array (value), enteredMsg (reference),
;	commaSpace (reference)
; Returns: none
; Preconditions: array, enteredMsg, and commaSpace are passed as offset, and
;	LENGTHOF array is passed as value to the stack, and are pushed to the
;	stack in the order specified in the Receives field
; Postconditions: a label for the list is displayed to the console, and the
;	validated user-entered integers are displayed as strings to the console
; Registers changed: ebp (saved by stack frame setup), esi, ecx, ebx, edx
;------------------------------------------------------------------------------
displayList	PROC	USES	esi ebx ecx edx
	; set up stack frame
		push	ebp
		mov	ebp, esp
	; print array title
		call	Crlf
		mov	edx, [ebp + 28]
		displayString	edx
		call	Crlf
		mov	esi, [ebp + 36]
		mov	ecx, [ebp + 32]
		mov	ebx, 1	; counter for numbers in current row

	printNum:
		push	[esi]
		call	WriteVal
		add	esi, 4
		cmp	ebx, [ebp + 32]
		jge	endList	; do not write commaSpace after last number
		mov	edx, [ebp + 24]	; commaSpace = [ebp + 24]
		displayString	edx
		inc	ebx
		loop	printNum

	endList:
		call	Crlf

	pop	ebp
	ret	16
displayList	ENDP

;------------------------------------------------------------------------------
; displaySumAvg
; Displays the sum and average of an array of integers. Displays the sum and
;	average as strings by calling on writeVal.
; Receives: array (reference), LENGTHOF array (value), sumMsg (reference),
;	avgMsg (reference)
; Returns: none
; Preconditions: array, sumMsg, and avgMsg are passed as offset, and LENGTHOF
;	array is passed as value, and are pushed to the stack in the order
;	specified in the Receives field
; Postconditions: the calculated sum and average are printed to the console as
;	strings, with identifying text
; Registers changed: ebp (saved by stack frame setup), edx, esi, ecx, eax, ebx
;------------------------------------------------------------------------------
displaySumAvg	PROC	USES	esi	edx ecx eax ebx
	; set up stack frame
		push	ebp
		mov	ebp, esp

		mov	edx, [ebp + 32]		; sumMsg = [ebp + 32]
		displayString	edx
		mov	esi, [ebp + 40]		; array = [ebp + 40]
		mov	ecx, [ebp + 36]		; LENGTHOF array = [ebp + 36]
		xor	eax, eax	; also clears overflow and carry flags
	
	; calculate sum
	sumNumbers:
		add	eax, [esi]
		add	esi, 4
		loop	sumNumbers
	
	; display sum
		push	eax
		call	writeVal
		call	Crlf

	; calculate and display average
		mov	edx, [ebp + 28]		; avgMsg = [ebp + 28]
		displayString	edx
		cdq
		mov	ebx, [ebp + 36]		; LENGTHOF array = [ebp + 36]
		div	ebx
		push	eax
		call	writeVal
		call	Crlf

	pop	ebp
	ret	16
displaySumAvg	ENDP

;------------------------------------------------------------------------------
; writeVal
; Writes an integer to the console as a string. Calls on convertChar to convert
;	an integer to a string.
; Receives: integer (value)
; Returns: none
; Preconditions: integer is passed as value to the stack
; Postconditions: integer is printed as a string to the console
; Registers changed: ebp (saved by LOCAL), eax
;------------------------------------------------------------------------------
writeVal	PROC	USES	eax
	LOCAL	outputStr[11]:BYTE

		lea	eax, outputStr
		push	eax
		push	[ebp + 8]
		call	convertChar

		lea	eax, outputStr
		displayString	eax

	ret	4
writeVal		ENDP

;------------------------------------------------------------------------------
; convertChar
; Converts an integer to a string and saves it in outputStr (from writeVal).
; Receives: outputStr (reference), integer (value)
; Returns: none
; Preconditions: outputStr is passed as offset and integer is passed as value,
;	and are pushed to the stack in the order specified in the Receives field
; Postconditions: saves the converted integer as a string in outputStr
; Registers changed: ebp (saved by LOCAL), eax, ebx, ecx
;------------------------------------------------------------------------------
convertChar	PROC	USES	eax ebx ecx
	LOCAL	tempChar:DWORD

	; set up division of integer by 10
		mov	eax, [ebp + 8]
		mov	ebx, 10
		mov	ecx, 0
		cld

	; count the number of digits and push the digits in reverse order
	divideTen:
		cdq
		div	ebx
		push	edx		; remainder = last digit
		inc	ecx
		cmp	eax, 0
		jne	divideTen

		mov	edi, [ebp + 12]	; set up destination char array

	storeChar:
		pop	tempChar
		mov	al, BYTE PTR tempChar
		add	al, 48
		stosb
		loop	storeChar

		mov	al, 0
		stosb

	ret	8
convertChar		ENDP

;------------------------------------------------------------------------------
; displayEnd
; Displays the program ending to the user.
; Receives: thanksMsg (reference)
; Returns: none
; Preconditions: thanksMsg is passed as reference to the stack
; Postconditions: thanksMsg is printed to the console
; Registers changed: ebp (saved by stack frame setup), edx
;------------------------------------------------------------------------------
displayEnd	PROC	USES	edx
	; set up stack frame
		push	ebp
		mov	ebp, esp

		call	Crlf
		mov	edx, [ebp + 12]
		displayString	edx
		call	Crlf

	pop	ebp
	ret	4
displayEnd	ENDP

END	main