const int ledPin=12;
const int decodeButton=2;
const int BUZZER=7;

String morseCode="";
String text="";
int startPos=0,endPos=0;
bool decodeButtonState=0;
bool prevButtonState=0;
bool buttonReleased=false;

String litere[26]={
  //A,  B,    C,       D,   E,    F,   G,      H,    I,   J,     K,
  ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",
  //L,    M,   N,   O,     P,      Q,    R,    S,   T,   U,    V,
  ".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",
  //W,    X,     Y,      Z
  ".--","-..-","-.--","--.."};



void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(decodeButton,  INPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);

Serial.println("*************************************************************");
Serial.println("                     Demonstration of Morse Code              ");
Serial.println("*************************************************************   ");
Serial.println("\Instructions. How to use the decoder");
Serial.println("1. First Write Your Morse code by pushing the button. Push and release for (.) Push and hold for (-)");
Serial.println("2. For Space between words  write 1 and Press Enter ");
Serial.println("3. When you are done with the sentence press 2 and Enter");
Serial.println("4. If you want to encode a text just write it in the serial monitor and press Enter");


Serial.println("\Enter Your Morse Code Here  ");
}

int startPressed = 0;    // the moment the button was pressed
int endPressed = 0;
int ss=0;
char updateState() {
  // the button has been jusst pressed
  char c;
  
  if (decodeButtonState != prevButtonState){
    
  if (decodeButtonState == HIGH) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(BUZZER, HIGH);
      startPressed = millis();
      
      buttonReleased = false;
  // the button has been just released
  } else{
    digitalWrite(ledPin,LOW);
    digitalWrite(BUZZER,LOW);
    buttonReleased = true;
      endPressed = millis();
      int holdTime = endPressed - startPressed;
      if (holdTime >= 100 && holdTime < 750) {
          //Serial.print(".");
          c='.';
          morseCode+=c;
          
      }
      else if (holdTime >= 750) {
          //Serial.print("-");
          c='-';
          morseCode+=c;
      }
      

      
  }
 
  prevButtonState=decodeButtonState;
  //Serial.println(morseCode);
  }
  
  
}
void process()
{
  decodeButtonState=digitalRead(decodeButton);
  updateState();
  delay(200);
}

char convertIntoText()
{
  int characterAscii=65;
  
  for(int i=0; i<26;   i++)
  {
    if(morseCode == litere[i])
    {
     Serial.print(char(characterAscii));
     break;
    }
    characterAscii++;  
  }
  morseCode="";

}

void dot()
{
  Serial.print(".");
  digitalWrite(ledPin, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  digitalWrite(BUZZER, LOW);
  delay(250);
}

void dash()
{
  Serial.print("-");
  digitalWrite(ledPin, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(250 * 3);
  digitalWrite(ledPin, LOW);
  digitalWrite(BUZZER, LOW);
  delay(250);
}

void encode(char c)
{
  String s=litere[int(c)-65];
  int len=s.length();
  for (int i=0;i<len;i++)
   {
    if(s[i]=='.') dot();
    else if(s[i]=='-') dash();
   }
}

int startPos1=0;
int endPos1=0;

void loop() {
  process();
  ss=millis();
    if(ss-endPressed>1800) {
        convertIntoText();
      }
  while(Serial.available() > 0 ) 
  { 
    
      int  ascii=Serial.read();
      
      if(ascii==49) //   49 is Ascii value of 1
            Serial.print(' ');
      else if(ascii==50)
             {Serial.println("end of text");
             Serial.println("Start of new translation:\n");}
      else {
        Serial.print(char(ascii));
        encode(ascii);
        Serial.println();
      }
      
  }


}

