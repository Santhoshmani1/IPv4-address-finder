#include <curl/curl.h>
#include <iostream>
#include <string>

const char* API_BASEURL = "https://ipinfo.io/";

using namespace std;

// Reads IPv4 address from user and returns the API endpoint
string GetApiEndpoint() {
  string ipv4_address;
  cin >> ipv4_address;

  if ( ipv4_address.length()  < 7 || ipv4_address.length() > 15) {
    cerr << "Invalid IPv4 address! Must be in dotted decimal notation form" << endl;
    return "-1";
  }
  return API_BASEURL + ipv4_address + "/json";
}

// Writes the response to standard output
size_t WriteFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*)ptr, size * nmemb);
  return size * nmemb;
}


int main(int argc, char** argv) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* curl = curl_easy_init();
  if (curl) {
    const string api_endpoint = GetApiEndpoint();
    if (api_endpoint == "-1"){
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }
    cout << "API Endpoint: " << api_endpoint << endl;

    curl_easy_setopt(curl, CURLOPT_URL, api_endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    std::string response_string;
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    } else {
      cout << "Response: " << response_string << endl;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
  } else {
    cerr << "Failed to initialize CURL" << endl;
  }

  return 0;
}
