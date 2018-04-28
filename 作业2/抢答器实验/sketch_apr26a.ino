int led_1 = 10;
int led_2 = 11;
int led_3 = 12;
int btn1 = 4;
int btn2 = 5;
int btn3 = 6;
int beezer = 7;
int team1;
int team2;
int team3;
void setup()
{
pinMode(led_1,OUTPUT);
pinMode(led_2,OUTPUT);
pinMode(led_3,OUTPUT);

pinMode(beezer,OUTPUT);

pinMode(btn1,INPUT);
pinMode(btn2,INPUT);
pinMode(btn3,INPUT);
digitalWrite(led_1,LOW);
digitalWrite(led_2,LOW);  
digitalWrite(led_3,LOW); 
digitalWrite(beezer,HIGH);
}
void loop()
{
  team1 = digitalRead(btn1);
  if(team1==LOW)
  { digitalWrite(led_1,HIGH);
    digitalWrite(beezer,LOW);
  }
  else
  { digitalWrite(led_1,LOW);}
    team2 = digitalRead(btn2);
    if(team2==LOW)
    { digitalWrite(led_2,HIGH);
      digitalWrite(beezer,LOW);
    }
    else
    { digitalWrite(led_2,LOW);}
      team3 = digitalRead(btn3);
      if(team3==LOW)
      { digitalWrite(led_3,HIGH);
        digitalWrite(beezer,LOW);
      }
      else
      { digitalWrite(led_3,LOW);}
        team3 = digitalRead(btn3);
        if(team3==LOW)
        { digitalWrite(led_3,HIGH);
          digitalWrite(beezer,LOW);
        }
        else
        { digitalWrite(led_3,LOW);}
}

