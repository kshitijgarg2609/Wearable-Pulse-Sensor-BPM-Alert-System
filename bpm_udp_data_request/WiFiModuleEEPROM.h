#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

void initProcess(boolean);
void processHotspotFunction();
void toggleHotspot();
void ON();
void OFF();
void ON_AP();
void OFF_AP();
void confProcess();
void webServerProcess();
String getHtmlPage();
void initWebServer();
void erase();
void checkCode();
void checkActive();
void setCode();
char getCode();
void setActive(boolean);
boolean getActive();
void setWFID(String);
String loadWFID();
void setKey(String);
String loadKey();
void writeString(int,String);
String fetchString(int);

boolean stat_actv;
int conf=0;
int lst_msg=0;

String wssid_str;
String wkey_str;

int code,actv,base,wssid,wkey;

char symbol='K';

ESP8266WebServer ht_server(80);
int push_button_pin=D3;
int led_pin=LED_BUILTIN;
boolean led_flg;
boolean hflg=true;
boolean tflg=false;
String ssid_ap="K";
String key_ap="123456789";

String serial_data="";
boolean serial_flg=false;

void initProcess(boolean a)
{
  WiFi.mode(WIFI_AP_STA);
  code=0;
  actv=1;
  base=2;
  wssid=base;
  wkey=wssid+32+1;
  EEPROM.begin(150);
  checkCode();
  wssid_str=loadWFID();
  wkey_str=loadKey();
  checkActive();
  led_flg=a;
  if(led_flg)
  {
    pinMode(led_pin,OUTPUT);
  }
  pinMode(push_button_pin,INPUT_PULLUP);
  toggleHotspot();
  initWebServer();
}

void processHotspotFunction()
{
  boolean cflg=(digitalRead(push_button_pin)==LOW)&&(digitalRead(push_button_pin)==LOW);
  if(tflg==cflg)
  {
    return;
  }
  tflg=cflg;
  if(!tflg)
  {
   toggleHotspot();
   delay(1);
  }
}

void toggleHotspot()
{
  if(!hflg)
  {
    ON_AP();
    hflg=true;
  }
  else
  {
    OFF_AP();
    hflg=false;
  }
}

void ON()
{
  WiFi.begin(wssid_str,wkey_str);
  setActive(true);
}

void OFF()
{
  WiFi.disconnect();
  setActive(false);
}

void ON_AP()
{
  WiFi.softAP(ssid_ap,key_ap);
  if(led_flg)
  {
    digitalWrite(led_pin,LOW);
  }
}

void OFF_AP()
{
  WiFi.softAPdisconnect();
  if(led_flg)
  {
    digitalWrite(led_pin,HIGH);
  }
}

void confProcess()
{
  processHotspotFunction();
  webServerProcess();
}

void initWebServer()
{
  ht_server.on("/",[]()
  {
    if(ht_server.args())
    {
      if(conf==0)
      {
        if(ht_server.hasArg("conf")&&ht_server.arg("conf").equals("Configure"))
        {
          conf=1;
        }
      }
      else if(conf==1)
      {
        if(ht_server.hasArg("conf")&&ht_server.arg("conf").equals("Exit"))
        {
          conf=0;
        }
        else if(ht_server.hasArg("stat_actv")&&ht_server.arg("stat_actv").equals("Activate"))
        {
          ON();
        }
        else if(ht_server.hasArg("stat_actv")&&ht_server.arg("stat_actv").equals("Deactivate"))
        {
          OFF();
        }
        else if(ht_server.hasArg("Update")&&ht_server.arg("Update").equals("Update"))
        {
          if(ht_server.hasArg("wssid"))
          {
            setWFID(ht_server.arg("wssid"));
          }
          if(ht_server.hasArg("wkey"))
          {
            setKey(ht_server.arg("wkey"));
          }
        }
      }
    }
    ht_server.send(200,"text/html",getHtmlPage());
  });
  ht_server.begin();
}

