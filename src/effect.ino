// =========== РАДУГА - Вольтметр===========
void rainbow( byte height) {
  static byte hue = 0;
  hue += 5;
   for (int j = 0; j < height; j++)
   { for (int i = 0; i < M_WIDTH; i++)
    
      matrix.set(i, j, mWheel8(hue + j * 255 / M_WIDTH));
  }
}

// =========== КОНФЕТТИ ===========
void confetti() {
    byte x,y;
for (byte j = 0; j < M_WIDTH; j++)
{
  /* code */

if (Sheight[j]+j >35 ) { // слегка приподнимаем ВЧ для более красивой картинки
  x=random(0,16);
  y=random(0,16);
    if (matrix.get(x,y) == 0)
      if (random(0, 50) > 25) matrix.set(x,y, mWheel8(j*16 ));
  
}
}
for (int i = 0; i < NUM_LEDS; i++) {
    matrix.fade(i, 30); // fade(int num, byte val) - уменьшить яркость светодиода num на величину val
  } 
}

// =========== КОНФЕТТИ- ДОЖДЬ ===========
void conf_raine() {
  mData tmp;
  byte x,y;
for (byte j = 0; j < M_WIDTH; j++)
{
  /* code */

if (Sheight[j]+j >35 ) { // слегка приподнимаем ВЧ для более красивой картинки
  x=random(0,16);
  y=random(0,16);
    if (matrix.get(x,y) == 0)
      if (random(0, 50) > 25) matrix.set(x,y, mWheel8(j*16 ));
    
  
}
}
for (int i = 0; i < NUM_LEDS; i++) {
    matrix.fade(i, 30); // fade(int num, byte val) - уменьшить яркость светодиода num на величину val
  } 
  for (byte i = 0; i < M_HEIGHT-1; i++)
  {
   for (byte j = 0; j < M_WIDTH; j++)  { 
     tmp = matrix.get(j,i+1);
     if (tmp!= 0) {matrix.set(j, i, tmp); matrix.fade(j, i+1, 80);}
       
        }
   
  }
   
}

// =========== спектр РАДУГА ===========
void rainSpectr( ) {
  static byte hue = 0;
  static int counter_sp=0;
  byte tmp;
mashtab ();

for (byte i = 0; i < M_WIDTH/2; i++)
{
  Sheight[i+8]=Sheight[8-i];
}

// вращение
  int tmp_s=0;
  if (counter_sp < 801 ) { counter_sp=counter_sp+1;} else { counter_sp=0 ;} ;
tmp = round(counter_sp/50);
for (byte i = 0; i < tmp; i++)
{
  tmp_s = Sheight[0];
 for (byte j = 0; j < M_WIDTH-1; j++)
 {
   Sheight[j]=Sheight[j+1];
 }
 Sheight[M_WIDTH-1]=tmp_s;
}
// раскрашиваем в радугу и пишем в буфер
  hue += 5;
   for (int i = 0; i < M_WIDTH; i++)
   { for (int j = 0; j < Sheight[i]; j++) 
          matrix.set(i, j, mWheel8(hue + j * 255 / M_WIDTH));
  }
}

/*
// =========== ОГОНЬ ===========
mGradient<4> myGrad;
boolean loadingFlag2 = true;
void fire2D() {
  static int count = 0;
  if (loadingFlag2) {
    loadingFlag2 = false;
    // заполняем палитру
    myGrad.colors[0] = mBlack;
    myGrad.colors[1] = mRed;
    myGrad.colors[2] = mYellow;
    myGrad.colors[3] = mWhite;
  }
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, myGrad.get(inoise8(i * 50, j * 50, count), 255));
  count += 20;
}

*/
/*
// =========== РАДУЖНЫЕ ШТУКИ ===========
void rainbow2D() {
  static int count = 0;
  static byte count2 = 0;
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, mWheel8(count2 + inoise8(i * 50, j * 50, count), 255));
  count += 20;
  count2++;
}

*/
