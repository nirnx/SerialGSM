#ifndef _SerialGSM_H
#define _SerialGSM_H
#include "Arduino.h"
#include <SoftwareSerial.h>

#define SERIALTIMEOUT 2000
#define PHONESIZE 13
#define MAXMSGLEN 160

class SerialGSM : public SoftwareSerial {
public:
  SerialGSM(int rxpin,int txpin);
  void FwdSMS2Serial();
  void SendSMS();
  void SendSMS(char * cellnumber,char * outmsg);
  void DeleteAllSMS();
  void Reset();
  void EndSMS();
  void StartSMS();
  void Call(char * cellnumber);
  void Hangup();
  int ReadLine();
  int GetGSMStatus();
  int ReceiveSMS();
  int ReceiveCall();
  void Verbose(boolean var1);
  boolean Verbose();
  void Sender(char * var1);
  char * Sender();
  void Rcpt(char * var1);
  char * Rcpt();
  void Message(char * var1);
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
  char sendernumber[PHONESIZE + 1];
  char rcpt[PHONESIZE + 1];
  char outmessage[160];
  char inmessage[160];
  
protected:
  unsigned long lastrec;
  int laststatuscode;
};

#endif /* not defined _SerialGSM_H */

