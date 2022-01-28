
void fireRoutine() {
  static uint32_t prevTime = 0;
  if (millis() - prevTime > 30) {
    prevTime = millis();
    if (pcnt >= 100) {
      shiftUp();
      generateLine();
      pcnt = 0;
    }
    drawFrame(pcnt);
    pcnt += 30;
    matrix.show();
  }
}


// Randomly generate the next line (matrix row)

void generateLine() {
  for (uint8_t x = 0; x < M_WIDTH; x++) {
    line[x] = random(64, 255);
  }
}

//shift all values in the matrix up one row

void shiftUp() {
  for (uint8_t y = M_HEIGHT - 1; y > 0; y--) {
    for (uint8_t x = 0; x < M_WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x % 16;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < M_WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x % 16;
    matrixValue[0][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(int pcnt) {
  int nextv;

  //each row interpolates with the one before it
  for (unsigned char y = M_HEIGHT - 1; y > 0; y--) {
    for (unsigned char x = 0; x < M_WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x % 16;
      if (y < 8) {
        nextv =
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));

        mData color = mHSV(
                          HUE_START + pgm_read_byte(&(hueMask[y][newX])), // H
                          255, // S
                          (uint8_t)max(0, nextv) // V
                        );

        matrix.set(x, y, color);
      } else if (y == 8) {
        if (random(0, 20) == 0 && matrix.get(x, y - 1) != 0) matrix.set(x, y, matrix.get(x, y - 1));
        else matrix.set(x, y, mHEX(0));
      } else if (true) {

        // старая версия для яркости
        if (matrix.get(x, y - 1) > 0)
          matrix.set(x, y, matrix.get(x, y - 1));
        else matrix.set(x, y, mHEX(0));
      }
    }
  }

  //first row interpolates with the "next" line
  for (unsigned char x = 0; x < M_WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x % 16;
    mData color = mHSV(
                      HUE_START + pgm_read_byte(&(hueMask[0][newX])), // H
                      255,           // S
                      (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
                    );
    matrix.set(x, 0, color);
  }
}