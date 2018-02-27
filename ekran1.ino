
/* rozdzielczosc ekranu*/
#define SCREEN_H          30
#define SCREEN_W          40

#define nop __asm__("nop \n")

/*linie pionowe 480 */ 
#define V_FRONTP 10
#define V_SYNCPULSE 2
#define V_BACKP 33
#define V_BGRD 525
/*pixele 640 */
#define H_FRONTP 16
#define H_SYNCPULSE 96
#define H_BACKP 48
#define H_BGRD 800

/*Czujnik ultradźwiękowy */
#define TRIG 13
#define ECHO 12

/********************************************/
int  lineCounter;
char showRow;
char displayState;    //

char screen[SCREEN_H][SCREEN_W];

/*stany wyswietlania*/
#define IN_DRAW            1
#define IN_VSYNC_PULSE     2
#define IN_VSYNC_FP        3


#define LEFTSIDE         0
#define RIGHTSIDE        39
#define TOP             0
#define BOTTOM         29


 //pozycja jedności dla cyferek
#define POSITION_1     31
#define POSITION_2     26
#define POSITION_3     21
#define POSITION_4   16
#define POSITION_5    11

#define PIXEL_HEIGHT     1
#define PIXEL_WIDTH     1

#define COLOR_RED    0x5 //czerwony
#define COLOR_GREEN    0x6 // zielony
#define COLOR_YELLOW    0x7 //żółty
#define BG_COLOR   0x0 //czarny

#define THRESHOLD     4

char pencil;
char pixelX;
char pixelY;
char pixelColor;

double distance;
double timee;
double table[10];
double table2[5];

/*****************************************/
//PRZERWANIA//
/*skończonym przejściu śpi)*/
ISR(TIMER2_OVF_vect) {
  sei();
  __asm__("sleep \n");
}

ISR(TIMER2_COMPB_vect) {
  register char  screenCounter;
  register char* screenPtr;

  if (displayState == IN_DRAW) {
    screenPtr = &(screen[(lineCounter - V_SYNCPULSE - V_BACKP) >> 4][0]);
    screenCounter = SCREEN_W;

       while (screenCounter--) {
      PORTD = *(screenPtr++);
      nop;
    }

    nop; 
    nop; 
    nop;
    PORTD = 0;
  }
  else if ( displayState == IN_VSYNC_PULSE){
    PORTD = bit(PORTD4);
  }
  
  else if (displayState== IN_VSYNC_FP){

    /**/
    PORTD &= ~(bit(PORTD0) | bit(PORTD1) | bit(PORTD4));

  /*zerowanie lc*/
    if (lineCounter == (V_BGRD - 1)) {
      lineCounter = -1;
    }
  }

  lineCounter++;
  showRow = !showRow;

  if (lineCounter < V_SYNCPULSE) {
    displayState = IN_VSYNC_PULSE;
  } 
  else if (showRow && (lineCounter >= (V_SYNCPULSE + V_BACKP)) &&
    (lineCounter < (V_BGRD - V_FRONTP))) {
    displayState = IN_DRAW;
  } 
  else {
    displayState = IN_VSYNC_FP;
  }
}

