# Password-Based Door Locker System

# Description
 Based on 2 ATMega16 Microcontrollers one (HMI) for user interface using LCD and keypad and the other (controller) for storing data in the external M2416 EEPROM controlling the DC
 motor and the Buzzer used for the alarm.
 Driver designed and implemented for project: Timer, PWM, I2C,UART,LCD and Keypad.

# Human Machine Interface Microcontroller(HMI_MCU):
 Type : ATmega16 MCU   
 It's connected to a Liquid Crystal Display(LCD) screen and a Keypad.LCD is used to display messages to the user and the Keypad is used to enter the password or choose any
 option.
 
# Control Microcontroller(Control_MCU):
 Type : ATmega16 MCU   
 It's connected to a Direct Current(DC) motor, M2416 Electrically Erasable Programmable Read-Only Memory(EEPROM) and a Buzzer.The DC motor is used to open/close the door,M2416 
 EEPROM is an external non-volatile memory used to store the user password and the buzzer is used as an alarm if the user is wrongle enterd the password 3 times.
 
# How it works?
 1- In the very begining the user is asked to enter a new password for the system through the LCD.
    *Notice: The user will be asked to enter a new PW for only the first time in the lifetime of the system(implemented using a non-volatile flag in the external EEPROM).* 
 2- The user enters a 5 characters/number/mixed through the Keypad.
 3- The user is asked to reenter the password 2 times to confirm it as the new password for the system.
 4- The confirmed PW is sent to the Control_MCU via Universal Asynchronous Receiver Transmitter(UART).
 5- The confiremd PW is sotred in M2416 external EEPROM.
 6- If anyone tries to open the system again the system will ask them to enter the PW if the entered PW is correct then the user will be asked to choose one of the two main
    options which are: 1- To open the door.
                        2- To change the PW.
 7-If he chose to open the door,the DC motor will rotate clockwise to open the door for 15 seconds then will be held for 2 minutes waiting the user to pass through it then
   it rotate for 15 seconds anti-clockwise to close the door.
 8- If the entered PW is wrong the user will be asked 2 more times to enter the correct pw if he fialed then the buzzer will shoutout for 2 minutes to indicate that the user is a
    theif.
 
    
