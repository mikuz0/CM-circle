

void SpektrAudio() {
  int sum=0;
  
   setAnalogMux(SOUND_PIN);
     for (int i = 0 ; i < FHT_N ; i++) {
    ADC_startConvert();
    sample = ADC_readWhenAvailable( );
    delayMicroseconds(52); // 42 верхнею частоту снизить до 7 кГц, частота дискритизации 14 кгц
    fht_input[i] = sample; // put real data into bins 
   
  }
    for (int i = 0 ; i < FHT_N ; i++) { sum=sum+sample; }
   sum=sum/FHT_N ;
  for (int i = 0; i < FHT_N ; i++)
  { fht_input[i] = fht_input[i] -sum; }
  
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht

    for (byte i = 0; i <  M_WIDTH; i++)
    {
      Sheight[i]=0;
      for (byte j = spec[i]; j < spec[i+1]; j++)
      {
        Sheight[i]=Sheight[i]+fht_log_out[j];
      }
      Sheight[i]=Sheight[i]/(spec[i+1]-spec[i]);
    }
     

}

