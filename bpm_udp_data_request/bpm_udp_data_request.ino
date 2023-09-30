#include "WiFiModuleEEPROM.h"
#include<WiFiUdp.h>
#include "vserial_io.h"
#include <Ticker.h>

WiFiUDP udp;
int thresh=560;
boolean rflg=false;
int bpm=0;
long *qu;
int qu_i=0;
int qu_s=10;

long sum_rr=0;

struct hb_data
{
  int amp;
  long mil;
}
;

hb_data arr[2];

Ticker chker;

void setup()
{
  chker.attach_ms(20,calculateBPM);
  qu=(long*)malloc(qu_s*sizeof(long));
  for(int i=0;i<qu_s;i++)
  {
    addInQueue(600);
  }
  initVSerial(128,115200);
  initProcess(true);
  udp.begin(4210);
}

void loop()
{
  processVSerial();
  int i=udp.parsePacket();
  if(i>0)
  {
    uint8_t arr[i];
    udp.read(arr, i);
    String tmp="";
    for(int j=0;j<i;j++)
    {
      tmp+=(char)(arr[j]);
    }
    cmd(tmp);
  }
  //amp=analogRead(A0);
  //bmp_value=String(amp);
  //Serial.println(bmp_value);
  confProcess();
}

void updateReadValue()
{
  bpm=random(65,72);
}

void cmd(String a)
{
  if(a.equals("D:BPM_VALUE"))
  {
    String amp_str=String(bpm);
    replyUdpPacket("D:BPM_VALUE:"+amp_str);
  }
  else if(a.indexOf("D:DIAL:")==0)
  {
    dial(a.substring(7));
  }
  else if(a.equals("D:ECHO:BPM"))
  {
    replyUdpPacket("D:ECHO:BPM");
  }
}

void replyUdpPacket(String a)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  for(int i=0;i<a.length();i++)
  {
    udp.write((uint8_t)(a.charAt(i)));
  }
  udp.endPacket();
}

void checkFunction()
{
  sendVSer("AT");
}

void processVSerial()
{
  if(fetchVSerData())
  {
    String dec=decodeVSer();
  }
}

void dial(String ph)
{
  if(!ph.equals(""))
  {
    //hangPhone();
    sendVSer("ATD"+ph+";");
  }
}

void calculateBPM()
{
  int amp=analogRead(A0);
  if(rflg && amp<thresh)
  {
    long rr=arr[1].mil-arr[0].mil;
    if(rr>=500 && rr<=1300)
    {
      addInQueue(rr);
      int avg_rr=getAverageRR();
      bpm=60000/avg_rr;
      //Serial.println("BPM : "+String(bpm)+" ,RR : "+String(avg_rr));
    }
    arr[0].amp=arr[1].amp;
    arr[0].mil=arr[1].mil;
    arr[1].amp=0;
    arr[1].mil=0;
    rflg=false;
  }
  else if(!rflg && amp>=thresh)
  {
    if(amp>arr[1].amp)
    {
      arr[1].amp=amp;
      arr[1].mil=millis();
    }
    rflg=true;
  }
}

int getAverageRR()
{
  return (int)(sum_rr/qu_s);
}

void addInQueue(long a)
{
  sum_rr-=qu[qu_i];
  qu[qu_i]=a;
  sum_rr+=qu[qu_i];
  qu_i=(qu_i+1)%qu_s;
}
