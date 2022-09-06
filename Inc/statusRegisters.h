#ifndef __statusRegisters_H
#define __statusRegisters_H

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
#define FIRMWARE_VERSION 100

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void updateStatusRegisters();
void canSendStatusRegistersUART();
void loadFirmwareVersion();

#endif /* __statusRegisters_H */
