#include <iostream>
#include <string>
#include <cstdlib>
#include "vcpkg_installed/x64-windows/include/websocketpp/client.hpp"


size_t writeResult(char* ptr, size_t size, size_t nmemb, void* userdata) {
	((std::string*)userdata)->append((char*)ptr, size * nmemb);
	return size * nmemb;
}


int main() {
	setlocale(LC_ALL, "Russian");
	std::cout << "Start" << std::endl << std::flush;
	CURL* curl_hndl = curl_easy_init();
	std::string resString;

	if (curl_hndl) {
		curl_easy_setopt(curl_hndl, CURLOPT_URL, "https://habr.com/ru/companies/ruvds/articles/674304/");
		curl_easy_setopt(curl_hndl, CURLOPT_WRITEFUNCTION, writeResult);
		curl_easy_setopt(curl_hndl, CURLOPT_WRITEDATA, &resString);
		CURLcode res = curl_easy_perform(curl_hndl);
		curl_easy_cleanup(curl_hndl);
		std::cout << resString << std::endl;
		std::cout << res;
	}
	std::cout << "End" << std::endl << std::flush;
	return 0;
}