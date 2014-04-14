#ifndef _SerialGSM_H
#define _SerialGSM_H
#include "Arduino.h"
#include <GSMSoftwareSerial.h>

#define SERIALTIMEOUT 2000
#define PHONESIZE 12
#define MAXMSGLEN 160

class SerialGSM : public GSMSoftwareSerial {
public:
  SerialGSM(int rxpin,int txpin);
  void registerSMSCallback(int (*callback)(void));
  void FwdSMS2Serial();
  void SendSMS(char * cellnumber,char * outmsg);
  void DeleteAllSMS();
  void Reset();
  void Call(char * cellnumber);
  void Hangup();
  int ReadLine();
  int GetGSMStatus();
  boolean ErrorOccured();
  int ReceiveCall();
  void Verbose(boolean var1);
  boolean Verbose();
  void Sender(char * var1);
  char * Sender();
  void Rcpt(char * var1);
  char * Rcpt();
  char * Message();
  void Boot();
  static const int STATUS_SIM_REMOVED = 0;
  static const int STATUS_SIM_INSERTED = 1;
  static const int STATUS_RINGING = 2;
  static const int STATUS_MODULE_PARTIALLYREADY = 3;
  static const int STATUS_MODULE_READY = 4;
  static const int STATUS_ID_RELEASED_CALLS = 5;
  static const int STATUS_RELEASED_CALL_ID = 6;
  static const int STATUS_NETWORK_EMERGENCY_SERVICE = 7;
  static const int STATUS_NETWORK_SERVICE_LOST = 8;
  static const int STATUS_AUDIO_ON = 9;
  static const int STATUS_PHONEBOOK_INIT = 10;
  static const int STATUS_NETWORK_REGISTERED = 11;
  
  boolean verbose;
  char senderNumber[PHONESIZE + 1];
  char recipient[PHONESIZE + 1];
  char inMessage[160];
  
  
protected:
  int ReceiveSMS();
  boolean WaitResp(char * response, int timeout);
  unsigned long lastRec;
  int lastStatusCode;
  boolean errorOccured;

private:
  // SMS Receive Callback
  int (*onReceiveSMS)(void) ;
};

#endif /* not defined _SerialGSM_H */

