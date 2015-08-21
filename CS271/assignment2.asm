TITLE Fibonacci Numbers     (assignment2.asm)

; Assignment 2
; Description: Assignment 2 for CS271-400; calculates Fibonacci numbers.
;	1. Displays the program title and programmer's name. Gets the user's name
;		and greets the user.
;	2. Promps the user to enter the number of Fibonacci terms to be displayed.
;		Advises the user to enter an integer in the range 1-46.
;	3. Gets and validates user input (n).
;	4. Calculates and displays all of the Fibonacci numbers up to and
;		including the nth term. The results should be displayed 5 terms per
;		line with at least five spaces between terms.
;	5. Displays a parting message that includes the user's name and terminates
;		the program.

INCLUDE Irvine32.inc

; upper limit constant
UPPERLIM		EQU		46

.data
programIntro	BYTE	"Fibonacci Numbers", 0
promptName	BYTE	"What is your name? ", 0
hello		BYTE	"Hello, ", 0
instructions	BYTE	"Enter the number of Fibonacci terms to be displayed."
		BYTE	13, 10, "Give the number as an integer in the range "
		BYTE	"[1 ... 46].", 0
promptNum	BYTE	"How many Fibonacci terms do you want? ", 0
rangeError	BYTE	"Out of range. Enter a number in [1 ... 46]", 0
spaces		BYTE	"     ", 0
goodBye		BYTE	"Goodbye, ", 0

inputName	BYTE	30 DUP(0)
inputNum	DWORD	?
prevTerm	DWORD	?
column		DWORD	?

.code
main PROC
; a. introduction
	; display program title and programmer's name
		mov	edx, OFFSET programIntro
		call	WriteString
		call	Crlf
		call	Crlf

	; gets the user's name
		mov	edx, OFFSET promptName
		call	WriteString
		mov	edx, OFFSET inputName
		mov	ecx, SIZEOF inputName
		call	ReadString

	; greets user
		mov	edx, OFFSET hello
		call	WriteString
		mov	edx, OFFSET inputName
		call	WriteString
		call	Crlf

; b. userInstructions
	; display instructions
		mov	edx, OFFSET instructions
		call	WriteString
		call	Crlf
		call	Crlf

; c. getUserData
	; get number of Fibonacci terms to display and validate number
	; uses post-test loop

	asknumber:
		mov	edx, OFFSET promptNum
		call	WriteString
		call	ReadInt
		cmp	eax, 1
		jl	error
		cmp	eax, UPPERLIM
		jg	error
		jmp	done

	error:
		mov	edx, OFFSET rangeError
		call	WriteString
		call	Crlf
		jmp	asknumber

	done:
		call	Crlf
		mov	inputNum, eax
		
; d. displayFibs
	; calculate and display Fibonacci numbers
	; 5 terms per line with at least 5 spaces between terms

		; use eax for calculating new terms
		; use ebx for current Fibonaci term
		; use prevTerm for previous Fibonacci term
		mov	ebx, 0			; special case for n = 1, 2
		mov	prevTerm, 1		; special case for n = 1, 2
		mov	ecx, inputNum		; loop counter
		mov	column, 1		; column counter

	fibonacciLoop:
	; f(n) = f(n - 1) + f(n - 2)
	; new term (eax) = current term (ebx) + previous term (prevTerm)
		mov	eax, ebx
		add	eax, prevTerm
		; write new term
		call	WriteDec
		mov	edx, OFFSET spaces
		call	WriteString
		; copy current term to previous term
		mov	prevTerm, ebx
		; copy new term to current term
		mov	ebx, eax
		cmp	column, 5
		jge	newRow
		inc	column
		jmp	continueRow
		newRow:
			call	Crlf
			mov	column, 1
		continueRow:
			loop	fibonacciLoop

; e. farewell
	; display parting message
		call	Crlf
		call	Crlf
		mov	edx, OFFSET goodBye
		call	WriteString
		mov	edx, OFFSET inputName
		call	WriteString
		call	Crlf

	exit	; exit to operating system
main ENDP

END main
