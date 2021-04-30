const byte numChars = 32;
char receivedChars[numChars];    // mảng lưu data từ UART
char tempChars[numChars];        // mảng tạm được copy từ mảng trên dùng để tách chuỗi

// biến lưu giá trị sau khi tách chuỗi
float velocity  = 0; 
float diameter  = 0; //Duong kinh
float delayvel  = 0;// độ rộng xung (us)
float revdis    = 0;
boolean HOME_FLAG;
int state=0;
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
  velocity = atof(strtokIndx);            // convert sang integer

  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 3
  diameter = atof(strtokIndx); 
  
  strtokIndx = strtok(NULL, ",");       // tiếp tục lấy phần thứ 4
  state = atoi(strtokIndx);          // convert sang integer

  // Tính độ rộng xung (us)
  delayvel = (60.0 / (velocity * 3200)) * pow(10, 6); // T = 60 / (rpm * ppr) (giây)

  // Tính xung cho quãng đường cần di chuyển
  revdis =  diameter * 3200 * 2; // 3200 xung nửa chu kỳ -> x2 -> 6400 xung 1cm 
}

void updateParsedData() {
  
}

void returnData() {
  Serial.println("<" + String(receivedChars) + ">");
}
