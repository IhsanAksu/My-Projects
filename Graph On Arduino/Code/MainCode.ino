#include <IRremote.hpp>
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);
extern uint8_t SmallFont[];
extern uint8_t TinyFont[];

/*
IR Commands:
(MyIRemoteTransmitter)
1  >> 69
2  >> 70
3  >> 71
4  >> 68
5  >> 64
6  >> 67
7  >> 7
8  >> 21
9  >> 9
0  >> 25
*  >> 22
#  >> 13
up >> 24
dw >> 82
rg >> 90
lf >> 8
ok >> 28
*/

int errorReturn = -1;
int IR_Delay = 50;
int IR_Pin = 2;

int IRNum[10] = {25,69,70,71,68,64,67,7,21,9};
bool timeWait = true;
int page = 0;
int page2v = 0;
int maxPage = 1;

//page0:Formula
int a = 0;
int b = 0;
int c = 0;
int selected = 0;

//page2:Graph
int xl = 128;
int yl = 64;

void setup() {
  //setup OLED and Serial Communication
  Serial.begin(115200);
  
  Serial.println("Oled Connection...");
  if(!myOLED.begin(SSD1306_128X64))
    while(1);
  Serial.println("Oled Connected!");
  myOLED.setFont(SmallFont);
  page0();

  IrReceiver.begin(2);
  Serial.println("IR Activated!");
}

int getIRChannel(){
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      //IrReceiver.printIRResultRawFormatted(&Serial);
      IrReceiver.resume();
      return errorReturn;
    }
    else {
      IrReceiver.resume();
      IrReceiver.stop();
      delay(IR_Delay);
      IrReceiver.begin(2);
      return IrReceiver.decodedIRData.command;
    }
  }
}

void cmdMain(int cmd){
  //page++
  if(cmd == 13){
    if(page<maxPage){
      page += 1;
    }
    else{
      page = 0;
    }
  }

  //page--
  else if(cmd == 22){
    if(page>0){
      page -= 1;
    }
    else{
      page = maxPage;
    }
  }
}

void page0C(int cmd){
  //selected++
  if(cmd == 82){
    if(selected<2){
      selected += 1;  
    }
  }
  //selected--
  else if(cmd == 24){
    if(selected>0){
      selected -= 1;  
    }
  }
  //set 0
  else if(cmd == 28){
    if(selected == 0){
      a = 0; 
    }
    else if(selected == 1){
      b = 0;
    }
    else if(selected == 2){
      c = 0;
    }
  }
  //negative
  else if(cmd == 90){
    if(selected == 0){
      a = -a; 
    }
    else if(selected == 1){
      b = -b;
    }
    else if(selected == 2){
      c = -c;
    }
  }
  else{
    int ind = findIndex(IRNum,cmd);
    if(!(ind==-1)){
      if(selected == 0){
        a = (a*10) + ind; 
      }
      else if(selected == 1){
        b = (b*10) + ind;
      }
      else if(selected == 2){
        c = (c*10) + ind;
      }
    }
  }
}

void page0(){
  myOLED.clrScr();
  myOLED.print("a:", 15, 10);
  myOLED.printNumI(a, 40, 10);
  myOLED.print("b:", 15, 20);
  myOLED.printNumI(b, 40, 20);
  myOLED.print("c:", 15, 30);
  myOLED.printNumI(c, 40, 30);
  myOLED.print(">>",0, (selected+1)*10);

  int delta = (b*b)-(4*a*c);
  myOLED.print("Delta:", 15, 50);
  myOLED.printNumI(delta, 55, 50);

  
  //delta == 0
  if(delta == 0){
    int x1 = (-b + sqrt(delta))/(2*a);
    myOLED.print("X1:", 80, 10);
    myOLED.printNumI(x1, 100, 10);
  }

  //delta > 0
  else if(delta > 0){
    float x1 = (-b + sqrt(delta))/(2*a);
    myOLED.print("X1:", 80, 10);
    myOLED.printNumF(float(x1), 1, 100, 10);
    myOLED.print("X2:", 80, 20);
    x1 = (-b - sqrt(delta))/(2*a);
    myOLED.printNumF(float(x1), 1, 100, 20);
  }

  myOLED.update();
}

void page2C(int cmd){
  //xl += 64
  if(cmd == 8){
    xl += 16;
  }
  //xl -= 64
  else if(cmd == 90){
    xl -= 16;
  }
  //yl += 32
  else if(cmd == 24){
    yl += 8;
  }
  //yl -= 32
  else if(cmd == 82){
    yl -= 8;
  }
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void page2(){
  myOLED.clrScr();
  myOLED.drawLine(0, 32, 128, 32);
  myOLED.drawLine(64, 0, 64, 64);
  myOLED.setFont(TinyFont);
  myOLED.printNumI(-xl/2, 0, 40);
  myOLED.printNumI(xl/2, 100, 40);
  myOLED.printNumI(yl/2, 70, 0);
  myOLED.printNumI(-yl/2, 70, 50);
  myOLED.setFont(SmallFont);
  for(int x = 0; x < 128; x++){
    float fixedX = mapFloat(x,0, 128, -xl/2, xl/2);
    float y = (a*fixedX*fixedX)+(b*fixedX)+(c);
    int oledy = mapFloat(y,-yl/2, yl/2, 64, 0);
    Serial.print(fixedX);
    Serial.print("***");
    Serial.println(y);
    myOLED.invPixel(x,oledy);
    
  }
  myOLED.update();
}



int findIndex(int arr[], int target) {
    int index = -1;
    // Listeyi dolaşarak değeri arıyoruz
    for (int i = 0; i < 10; i++) {
        if (arr[i] == target) {
            index = i;
            break;
        }
    }
    return index;
}

void loop() {
  int cmd;
  if (IrReceiver.decode()) {
    cmd = getIRChannel();
    if (!IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(cmd);
      cmdMain(cmd);
      
      Serial.print("Page:");
      Serial.println(page);
      if(page == 0){
        page0C(cmd);
        page0();
      }
      else if(page == 1){
        page2C(cmd);
        page2();
      }
    }
  }
}

