#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
String dataIn;             //variabel penampung
boolean parsing = false;   //Inisialisasi (just in case)s
char inData[20] = "2,300";
char inData2[20];
String nilai[6];
byte index = 0;
int state = 0;
int nodes = 1;
long limit = 0;
String lw20[6];
String sudut[6];
String datasimpen;
String nilaibaru;
int hitung = 0;
//int urutan = [1,2,3,4,5];


char message[15];
String text = "?l,0\r\n";
String text2 = "?\r\n";


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(13, OUTPUT);
  delay(100);
}

void loop() {
  batasnode();
  if (state == 0)
  {
    delay(20);
    digitalWrite(4, HIGH);
    kirim();
  }
  else
  {
    digitalWrite(4, LOW);
    terima();
  }
}

void kirim()
{
  //Serial.println(hitung);
  hitung++;
  if (hitung == 4)
  {
    Serial.print("bb|");`1
    Serial.print(sudut[3]);
    Serial.print("|");
    Serial.print(lw20[3]);
    Serial.print("|");
    Serial.print(String(nilai[1]) + "|" + String(nilai[2]));
    Serial.println("|");

    hitung = 0;
  }

  
  //Serial.print("bb|");
  String kirim = (String(nodes) + ",OK");
  mySerial.println(kirim);
  //Serial.println(kirim);
  state = 1;
  limit = 0;

}

void terima()
{
  if (mySerial.available()) {
    char inChar = (char)mySerial.read();                //serial in char per char
    limit = 0;
    //Serial.println(String(inChar));
    if (inChar == '\n')                  //indikasi paket data selesai (CR)
    {
      //Serial.println(dataIn);
      nilaibaru = "";
      for (int s = 0; s < 15; s++)
      {
        nilaibaru += String(inData[s]);
      }
      parsingData();
      index = 0;
      inData[index] = NULL;
      dataIn = "";
    }
    else
    {
      dataIn += String(inChar);
      inData[index] = inChar;
      index++;
      inData[index] = '\0'; // Keep the string NULL terminated
    }
  }
  else
  {
    limit++;
    if (limit > 35000)
    {
      //Serial.println(limit);
      Serial.println("timeout");
      limit = 0;
      state = 0;
      nodes++;
    }
  }

}

void parsingData()
{

  //Serial.println("data serial masuk : ." + dataIn + ".");
  dataIn.toCharArray(inData, 20);
  char *p = inData; //assign the string to *p
  char *str;        //intialize str
  String wew[20];
  int counter = 0; //initialise the counter
  //Serial.println("stuck");
  while (str = strtok_r(p, ",", &p))
  {
    wew[counter] = str;
    counter++;
  }
  //Serial.println(wew[1]);
  if (wew[0].toInt() == nodes)
  {
    if (nodes == 3 || nodes == 6)
    {
      lw20[nodes] = "0";
      lw20[nodes] = wew[2];
      sudut[nodes] = wew[1];
    }
    else
    {
      nilai[nodes] = "0";
      nilai[nodes] = wew[1];
    }
    state = 0;
    nodes++;
    digitalWrite(13, LOW);
  }
  else
  {
    nilai[nodes] = "0";
    nilai[nodes] = ' ';
    state = 1;
    nodes++;
    digitalWrite(13, LOW);
  }
}

void batasnode()
{
  if (nodes > 3)
  {
    nodes = 1;
  }
}

