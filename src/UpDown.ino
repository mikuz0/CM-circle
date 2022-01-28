
void UpDown (bool ud,  bool res)
{
    
    

// !cs-5 pin --> CS_PIN
// u/!d - 8 pin --> UD_PIN

digitalWrite(CS_PIN, HIGH);
delayMicroseconds(T1);
//if (res ) counter=0 ;

if (ud )
{
    // PLUS
digitalWrite(UD_PIN , HIGH);
delayMicroseconds(T1);
digitalWrite(CS_PIN, LOW);
delayMicroseconds(T1); 
digitalWrite(UD_PIN , LOW);
delayMicroseconds(Ti); 
digitalWrite(UD_PIN , HIGH);
delayMicroseconds(T1);

counter = counter +1;
}
else
{
    // code MINUS
digitalWrite(UD_PIN , LOW);
delayMicroseconds(T1);
digitalWrite(CS_PIN, LOW);
delayMicroseconds(T1); 
digitalWrite(UD_PIN , HIGH);
delayMicroseconds(Ti); 
digitalWrite(UD_PIN , LOW);
delayMicroseconds(T1);
counter = counter - 1;
}

digitalWrite(CS_PIN, HIGH);

Serial.print("{P2|UP_DOWN|255,0,0|0 ");
Serial.print(counter);
Serial.println("}"); 

}
