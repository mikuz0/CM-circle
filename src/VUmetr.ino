
byte VUmetr() {

            // сбрасываем значения
    int SoundLevel_max = 0;
    int SoundLevel_min=1023;
    int ScurrentLevel;
    
        for (byte i = 0; i < 250; i ++) {    
                ADC_startConvert();
                ScurrentLevel = ADC_readWhenAvailable( );   // делаем  измерений
                                      
          
          if (SoundLevel_max < ScurrentLevel) SoundLevel_max = ScurrentLevel;   // ищем максимальное
          if (SoundLevel_min > ScurrentLevel) SoundLevel_min = ScurrentLevel ; //минимальная
                  } ;

ScurrentLevel=(SoundLevel_max-SoundLevel_min)/2;
return map( ScurrentLevel, 0 , 250, 0, M_HEIGHT );

}

 