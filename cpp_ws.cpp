#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <codecvt>

#pragma comment(lib, "libcurl.lib")

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using json = nlohmann::json;

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

std::wstring responseMessage;

size_t writeResult(char* ptr, size_t size, size_t nmemb, void* userdata) {
    ((std::string*)userdata)->append((char*)ptr, size * nmemb);
 
    return size * nmemb;
}


void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    responseMessage = std::wstring(msg->get_payload().begin(), msg->get_payload().end());

    websocketpp::lib::error_code ec;
    if (ec) {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }
    c->close(hdl, 100, "message recieved");
}

int send_cmd(int cmndN, std::string debugURL, std::string browserID) {
    json commandJson = {
        {"id", 1},
        {"method", "DOM.getDocument"},
        {"params", {
            {"depth", -1}
        }}
    };
    std::string command = commandJson.dump();
    json commandJson2 = {
        {"id", 1},
        {"method", "Browser.close"},
        {"params", {
        }}
    };
    std::string command2 = commandJson2.dump();
    client c;
    try {
        c.init_asio();
        c.start_perpetual();

        c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection((cmndN == 0 ? debugURL : browserID), ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 1;
        }
        c.connect(con);
        while (con->get_state() != websocketpp::session::state::open) {
            c.run_one();
        }
        c.send(con, (cmndN == 0 ? command : command2), websocketpp::frame::opcode::text, ec);

        if (ec) {
            std::cout << "Error sending message: " << ec.message() << std::endl;
            c.close(con, 400, "error");
        }
        c.run();
        std::thread websocketThread([&c]() {
            try {
                c.run();
            }
            catch (websocketpp::exception const& e) {
                std::cout << e.what() << std::endl;
            }
            });
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

std::wstring getDOMfromPage(std::string url, std::string port) {
    std::wstring res;
    CURL* curl_hndl0 = curl_easy_init();
    std::string resString0;
    std::string path = "http://127.0.0.1:";
    path.append(port);
    path.append("/json/version");
    if (curl_hndl0) {
        curl_easy_setopt(curl_hndl0, CURLOPT_URL, path.c_str());
        curl_easy_setopt(curl_hndl0, CURLOPT_WRITEFUNCTION, writeResult);
        curl_easy_setopt(curl_hndl0, CURLOPT_WRITEDATA, &resString0);
        CURLcode res = curl_easy_perform(curl_hndl0);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        curl_easy_cleanup(curl_hndl0);
        std::cout << res;
    }

    json response0 = json::parse(resString0);

    std::string browserID = response0["webSocketDebuggerUrl"];

    CURL* curl_hndl = curl_easy_init();
    std::string resString;
    std::string path2 = "http://127.0.0.1:";
    path2.append(port);
    path2.append("/json/new?");
    path2.append(url);
    if (curl_hndl) {
        curl_easy_setopt(curl_hndl, CURLOPT_URL, path2.c_str());
        curl_easy_setopt(curl_hndl, CURLOPT_WRITEFUNCTION, writeResult);
        curl_easy_setopt(curl_hndl, CURLOPT_WRITEDATA, &resString);
        curl_easy_setopt(curl_hndl, CURLOPT_CUSTOMREQUEST, "PUT");
        CURLcode res = curl_easy_perform(curl_hndl);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        curl_easy_cleanup(curl_hndl);
        std::cout << res;
    }
    json response = json::parse(resString);

    std::string webSocketDebuggerUrl = response["webSocketDebuggerUrl"];

    for (int i = 0; i < 2; i++) {
        send_cmd(i, webSocketDebuggerUrl, browserID);
        if (i == 0) res.append(responseMessage);
    }

    return res;
}

void convertCyrillicToReadable(nlohmann::json& json) {
    for (auto& element : json.items()) {
        if (element.value().is_string()) {
            std::string value = element.value();
            bool containsCyrillic = false;
            for (char c : value) {
                if (c >= 0x400 && c <= 0x4FF) { // Range for Cyrillic characters in Unicode
                    containsCyrillic = true;
                    break;
                }
            }
            if (containsCyrillic) {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                std::wstring wideString = converter.from_bytes(value);
                element.value() = wideString;
            }
        }
        else if (element.value().is_object() || element.value().is_array()) {
            convertCyrillicToReadable(element.value());
        }
    }
}


int main(int argc, char *argv[]) {
    int res = 1;
    if (argc == 3 || argc == 4) {
        std::string port = argv[1];
        std::string url = argv[2];
        int alt = std::atoi(argv[3]);
        std::string cmd = "start chrome --headless --disable-gpu --remote-debugging-port=";
        std::string alt_cmd = "start chrome --remote-debugging-port=";
        (alt == 0) ? cmd.append(port) : alt_cmd.append(port);
        system((alt == 0) ? cmd.c_str() : alt_cmd.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::wstring nString = getDOMfromPage(url, port);

        nlohmann::json Json = nlohmann::json::parse(nString);
        convertCyrillicToReadable(Json);

        std::string path = (std::string)argv[0];
        auto indx = path.find_last_of('\\');
        path.erase(indx);
        std::ofstream outputFile(path + "\\output.txt", std::ios::out | std::ios::binary);
        outputFile.write(Json.dump().c_str(), Json.dump().size());
        outputFile.close();

        res = 0;
    } else {
        std::cout << "Not enough arguments: ws_bridge.exe {port} {url}" << std::endl;
        res = 2;
    }
    return res;
}