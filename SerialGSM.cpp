// SerialGSM version 1.1
// by Meir Michanie
// meirm@riunx.com





// error codes
// http://www.developershome.com/sms/resultCodes2.asp
#include <SerialGSM.h>
#include <string.h>
#include <stdlib.h>

SerialGSM::SerialGSM(int rxpin,int txpin):
SoftwareSerial(rxpin,txpin)
{
 verbose=false;
 laststatuscode=0;
}

void SerialGSM::FwdSMS2Serial(){
  Serial.println("AT+CMGF=1"); // set SMS mode to text
  this->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  this->ReadLine();
  Serial.println("AT+CNMI=3,3,0,0"); // set module to send SMS data to serial out upon receipt 
  this->println("AT+CNMI=3,3,0,0"); // set module to send SMS data to serial out upon receipt 
  delay(200);
  this->ReadLine();
}

void SerialGSM::SendSMS(char * cellnumber,char * outmsg){
  if (strlen(outmsg) > 140)
  {
	Serial.println("Error: SMS Message was longer than 140 characters.");
	return;
  }
  
  this->Rcpt(cellnumber);
  if (verbose) Serial.println(rcpt);
  this->StartSMS();
  this->Message(outmsg);
  Serial.print(outmessage);
  this->print(outmessage);
  this->EndSMS();
  delay(500);
  this->ReadLine();
}

void SerialGSM::SendSMS(){
  if (verbose) Serial.println(rcpt);
  if (verbose) Serial.println(outmessage);
  this->StartSMS();
  Serial.print(outmessage);
  this->print(outmessage);
  this->EndSMS();
  delay(500);
  this->ReadLine();
}

void SerialGSM::DeleteAllSMS(){
  Serial.println("AT+CMGD=1,4"); // delete all SMS
  this->println("AT+CMGD=1,4"); // delete all SMS
  delay(200);
  this->ReadLine();
}

void SerialGSM::Reset(){
  Serial.println("AT+CFUN=0,1"); // Reset Modem, Disable Auto Power Saving
  this->println("AT+CFUN=0,1"); // Reset Modem, Disable Auto Power Saving
  delay(200);
  this->ReadLine();
}


void SerialGSM::EndSMS(){
  this->print(char(26));  // ASCII equivalent of Ctrl-Z
  Serial.println();

  //delay(5 * 1000); // the SMS module needs time to return to OK status
}

void SerialGSM::StartSMS(){

  Serial.println("AT+CMGF=1"); // set SMS mode to text
  this->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  this->ReadLine();

  Serial.print("AT+CMGS=");
  this->print("AT+CMGS=");

  this->print(char(34)); // ASCII equivalent of "

  Serial.print(rcpt);
  this->print(rcpt);

  this->println(char(34));  // ASCII equivalent of "

  delay(500); // give the module some thinking time
  this->ReadLine();

}

void SerialGSM::Call(char * cellnumber){
  this->Rcpt(cellnumber);
  if (verbose) Serial.println(rcpt);
    
  Serial.print("ATD");
  this->print("ATD");
  Serial.print(rcpt);
  this->print(rcpt);
  
  //End Command - ASCII carriage return
  this->print(char(13));
  Serial.println();
  
  //Let the module process
  delay(500);
  this->ReadLine();
}

void SerialGSM::Hangup(){

  Serial.print("ATH");
  this->print("ATH");
  
  //End Command - ASCII carriage return
  this->print(char(13));
  Serial.println();

  //Let the module process
  delay(500);
  this->ReadLine();
}

int SerialGSM::ReadLine(){
  static int pos=0;
  char nc;
  while (this->available()){
    nc=this->read();
    if (nc == '\n' or (pos > MAXMSGLEN) or ((millis()> lastrec + SERIALTIMEOUT)and (pos > 0)) ){
      nc='\0';
      lastrec=millis();
      inmessage[pos]=nc;
     pos=0;
     if (verbose) Serial.println(inmessage);
      return 1;
    }
    else if (nc=='\r') {
    }
    else{
      inmessage[pos++]=nc;
      lastrec=millis();
    }
  }
  return 0;
}

// GetGSMStatus returns one of the following status codes as reported from the GSM module
// 0 SIM card removed 
// 1 SIM card inserted 
// 2 Ring melody 
// 3 AT module is partially ready 
// 4 AT module is totally ready 
// 5 ID of released calls 
// 6 Released call whose ID=<idx> 
// 7 The network service is available for an emergency call 
// 8 The network is lost 
// 9 Audio ON 
// 10 Show the status of each phonebook after init phrase 
// 11 Registered to network
int SerialGSM::GetGSMStatus(){
  char status[2];
    //Parse the Status Code
    if (strstr(inmessage, "SIND: ") != NULL){
        int pos = 6;
        if(strstr(inmessage, "+SIND:")) pos++; 
        
		//Read in all numbers
        int i = 0;
        while(inmessage[pos + i] != NULL and inmessage[pos + i] != ','){
            status[i] = inmessage[pos + i];
            i++;
        }
		
		laststatuscode = atoi(status);
    }
  return laststatuscode;
}

int SerialGSM::ReceiveSMS(){
  static boolean insms=0;
  // Get the number of the sms sender in order to be able to reply
	if ( strstr(inmessage, "CMT: ") != NULL ){
	    insms=1;
	    int sf=6;
	    if(strstr(inmessage, "+CMT:")) sf++; 
		    for (int i=0;i < PHONESIZE;i++){
		      sendernumber[i]=inmessage[sf+i];
		    }
		sendernumber[PHONESIZE]='\0';
		return 0;
	 }else{ 
		if(insms) {
			insms=0;
			return 1;
		}
	}
  return 0;
}

int SerialGSM::ReceiveCall(){
  static boolean incall = 0;
	// Get the caller id
	if ( strstr(inmessage, "CLIP: ") != NULL ){
	    incall=1;
	    int sf=6;
	    if(strstr(inmessage, "+CLIP:")) sf++; 
		    for (int i=0;i < PHONESIZE;i++){
		      sendernumber[i]=inmessage[sf+i];
		    }
		sendernumber[PHONESIZE]='\0';
		return 0;
	 }else{ 
		if(incall) {
			incall=0;
			return 1;
		}
	}
  return 0;
}

boolean SerialGSM::Verbose(){
	return verbose;
}

void SerialGSM::Verbose(boolean var1){
	verbose=var1;
}

char * SerialGSM::Sender(){
	return sendernumber;
}


char * SerialGSM::Rcpt(){
	return rcpt;
}

char * SerialGSM::Message(){
	return inmessage;
}


void SerialGSM::Sender(char * var1){
	sprintf(sendernumber,"%s",var1);
}


void SerialGSM::Rcpt(char * var1){
	sprintf(rcpt,"%s",var1);
}

void SerialGSM::Message(char * var1){
	sprintf(outmessage,"%s",var1);
}

void SerialGSM::Boot(){
  int counter=0;
  while(counter++ < 15){
    if (verbose) Serial.print(".");
    delay(1000);
  }
  if (verbose) Serial.println();
  
}
