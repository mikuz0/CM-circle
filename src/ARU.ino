int ARU ()
{
    setAnalogMux(SOUND_PIN);
    int samp =0;
    int min=1500, max=0;
    for ( byte i = 0; i < 255; i++)
    {
        ADC_startConvert();
        samp = ADC_readWhenAvailable( );
        if (min > samp)   {  min = samp;}
        
        if (max < samp)  {  max = samp ;}
    }
        samp=(max-min)/2;
if ( samp > ARU_MAX)
{
 con=true; 
 for (byte i = 0; i < (samp-ARU_MAX)/20; i++)
 {
   UpDown( con , false);
 }  
} ;
if (  samp < ARU_MIN)
{
  con=false; 
  UpDown( con , false);
};     
return samp;
}