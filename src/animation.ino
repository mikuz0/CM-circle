
void animation () 
{ 
  byte tmp;
  switch (mode)
  {
  case 1:
    tmp = VUmetr();
    matrix.clear();
    rainbow ( tmp );
    break;
  case 2:
    SpektrAudio();
    matrix.clear();
    rainSpectr( );
    break;
  case 3:
    SpektrAudio();
    conf_raine();
    break;
  case 4:
  SpektrAudio();
    confetti();
    break;
  case 5:
 fireRoutine();
    break;
  default:
  if (mode = 0) { mode=MAX_MODE ; } else
 { mode=1; }
    break;
  }
  
matrix.show();
  
 
}