String getHtmlPage()
{
  String conf_button=(conf==1)?"Exit":"Configure";
  String actv_button=(stat_actv)?"Deactivate":"Activate";
  String html;
  if(conf==1)
  {
    html="<!DOCTYPE html>\
    <html>\
    <body>\
    <h2><u>WiFi CONFIGURATION</u></h2>\
    <form action='/' method='post'>\
    <input type='submit' name='conf' value='"+conf_button+"'>\
    </form>\
    <br>\
    <form action='/' method='post'>\
    <input type='submit' name='stat_actv' value='"+actv_button+"'>\
    </form>\
    <br>\
    <br>\
    <form action='/' method='post'>\
    <label for='wssid'>WiFi SSID :</label>\
    <br>\
    <input type='text' id='wssid' name='wssid' value='"+wssid_str+"'>\
    <br>\
    <label for='wkey'>WiFi Key :<br>\
    (type 'NULL' if WiFi has no password)</label>\
    <br>\
    <input type='text' id='wkey' name='wkey' value='"+wkey_str+"'>\
    <br>\
    <br>\
    <input type='submit' name='Update' value='Update'>\
    </form>\
    </body>\
    </html>";
  }
  else
  {
    html="<!DOCTYPE html>\
    <html>\
    <body>\
    <h2><u>WiFi CONFIGURATION</u></h2>\
    <form action='/' method='post'>\
    <input type='submit' name='conf' value='"+conf_button+"'>\
    </form>\
    </body>\
    </html>";
  }
  return html;
}

void webServerProcess()
{
  ht_server.handleClient();
}

void erase()
{
  OFF();
  setWFID("xyz");
  setKey("123456789");
}

void checkCode()
{
  if(getCode()!=symbol)
  {
    setCode();
  }
}

void checkActive()
{
  stat_actv=getActive();
  if(stat_actv)
  {
    ON();
  }
  else
  {
    OFF();
  }
}


//#############################################################################################
//set get

//#############################################################################################
//code
void setCode()
{
  EEPROM.put(code,symbol);
  EEPROM.commit();
  erase();
}

char getCode()
{
  char ccd;
  EEPROM.get(code,ccd);
  return ccd;
}

//actv
void setActive(boolean a)
{
  stat_actv=a;
  char actv_c;
  if(a)
  {
    actv_c='1';
  }
  else
  {
    actv_c='0';
  }
  EEPROM.put(actv,actv_c);
  EEPROM.commit();
}

boolean getActive()
{
  char actv_c;
  EEPROM.get(actv,actv_c);
  return (actv_c=='1');
}

//wifi id password
void setWFID(String a)
{
  int dlen=a.length();
  if(dlen>0&&dlen<=32)
  {
    writeString(wssid,a);
    wssid_str=a;
  }
}

String loadWFID()
{
  return fetchString(wssid);
}

void setKey(String a)
{
  int dlen=a.length();
  if(dlen==4&&a.equals("NULL"))
  {
    writeString(wkey,a);
    wkey_str=a;
  }
  else if(dlen>=8&&dlen<=64)
  {
    writeString(wkey,a);
    wkey_str=a; 
  }
}

String loadKey()
{
  String a=fetchString(wkey);
  if(a.equals("NULL"))
  {
    a="";
  }
  return a;
}

//#############################################################################################

void writeString(int a,String b)
{
  uint8_t alen=b.length();
  EEPROM.put(a,alen);
  for(int i=0;i<alen;i++)
  {
    char c=b.charAt(i);
    EEPROM.put(a+1+i,c);
  }
  EEPROM.commit();
}

String fetchString(int b)
{
  String tmp="";
  uint8_t alen;
  EEPROM.get(b,alen);
  for(int i=0;i<alen;i++)
  {
    char c;
    EEPROM.get(b+1+i,c);
    tmp+=c;
  }
  return tmp;
}
