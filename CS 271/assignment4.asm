TITLE Composite Numbers	(assignment4.asm)

; Assignment 4
; Description: Assignment 4 for CS271-400; composite number calculator.
;	1. The program will introduce the program name, programmer, and
;		instructions. The user is instructed to enter a number of
;		composite numbers to be displayed.
;	2. Prompts and validates user data:
;		a. The user is prompted to enter an integer in the range
;			[1, 400]. User enters a number n.
;		b. The program verifies that 1 <= n <= 400. If n is out of
;			range, the user is re-prompted until the user enters a
;			value in the specified range.
;	3. Calculate and display composites:
;		a. The program calculates and displays all of the composite
;			numbers up to and including the nth composite.
;		b. Results are displayed 10 composites per line with at least
;			3 spaces between the numbers.
;	4. Program displays a farewell to the user.
;	**EC: Align the output columns (note: for window size > 80)

INCLUDE	Irvine32.inc

; upper limit defined as a constant
UPPERLIM	EQU	400
LOWERLIM	EQU	1
COLUMNMAX	EQU	10

.data
programIntro	BYTE	"Composite Numbers", 9
		BYTE	"Programmed by Sharon Kuo", 13, 10
		BYTE	"*EC: Align the output columns", 0
instructions	BYTE	"Enter the number of composite numbers you would like "
		BYTE	"to see.", 10, 13
		BYTE	"I'll accept orders for up to 400 composites.", 0
promptNum	BYTE	"Enter the number of composites to display "
		BYTE	"[1 ... 400]: ", 0
rangeError	BYTE	"Out of range. Try again.", 0
spaces		BYTE	"   ", 9, 0
exitMsg		BYTE	"Results certified by Sharon Kuo. Goodbye!", 0

inputNum	DWORD	?

; helper variables for isComposite
column		DWORD	?
currentNum	DWORD	?
tryFactor	DWORD	?
tryFactorMax	DWORD	?

.code
main	PROC
	call	introduction
	call	getUserData	; validate called as part of getUserData
	call	showComposites	; isComposite called as part of showComposites
	call	farewell
	exit	; exit to operating system
main	ENDP

;------------------------------------------------------------------------------
; introduction
; Displays an introduction to the user.
; Receives: none
; Returns: none
; Preconditions: none
; Postconditions: prints program name, programmer name, and instructions to
;	console
; Registers changed: edx
;------------------------------------------------------------------------------
introduction	PROC
	; display program name and programmer name
		mov	edx, OFFSET programIntro
		call	WriteString
		call	Crlf
		call	Crlf
	; display instructions
		mov	edx, OFFSET instructions
		call	WriteString
		call	Crlf
		call	Crlf

	ret
introduction	ENDP

;------------------------------------------------------------------------------
; getUserData
; Requests a number from the user, from [1 ... 400], inclusive. Verifies that
;	input is within the indicated range. Displays an error message for
;	invalid numbers, and repeats prompt until a valid number is input.
; Receives: none
; Returns: changes value of inputNum
; Preconditions: none
; Postconditions: prints prompt to console, sets inputNum to a user input
;	number, and calls validate for input validation
; Registers changed: edx, eax
;------------------------------------------------------------------------------
getUserData	PROC
	; prompt user
		mov	edx, OFFSET promptNum
		call	WriteString
		call	ReadInt
		mov	inputNum, eax
		call	validate

	ret
getUserData	ENDP

;------------------------------------------------------------------------------
; validate
; Verifies that user input is within the valid range (from 1 to 400).
; Receives: none
; Returns: none
; Preconditions: user has supplied an input to inputNum through getUserData
; Postconditions: validates if inputNum is in [1, 400] and calls getUserData
;	if it is not in [1, 400]
; Registers changed: eax, edx
;------------------------------------------------------------------------------
validate	PROC
	; validate data
		mov	eax, inputNum
		cmp	eax, LOWERLIM
		jl	displayError
		cmp	eax, UPPERLIM
		jg	displayError
		jmp	validated	; number passes tests for >= 1 & <= 400

	; print error, call getUserData to prompt for another number
	displayError:
		mov	edx, OFFSET rangeError
		call	WriteString
		call	Crlf
		call	getUserData

	validated:
		ret
validate	ENDP

;------------------------------------------------------------------------------
; showComposites
; Displays calculated composite numbers. Calls isComposite for calculation of
;	the composite numbers. Displays 10 composite numbers per row, with aligned
;	columns, and at least 3 spaces between numbers.
; Receives: none
; Returns: none
; Preconditions: validated inputNum
; Postconditions: displays n composite numbers, where n is inputNum, with 10
;	numbers per row
; Registers changed: ecx
;------------------------------------------------------------------------------
showComposites	PROC
	; start loop counter at inputNum
	; manually set first composite number = 4
		mov	ecx, inputNum
		mov	currentNum, 4
		mov	column, 1

	; determine next composite number to print
	calculateLoop:
		call	isComposite

	; print number and spaces after number, and calculate if new row is needed
	printNum:
		mov	eax, currentNum
		call	WriteDec
		mov	edx, OFFSET spaces
		call	WriteString
		inc	currentNum
		cmp	column, COLUMNMAX
		jge	newRow
		inc	column
		jmp	continueLoop

	; print new line if needed
	newRow:
		call	Crlf
		mov	column, 1

	continueLoop:
		loop	calculateLoop

	ret
showComposites	ENDP

;------------------------------------------------------------------------------
; isComposite
; Calculates the next composite number to be displayed. Does this by dividing a
;	number x by 2, 3, ..., x - 1 until remainder of 0 is found, indicating that
;	x is a composite number. If attempted factoring reaches x - 1 without
;	a remainder of 0, then x is incremented and the process is repeated.
; Receives: none
; Returns: changes or leaves currentNum the same
; Preconditions: currentNum is >= 4
; Postconditions: checks if currentNum is composite; if not, increments
;	currentNum and checks if composite until a composite number is found
; Registers changed: eax, edx
;------------------------------------------------------------------------------
isComposite	PROC
	; set first number to try factoring by as 2, and set upper limit for
	;	numbers to try factoring as currentNum - 1
	resetFactoring:
		mov	eax, currentNum
		dec	eax
		mov	tryFactor, 2
		mov	tryFactorMax, eax

	; tries to factor currentNum by the current factor attempt number
	; 	(tryFactor); if remainder is 0, then a composite is found and procedure
	;	returns; if not, then increments tryFactor, checks if tryFactor is
	;	greater than the factoring upper limit (currentNum - 1), and tries
	;	factoring again
	; if no factors are found (tryFactor > tryFactorMax), then increment
	; 	currentNum and reset the factoring again
	tryFactoring:
		mov	eax, currentNum
		cdq
		div	tryFactor
		cmp	edx, 0
		je	compositeFound	; tryFactor is a factor of currentNum
		inc	tryFactor
		mov	eax, tryFactor
		cmp	eax, tryFactorMax
		jle	tryFactoring	; try factoring again with new factor
		inc	currentNum	; failed to find any factors
		jmp	resetFactoring	; try new number to factor

	compositeFound:
		ret
isComposite	ENDP

;------------------------------------------------------------------------------
; farewell
; Displays a farewell message to the user.
; Receives: none
; Returns: none
; Preconditions: none
; Postconditions: prints farewell to console
; Registers changed: edx
;------------------------------------------------------------------------------
farewell	PROC
	; display farewell
		call	Crlf
		call	Crlf
		mov	edx, OFFSET exitMsg
		call	WriteString
		call	Crlf

	ret
farewell	ENDP

END	main
