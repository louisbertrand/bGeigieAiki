#include "web_server.h"
#include "user_config.h"
#include "esp_config.h"
#include "debugger.h"

typedef enum {
  request_line,
  header,
  content
} RequestParse;

ConfigWebServer::ConfigWebServer(EspConfig& config) : server(SERVER_WIFI_PORT, SERVER_MAX_CLIENTS), config(config) {
}

bool ConfigWebServer::initialize() {
  if(!config.get_ap_ssid() || !config.get_ap_password()) {
    debug_println("No SSID or password to start config server");
    return false;
  }

  WiFi.softAP(config.get_ap_ssid(), config.get_ap_password());

  delay(100);

  IPAddress ip(ACCESS_POINT_IP);
  IPAddress n_mask(ACCESS_POINT_NMASK);
  WiFi.softAPConfig(ip, ip, n_mask);

  delay(100);

  server.begin();
  return is_running();
}

void ConfigWebServer::stop() {
  server.stop();
  WiFi.softAPdisconnect(true);
}

void ConfigWebServer::handle_requests() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if(client) {                             // If a new client connects,
    debug_println("New Client.");          // print a message out in the serial port
    HttpRequest request;
    RequestParse state = RequestParse::request_line;

    String currentLine = "";
    while(client.connected()) {
      if(client.available()) {
        char c = static_cast<char>(client.read());
        switch(c) {
          case '\r':
            break;
          case '\n': {
            switch(state) {
              case request_line:
                request.set_request_line(currentLine.c_str());
                state = RequestParse::header;
                currentLine = "";
                break;
              case header:
                if(currentLine != "") {
                  request.add_header_line(currentLine.c_str());
                  currentLine = "";
                } else {
                  state = RequestParse::content;
                }
                break;
              case content:
                break;
            }
            break;
          }
          default:
            currentLine += c;
            break;
        }
      }
      if(state == RequestParse::content) {
        handle_client_request(client, request);
        break;
      }
    }
    // Close the connection
    client.stop();
    debug_println("Client disconnected.");
  }
}

bool ConfigWebServer::is_running() {
  return !!server;
}

void ConfigWebServer::handle_client_request(Stream& client, HttpRequest& request) {
  char transmission[1024];
  if(strcmp(request.get_uri(), "/save") == 0) {
    char value[64];
    if(request.get_param_value("ssid", value, 64)) {
      debug_print("set new use ssid: ");
      debug_print(strlen(value));
      debug_println(value);
      config.set_wifi_ssid(value);
    }
    value[0] = '\0';
    if(request.get_param_value("password", value, 64)) {
      debug_print("set new use password: ");
      debug_println(value);
      config.set_wifi_password(value);
    }
    value[0] = '\0';
    if(request.get_param_value("apikey", value, 64)) {
      debug_print("set new use apikey: ");
      debug_println(value);
      config.set_api_key(value);
    }
    value[0] = '\0';
    if(request.get_param_value("devsrv", value, 64)) {
      debug_print("set new use dev: ");
      debug_println(value);
    }

    client.write(
        "HTTP/1.1 302 Found\r\n"
        "Location: /\r\n\r\n"
    );
  } else {
    sprintf(transmission,
            "HTTP/1.0 200\r\n\r\n"
            "<!DOCTYPE html>\r\n"
            "<html>\r\n"
            "<head>\r\n"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
            "</head>\r\n"
            "<body>\r\n"
            "<form action=\"save\" method=\"get\"> "
            "SSID: <input type=\"text\" name=\"ssid\" value=\"%s\"><br> "
            "PASSWORD: <input type=\"text\" name=\"password\" value=\"%s\"><br> "
            "SAFECAST APIKEY: <input type=\"text\" name=\"apikey\" value=\"%s\"><br>"
            "Use safecast development server:<input name=\"devsrv\" type=\"checkbox\" %s><br>"
            "<input type=\"submit\" value=\"Submit\">"
            "</form><br>\r\n"
            "</body>\r\n"
            "</html>",
            config.get_wifi_ssid(),
            config.get_wifi_password(),
            config.get_api_key(),
            config.get_use_dev() ? "checked" : ""
    );
    client.write(transmission);
  }
}
