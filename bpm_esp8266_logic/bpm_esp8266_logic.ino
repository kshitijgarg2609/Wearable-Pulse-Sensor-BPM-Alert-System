#include <Ticker.h>

int thresh=560;
boolean rflg=false;

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

hb_data fs[2];

Ticker chker;

void setup()
{
  Serial.begin(115200);
  chker.attach_ms(20,calculateBPM);
  qu=(long*)malloc(qu_s*sizeof(long));
  for(int i=0;i<qu_s;i++)
  {
    addInQueue(600);
  }
}

void loop()
{
  
}

void calculateBPM()
{
  int amp=analogRead(A0);
  if(rflg && amp<thresh)
  {
    long rr=fs[1].mil-fs[0].mil;
    if(rr>=500 && rr<=1300)
    {
      addInQueue(rr);
      int avg_rr=getAverageRR();
      int bpm=60000/avg_rr;
      Serial.println("BPM : "+String(bpm)+" ,RR : "+String(avg_rr));
    }
    fs[0].amp=fs[1].amp;
    fs[0].mil=fs[1].mil;
    fs[1].amp=0;
    fs[1].mil=0;
    rflg=false;
  }
  else if(!rflg && amp>=thresh)
  {
    if(amp>fs[1].amp)
    {
      fs[1].amp=amp;
      fs[1].mil=millis();
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