/*****************************************/
void setup(void) {
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);  
  
  cli();
  
  PORTD  = 0;
  DDRD   = bit(DDD0)  | bit(DDD1)  | bit(DDD3)  | bit(DDD4); //ustawienie jako wyjścia (kolory)

  TCCR2A = bit(COM2B1) | bit(WGM21)  | bit(WGM20); //wejście w tryb crc
  TCCR2B = bit(WGM22)  | bit(CS21);                 //
  OCR2A  = 63;                                      //rate odświeżania
  OCR2B  =  7;                                      //rate odświeżania b
  TIMSK2 = bit(TOIE2)  | bit(OCIE2B); //mask registers
  TIFR2  = bit(TOV2)   | bit(OCF2B);  //flag

  SMCR = bit(SE);


  lineCounter  = 0;
  showRow      = 0;
  displayState = IN_VSYNC_PULSE;

  memset(screen, 0, sizeof(screen));

  sei();

  pixelY = (BOTTOM - PIXEL_HEIGHT);
  pixelX = (RIGHTSIDE - LEFTSIDE - PIXEL_WIDTH) / 2;
 ////
  distance = 3;
  timee = 2;
 // pixelX = (RIGHTSIDE - 1 );

  return;
}
/*****************************************/
inline void drawMe(void) {
  char r, c; 
  for (r = 0; r < SCREEN_H; r++) {
    for (c = 0; c < SCREEN_W; c++) {

      if(r==pixelY && c == pixelX){
        screen[r][c] = COLOR_RED;
      } 
      else {
        screen[r][c] = COLOR_YELLOW;
      }
    }
  }
  drawNumber((int)distance);
  return;
}

inline void drawNumber(int num){
  int licz = 0;
  int x;
  while(num != 0){
    x = num % 10;
    num = num / 10;
    licz++;
    drawNum(x, licz);
  }
}

