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
