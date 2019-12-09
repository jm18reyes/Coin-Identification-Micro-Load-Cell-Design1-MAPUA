#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 3
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(7,OUTPUT);
  
  // Initialize I2C communications as Master
  Wire.begin();
  
  //Setup serial monitor
  Serial.begin(9600);
  Serial.println("I2C Master Demonstration");
}

/*
int sizeAr = 5; //size of array
int loadcellAr[5]; //load cell array
long val = 0; //reference value; also means the origin value;
float newVal = 0; //dati int
int count = 0;
int reference = 0;
int min1 = 0;
int min2 = 0;
int max1 = 0;
int max2 = 0;
int countmin = 0;
int countmax = 0;
int i = 0;
int j = 0;
float loadcell = 0; //dati long
*/

float newVal = 0;
int valueFlag = 0;
long val = 0;
int loadcell = 0;
int activeFlag = 1;
int count = 0;
int reference = 0;
int origReference = 0;
int difference = 0;
void loop() {
  // put your main code here, to run repeatedly:
  //version 2
  digitalWrite(7,LOW);
  delay(1000);

  Serial.println("Stabilizing reference value..");
  while (count != 7)
  {
    newVal = analogRead(A1);
    val = 0.3 * val    +   0.7 * newVal;
    if (newVal == val)
    {
      count = count + 1;
    }
  }
  reference = (int) val;

  if (valueFlag == 0)
  {
    
    Serial.println("Initialization Complete!");
    Serial.print("Reference: ");
    Serial.print(reference);
    Serial.print("\n");

    origReference = reference;
    difference = origReference - 17;
 
 
    Serial.println("Identifying...");
  
    delay(5000);
  /////////////////////////// input stabilizer
    val = 0;
    newVal = 0;
    count = 0;
    Serial.println("Stabilizing input value..");
    while (count != 7)
    {
     // Serial.println("pumasok sa while");
      newVal = analogRead(A1);
      val = 0.3 * val    +   0.7 * newVal;
      if (newVal == val)
      {
        count = count + 1;
      }
    }
  //////////////////////////
    loadcell = (int) val;
    Serial.println(loadcell);

 
    if ((loadcell >= reference - 1) && (loadcell <= reference + 1))
    {
      activeFlag = 0;
      digitalWrite(7,LOW);
    }
    else if (loadcell > (reference + 1))
    {
      activeFlag = 1;
      digitalWrite(7,HIGH);
    }
    else
    {
      Serial.println("Out of range");
      activeFlag = 0;
      digitalWrite(7,LOW);
    }
  
    if(activeFlag == 1)
    {
      if((loadcell >= (19 + difference)) && (loadcell <= (82 + difference)))
      {
        Serial.println("5 Cents");
        valueFlag = 1;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write("005");
        Wire.endTransmission();
      }
      else if((loadcell > (82 + difference)) && (loadcell <= (150 + difference)))
      {
        Serial.println("25 Cents");
        valueFlag = 2;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write("025");
        Wire.endTransmission();
      }
      else if((loadcell > (150 + difference)) && (loadcell <= (260 + difference)))  //dati 192 yung 2nd
      {
        Serial.println("1 Peso");
        valueFlag = 3;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write("1");
        Wire.endTransmission();
      }
      else if ((loadcell > (260 + difference)) && (loadcell <= (300 + difference))) // dati 192 yung first
      {
        Serial.println(" 5 Peso");
        valueFlag = 4;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write("5");
        Wire.endTransmission();
      }
      else
      {
        Serial.println("10 Peso");
        valueFlag = 5;
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write("10");
        Wire.endTransmission();
      }
    }
  }
  else if (valueFlag == 1)
  {
    if (!((reference >= (19 + difference)) && (reference<= (56 + difference))))
    {
      valueFlag = 0;
    }
  }
  else if (valueFlag == 2)
  {
    if (!((reference > (56 + difference)) && (reference <= (130 + difference))))
    {
      valueFlag = 0;
    }
  }
  else if (valueFlag == 3)
  {
    if(!((reference > (130 + difference)) && (reference <= (260 + difference))))  //dati 192 yung 2nd
    {
       valueFlag = 0;
    }
  }
  else if (valueFlag == 4)
  {
    if(!((reference > (260 + difference)) && (reference <= (300 + difference)))) //dati 192 yung first
    {
       valueFlag = 0;
    }
  }
  else
  {
    if(!(reference > (250 + difference)))
    {
       valueFlag = 0;
    }
  }
  val = 0;
  newVal = 0;
  count = 0;
  delay(5000);
}

/*
void sortVal(int a[])  // the array size is 5
{
  for (int i = 0; i < (5 - 1); i++)
  {
    for (int o = 0; o < (5 - (i + 1)); o++)
    {
      if (a[o] > a[o + 1])
      {
        int t = a[o];
        a[o] = a[o + 1];
        a[o + 1] = t;
      }
    }
  }
}

int mode(int z[], int mi, int mx)
{
  int modeCountMin = 0;
  int modeCountMax = 0;
  for (int x = 0; x < 5; x = x + 1)
  {
    if (z[x] == mi)
    {
      modeCountMin = modeCountMin + 1;
    }
    else if (z[x] == mx)
    {
      modeCountMax = modeCountMax + 1;
    }
  }
  if (modeCountMin > modeCountMax)
  {
    return mi;
  }
  else
  {
    return mx;
  }
}
*/