void drawNum(int num, int pos){
  switch(num){
    case 0:
    if(pos == 1){
     draw0(POSITION_1, 1);
    }
    else if(pos == 2){
      draw0(POSITION_2, 1);
    }
    else if(pos == 3){
      draw0(POSITION_3, 1);
    }
    break;
    case 1:
    if(pos == 1){
     draw1(POSITION_1, 1);
    }
    else if(pos == 2){
      draw1(POSITION_2, 1);
    }
    else if(pos == 3){
      draw1(POSITION_3, 1);
    }
    break;
    case 2:
    if(pos == 1){
      draw2(POSITION_1, 1);
    }
    else if(pos == 2){
      draw2(POSITION_2, 1);
    }
    else if(pos == 3){
      draw2(POSITION_3, 1);
    }
    break;
    case 3:
    if(pos == 1){
      draw3(POSITION_1, 1);
    }
    else if(pos == 2){
      draw3(POSITION_2, 1);
    }
    else if(pos == 3){
      draw3(POSITION_3, 1);
    }
    break;
    case 4:
    if(pos == 1){
      draw4(POSITION_1, 1);
    }
    else if(pos == 2){
      draw4(POSITION_2, 1);
    }
    else if(pos == 3){
      draw4(POSITION_3, 1);
    }
    break;
    case 5:
    if(pos == 1){
      draw5(POSITION_1, 1);
    }
    else if(pos == 2){
      draw5(POSITION_2, 1);
    }
    else if(pos == 3){
      draw5(POSITION_3, 1);
    }
    break;
    case 6:
    if(pos == 1){
      draw6(POSITION_1, 1);
    }
    else if(pos == 2){
      draw6(POSITION_2, 1);
    }
    else if(pos == 3){
      draw6(POSITION_3, 1);
    }
    break;
    case 7:
    if(pos == 1){
      draw7(POSITION_1, 1);
    }
    else if(pos == 2){
      draw7(POSITION_2, 1);
    }
    else if(pos == 3){
      draw7(POSITION_3, 1);
    }
    break;
    case 8:
    if(pos == 1){
      draw8(POSITION_1, 1);
    }
    else if(pos == 2){
      draw8(POSITION_2, 1);
    }
    else if(pos == 3){
      draw8(POSITION_3, 1);
    }
    break;
    case 9:
    if(pos == 1){
      draw9(POSITION_1, 1);
    }
    else if(pos == 2){
      draw9(POSITION_2, 1);
    }
    else if(pos == 3){
      draw9(POSITION_3, 1);
    }
    break;
  }
}
/*******************************/
void draw0(int leftX, int topY){
  int i;
  int tmp = leftX;
  
  for (i = 0; i < 4; i++) {
    screen[topY][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  tmp = topY;
  for (i = 0; i < 4; i++) {
    screen[tmp][leftX] = COLOR_RED;
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
}

void draw1(int leftX, int topY){
  int tmp = topY;
  int i;
  for(i = 0; i<5; i++){
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
  screen[topY+1][leftX+2] = COLOR_RED;
  screen[topY+2][leftX+1] = COLOR_RED;
}

void draw2(int leftX, int topY){
  int tmp = leftX;
  int i;
  for(i = 0; i<4; i++){
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  screen[topY+1][leftX+3] = COLOR_RED;
  screen[topY+3][leftX] = COLOR_RED;
}

void draw3(int leftX, int topY){
   int tmp = topY;
  int i;
  for(i = 0; i<5; i++){
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
  tmp = leftX;
  for(i = 0; i<4; i++){
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
}

void draw4(int leftX, int topY){
  int tmp = topY;
  int i;
  for(i = 0; i<5; i++){
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
  screen[topY][leftX] = COLOR_RED;
  screen[topY+1][leftX] = COLOR_RED;
  tmp = leftX;
  for(i = 0; i<4; i++){
    screen[topY+2][tmp] = COLOR_RED;
    tmp++;
  }
}

void draw5(int leftX, int topY){
  int tmp = leftX;
  int i;
  for(i = 0; i<4; i++){
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  screen[topY+1][leftX] = COLOR_RED;
  screen[topY+3][leftX+3] = COLOR_RED;
}

void draw6(int leftX, int topY){
  int tmp = leftX;
  int i;
  for(i = 0; i<4; i++){
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  screen[topY+1][leftX] = COLOR_RED;
  screen[topY+3][leftX+3] = COLOR_RED;
  screen[topY+3][leftX] = COLOR_RED;
}

void draw7(int leftX, int topY){
  int tmp = topY;
  int i;
  for(i = 0; i<5; i++){
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
  tmp = leftX;
  for (i = 0; i < 4; i++) {
    screen[topY][tmp] = COLOR_RED;
    tmp++;
  }
}

void draw8(int leftX, int topY){
  int i;
  int tmp = leftX;
  
  for (i = 0; i < 4; i++) {
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  tmp = topY;
  for (i = 0; i < 4; i++) {
    screen[tmp][leftX] = COLOR_RED;
    screen[tmp][leftX+3] = COLOR_RED;
    tmp++;
  }
}

void draw9(int leftX, int topY){
    int tmp = leftX;
  int i;
  for(i = 0; i<4; i++){
    screen[topY][tmp] = COLOR_RED;
    screen[topY+2][tmp] = COLOR_RED;
    screen[topY+4][tmp] = COLOR_RED;
    tmp++;
  }
  screen[topY+1][leftX] = COLOR_RED;
  screen[topY+1][leftX+3] = COLOR_RED;
  screen[topY+3][leftX+3] = COLOR_RED;
}
void drawMyGame(){

}

/*****************************************/

int getPosition(int distance){
  int pos;
  if(distance <=1){
    pos = LEFTSIDE;
  }
  else if(distance >= 40){
    pos = RIGHTSIDE;
  }
  else{
    pos = distance;
  }
  return pos;
}

/****************************************/
void loop(void) {
  
  drawMe();
  int i;
  int d;
  int n = 0;
  double avg = 0;
  for(i = 0; i < 5; i++){
    n++;
    digitalWrite(TRIG,HIGH);
    delay(10);
    digitalWrite(TRIG,LOW);
    timee = pulseIn(ECHO,HIGH);
    d = (340*(timee-10))/2000;
    table2[i] = d;
    avg += d;
    if(i > 1){
      if(i > 2){
     /* if ((abs(table2[i-1] - d) > THRESHOLD) || (abs(table2[i-2] - d) >THRESHOLD)){
        i--;
        avg = avg - d;
      }*/
      if (((int)table2[i-1] - d) > THRESHOLD || ((int)table2[i-2] - d) > THRESHOLD ){
        n--;
        avg = avg - d;
      }
    }
  }
  delay(20);
  distance = avg/n;
  //distance = avg/5;
  pixelX = getPosition((int)distance);
  delay(10);
  }
}
