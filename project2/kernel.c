/*
 * @author Elly Do, Sweta Tamrakar
 * @date February 15, 2022
 */

void putStr(int col, int row, char character[], char color);

void putChar(int col, int row, char character, char color);

int printString(char *str);

int readChar();

int readString(char *buf);

int readSector(char *buf, int absSector);

int handleInterrupt21(int ax, int bx, int cx, int dx);

void main() {
  //test for bonus 1
  char line[20];
  printString("Enter a line: \0");
  readString(line, 10);
  printString("\n\0");
  printString(line);
  
  //test for "Creating printString, readChar, and readString System Calls"
  /*
  char line[80];
  char ch[1];

  makeInterrupt21(); //setup ISR for interrupt 0x21

  interrupt(0x21, 0x00, "Type>\0", 0, 0);
  interrupt(0x21, 0x11, ch, 0, 0);
  line[0] = ch[0];
  line[1] = 0x00;
  interrupt(0x21, 0x00, line, 0, 0);
  */
  
  //test for "Reading a Sector from Disk via the BIOS (Interrupt 0x13)
  /*
  char buffer[512];
  readSector(buffer, 30);
  printString(buffer);
  */

  //test for "Reading from the Keyboard via the BIOS (Interrupt 0x16)

  //for readChar()
  /*char *buf = "*****\0";
  char ch;
  
  // just read the character.
  printString("Type a char: \0");
  ch = readChar();
  buf[2] = ch;
  printString("Read: \0");
  printString(buf);
  printString("\n\r\0");
  */

  //for readString()
  /*
  char line[20];
  printString("Enter a line: \0");
  readString(line);
  printString("\n\0");
  printString(line);
  */

   //test for "Printing to the Screen via the BIOS (Interrupt 0x10)
  /*printString("Hello World\0"); */
  
  //tests for project 1
  /*
  putStr(37, 14, "Elly Do", 0x4F);
  
  //0x4F is the hexadecimal for 01001111, which means white text on red
  //background
  putChar(35, 13, 'H', 0x4F);
  putChar(36, 13, 'E', 0x4F);
  putChar(37, 13, 'L', 0x4F);
  putChar(38, 13, 'L', 0x4F);
  putChar(39, 13, 'O', 0x4F);
  putChar(40, 13, ' ', 0x4F);
  putChar(41, 13, 'W', 0x4F);
  putChar(42, 13, 'O', 0x4F);
  putChar(43, 13, 'R', 0x4F);
  putChar(44, 13, 'L', 0x4F);
  putChar(45, 13, 'D', 0x4F);
  

  putInMemory(0xB000, 0x8140, 72);
  putInMemory(0xB000, 0x8141, 0x0F);
  putInMemory(0xB000, 0x8142, 69);
  putInMemory(0xB000, 0x8143, 0x0F);
  putInMemory(0xB000, 0x8144, 76);
  putInMemory(0xB000, 0x8145, 0x0F);
  putInMemory(0xB000, 0x8146, 76);
  putInMemory(0xB000, 0x8147, 0x0F);
  putInMemory(0xB000, 0x8148, 79);
  putInMemory(0xB000, 0x8149, 0x0F);
  putInMemory(0xB000, 0x814A, 32);
  putInMemory(0xB000, 0x814B, 0x0F);
  putInMemory(0xB000, 0x814C, 87);
  putInMemory(0xB000, 0x814D, 0x0F);
  putInMemory(0xB000, 0x814E, 79);
  putInMemory(0xB000, 0x814F, 0x0F);
  putInMemory(0xB000, 0x8150, 82);
  putInMemory(0xB000, 0x8151, 0x0F);
  putInMemory(0xB000, 0x8152, 76);
  putInMemory(0xB000, 0x8153, 0x0F);
  putInMemory(0xB000, 0x8154, 68);
  putInMemory(0xB000, 0x8155, 0x0F);
  */

  while(1) {
  /*infinite loop*/
  }
}

int handleInterrupt21(int ax, int bx, int cx, int dx){
  if(ax==0x00){
    return printString(bx);
  }else if(ax==0x11){
    char ch = readChar();
    char* buf = bx;
    buf[0] = ch;
    return 1;
  }else if(ax==0x01){
    return readString(bx);
  }else{
    return -1;
  }
}

int readSector(char *buf, int absSector) {
  // prbly we need to write into buffer
  int relSector;
  int head;
  int track;
  relSector = mod(absSector, 18) + 1;
  head = mod(absSector /18, 2);
  track = ( absSector / 36 );
  interrupt(0x13, 0x02 * 256 + 0x01, buf, track * 256 + relSector, head * 256 + 0x00);
  return 1;
}

/*
 * Helper method that carries out the function of the modulus operator. It 
 * yields the remainder when the first operand is divided by the second.
 */
int mod(int dividend, int divisor) {
  while (dividend >= divisor) {
    dividend = dividend - divisor;
  }
  return dividend;
}

int readString(char *buf, int maxChar) {
  int i = 0;
  char charRead[2];

  char enter = 0x0D;
  char ch;
  charRead[1]=0x00;

  ch=0x00;
  ch=readChar();
  while (ch != enter) {
    //if user hits backspace, remove the deleted character from the buf as well as the screen
    if(ch==0x08){
      i--; //move index one step back
      interrupt(0x10, 0x0E * 256 +0x08, 0, 0, 0); //move cursor one step back
      buf[i]=' '; //replace the deleted character with a space
      interrupt(0x10, 0x0E * 256 +buf[i], 0, 0, 0); //print out space on screen
      interrupt(0x10, 0x0E * 256 +0x08, 0, 0, 0); //move cursor one step back again
    } else if (i >= maxChar) {
      break;
      //buf[i] = 0x00;
      //return i;
    } else { //when we do not hit backspace or haven't reached the limit
      buf[i] = ch;
      charRead[0]=ch;
      printString(charRead);
      i++;
    }
    ch = readChar();
  }
  while(ch != enter){
    ch = readChar();
   }
  buf[i] = 0x00;
  return i;
}

int readChar() {
  return interrupt(0x16, 0x00, 0, 0, 0);
}

int printString(char *str) {
  int i = 0;
  while(str[i] != '\0') {
    char al = str[i];
    char ah = 0x0E;
    int ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);
    i++;
  }
  return i;
}

/*
 * Display a string in a specified color at a specified location on the 
 * screen
 */

void putChar(int col, int row, char character, char color) {
  putInMemory(0xB000, 0x8000 + (row * 160) + (col * 2), character);
  putInMemory(0xB000, 0x8000 + (row * 160) + (col * 2) + 1, color);
}

/*
 * Display a string in a specified color at a specified location on the 
 * screen
 */

void putStr(int col, int row, char character[], char color) {
  int i = 0;
  while (character[i] != '\0') {
      putChar(col, row, character[i], color);
      i++;
      col++;
    }
}