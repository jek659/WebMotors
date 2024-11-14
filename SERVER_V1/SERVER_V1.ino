#include <WiFi.h>
#include <SPIFFS.h>
#include <FTPServer.h>
#include <ESPmDNS.h>
#include <WebServer.h>

#define W_SSID ""
#define W_PASW ""
#define F_NAME ""
#define F_PASW ""
#define MOTOR1_D 2
#define MOTOR1_S 4
#define MOTOR2_D 16
#define MOTOR2_S 17
#define MOTOR3_D 2
#define MOTOR3_S 4
#define MOTOR4_D 16
#define MOTOR4_S 17
#define MOTOR5_D 2
#define MOTOR5_S 4
#define MOTOR6_D 16
#define MOTOR6_S 17
#define MOTOR7_D 2
#define MOTOR7_S 4
#define MOTOR8_D 16
#define MOTOR8_S 17
FTPServer ftp(SPIFFS);
WebServer srv(80);
class RobotX_Motor {
public:
  RobotX_Motor(byte pD, byte pS) {
    _portD = pD;
    _portS = pS;
    pinMode(_portD, OUTPUT);
    pinMode(_portS, OUTPUT);
  }
  void ndigitalWrite(byte val) {
    digitalWrite(_portD, val);
  }
  void nanalogWrite(byte val) {
    analogWrite(_portS, val);
  }
private:
  byte _portD;
  byte _portS;
};
RobotX_Motor m1 = RobotX_Motor(MOTOR1_S, MOTOR1_D);
RobotX_Motor m2 = RobotX_Motor(MOTOR2_S, MOTOR2_D);
RobotX_Motor m3 = RobotX_Motor(MOTOR3_S, MOTOR3_D);
RobotX_Motor m4 = RobotX_Motor(MOTOR4_S, MOTOR4_D);
RobotX_Motor m5 = RobotX_Motor(MOTOR5_S, MOTOR5_D);
RobotX_Motor m6 = RobotX_Motor(MOTOR6_S, MOTOR6_D);
RobotX_Motor m7 = RobotX_Motor(MOTOR7_S, MOTOR7_D);
RobotX_Motor m8 = RobotX_Motor(MOTOR8_S, MOTOR8_D);
RobotX_Motor motor_base[8] = { m1, m2, m3, m4, m5, m6, m7, m8 };
unsigned long task_start[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
unsigned long task_target[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long del_timer = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(W_SSID, W_PASW);
  delay(10000);
  if (!MDNS.begin("robotxmotors")) {  // Set the hostname to "robotxmotors.local"
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("local  IP: ");
  Serial.println(WiFi.localIP());
  SPIFFS.begin();
  ftp.begin(F_NAME, F_PASW);
  srv.on("/", home_p);
  srv.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  ftp.handleFTP();
  srv.handleClient();
  if (millis() - del_timer > 500) {
    for (byte i = 0; i < 8; i++) {
      if (millis() > task_target[i]) {
        motor_base[i].ndigitalWrite(0);
        motor_base[i].nanalogWrite(0);
      }
    }
    del_timer = millis();
  }
}

void home_p() {
  //pl=2&prt1=2&prt2=3&val1=1&val2=255&t=1000
  if (srv.hasArg("pl") and srv.hasArg("val1") and srv.hasArg("val2") and srv.hasArg("t")) {
    int place = (srv.arg("pl")).toInt();
    int val1 = (srv.arg("val1")).toInt();
    int val2 = (srv.arg("val2")).toInt();
    int t = (srv.arg("t")).toInt();
    switch (place) {
      case 1:
        motor_base[0].ndigitalWrite(val1);
        motor_base[0].nanalogWrite(val2);
        task_target[0] = millis() + t;
        get_file_content_server_send("/module1.html");
        break;
      case 2:
        motor_base[1].ndigitalWrite(val1);
        motor_base[1].nanalogWrite(val2);
        task_target[1] = millis() + t;
        get_file_content_server_send("/module2.html");
        break;
      case 3:
        motor_base[2].ndigitalWrite(val1);
        motor_base[2].nanalogWrite(val2);
        task_target[2] = millis() + t;
        get_file_content_server_send("/module3.html");
        break;
      case 4:
        motor_base[3].ndigitalWrite(val1);
        motor_base[3].nanalogWrite(val2);
        task_target[3] = millis() + t;
        get_file_content_server_send("/module4.html");
        break;
      case 5:
        motor_base[4].ndigitalWrite(val1);
        motor_base[4].nanalogWrite(val2);
        task_target[4] = millis() + t;
        get_file_content_server_send("/module5.html");
        break;
      case 6:
        motor_base[5].ndigitalWrite(val1);
        motor_base[5].nanalogWrite(val2);
        task_target[5] = millis() + t;
        get_file_content_server_send("/module6.html");
        break;
      case 7:
        motor_base[6].ndigitalWrite(val1);
        motor_base[6].nanalogWrite(val2);
        task_target[6] = millis() + t;
        get_file_content_server_send("/module7.html");
        break;
      case 8:
        motor_base[8].ndigitalWrite(val1);
        motor_base[8].nanalogWrite(val2);
        task_target[8] = millis() + t;
        get_file_content_server_send("/module8.html");
        break;
    }
  } else if (srv.hasArg("ports")) {
    String port = srv.arg("ports");
    if (port == "p1") {
      get_file_content_server_send("/module1.html");
    } else if (port == "p2") {
      get_file_content_server_send("/module2.html");
    } else if (port == "p3") {
      get_file_content_server_send("/module3.html");
    } else if (port == "p4") {
      get_file_content_server_send("/module4.html");
    } else if (port == "p5") {
      get_file_content_server_send("/module5.html");
    } else if (port == "p6") {
      get_file_content_server_send("/module6.html");
    } else if (port == "p7") {
      get_file_content_server_send("/module7.html");
    } else if (port == "p8") {
      get_file_content_server_send("/module8.html");
    }

  } else {
    get_file_content_server_send("/index.html");
  }
}
void get_file_content_server_send(String f_name) {
  if (SPIFFS.exists(f_name)) {
    File f_site = SPIFFS.open(f_name, "r");
    String page = f_site.readString();
    f_site.close();
    srv.send(200, "text/html", page);
  } else {
    srv.send(404, "text/plain", "File not found");
  }
}
