const byte numChars = 32;
char receivedChars[numChars];    // mảng lưu data từ UART
char tempChars[numChars];        // mảng tạm được copy từ mảng trên dùng để tách chuỗi

// biến lưu giá trị sau khi tách chuỗi
int   stepnum   = 0;  // động cơ bước số X
float velocity1  = 0; 
float velocity2  = 0;// lưu giá trị vận tốc
float distance1  = 0;
float distance2  = 0;// lưu giá trị khoảng cách
float delayvel1  = 0;
float delayvel2  = 0;// độ rộng xung (us)
float revdis1    = 0;
float revdis2    = 0;// cm -> số vòng -> số xung
int state=0;
int dirServo;
boolean newData = false;

void recvWithStartEndMarkers();  // hàm nhận data
void parseData();                // hàm tách chuỗi
void updateParsedData();         // hàm cập nhật giá trị thông số động cơ
void returnData();

void uartLoop() {
  recvWithStartEndMarkers();   // đọc giá trị UART nằm trong "<" và ">", sau khi đọc xong newData: false -> true
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // copy và lưu giá trị vào mảng tạm để bảo vệ mảng gốc
    // vì strtok() dùng trong parseData() sẽ thay "," bằng "\0"
    parseData();
    updateParsedData();
    returnData();
    newData = false;
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;  // cờ xác nhận bắt đầu lưu giá trị từ serial về
  static byte ndx = 0;                    // index để duyệt mảng
  char startMarker = '<';                 // kí tự bắt đầu chuỗi data
  char endMarker = '>';                   // kí tự kết thúc chuỗi data
  char rc;

  // Quét data từ serial cho tới khi gặp start marker
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {          // nếu chuỗi nhập từ serial lớn hơn số lượng phần tử của mảng nhận
          ndx = numChars - 1;         // kí tự mới sẽ thay thế phần tử cuối cùng của mảng
        }
      }
      else {
        receivedChars[ndx] = '\0'; // nếu gặp end marker -> điền vào cuối mảng nhận kí tự kết thúc chuỗi - null
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseData() {      // chia nhỏ data từ UART dựa theo ","

  char * strtokIndx; // dùng làm index cho hàm strtok()

  strtokIndx = strtok(tempChars, ",");  // lấy phần tử đầu tiên
  velocity1 = atof(strtokIndx);            // convert sang integer

  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 3
  distance1 = atof(strtokIndx); 
  
  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 2
  velocity2 = atof(strtokIndx);          // convert sang float

           // convert sang float

  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 4
  distance2 = atof(strtokIndx);          // convert sang integer
  
  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 4
  state = atoi(strtokIndx);          // convert sang integer

  // kiểm tra hướng của động cơ
//  if (aaaa == 0) dir = 0;
//  else if (aaaa == 1) dir = 1;

  // Tính độ rộng xung (us)
  delayvel1 = (60.0 / (velocity1 * 3200)) * pow(10, 6); // T = 60 / (rpm * ppr) (giây)
  delayvel2 = (60.0 / (velocity2 * 3200)) * pow(10, 6);
  // Tính xung cho quãng đường cần di chuyển

  revdis1 =  distance1 * 3200 * 2; // 3200 xung nửa chu kỳ -> x2 -> 6400 xung 1cm

  
  revdis2 =   distance2 * 3200 * 2; // 1 vòng cuộn giấy đi ???
  
}

//void showValue(){
//  Serial.print("Which stepper motor: ");
//  Serial.println(stepnum);
//  Serial.print("Velocity(RPM): ");
//  Serial.println(velocity);
//  Serial.print("Distance(cm): ");
//  Serial.println(distance);
//  Serial.print("Pulse for distance: ");
//  Serial.println(revdis);
//  Serial.print("Pulse Width(us): ");
//  Serial.println(delayvel);
//  Serial.print("Dir: ");
//  Serial.println(dir);
//}
// Thứ nguyên          <Không,  v/p   ,     mm     , Không>
// Frame truyền motor1 <Motor, Vận Tốc, Quãng Đường, Hướng>
//                     <  1  ,   60   ,      5     ,   1  >
// Frame truyền motor2 <Motor, Vận Tốc, Quãng Đường, Hướng>
//                     <  2  ,   60   ,      5     ,   1  >
// Thứ nguyên          <Không,   Độ   ,     ??     ,  //  >
// Frame truyền servo  <Motor,góc quay,   Tốc độ   ,  //  >
//                     <  3  ,   90   ,      5     ,  //  >
void updateParsedData() {
  //showValue();
  
    step1Count = revdis1;
    numberDelay1 = int(delayvel1 / inter);
    step2Count = revdis2;
    numberDelay2 = int(delayvel2 / inter);
    
    angleServo = velocity1;
    numberDelay3 = distance1;
  
}

void returnData() {
  Serial.println("<" + String(receivedChars) + ">");
}
