    MVI A, 'H'  ; Set A to the ASCII value of 'H'
    CALL PRINT  ; Call the subroutine to print the character

    MVI A, 'e'  ; Set A to the ASCII value of 'e'
    CALL PRINT  ; Call the subroutine to print the character

    MVI A, 'l'  ; Set A to the ASCII value of 'l'
    CALL PRINT  ; Call the subroutine to print the character

    MVI A, 'l'  ; Set A to the ASCII value of 'l'
    CALL PRINT  ; Call the subroutine to print the character

    MVI A, 'o'  ; Set A to the ASCII value of 'o'
    CALL PRINT  ; Call the subroutine to print the character 
    
    MVI A, '!'  ; Set A to the ASCII value of '!'
    CALL PRINT  ; Call the subroutine to print the character

    HLT          ; Halt the program execution

PRINT:
    OUT 1
    RET         ; Return from the subroutine

