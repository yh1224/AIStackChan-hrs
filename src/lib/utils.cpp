#include <sstream>
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>

// from http://hardwarefun.com/tutorials/url-encoding-in-arduino
// modified by chaeplin
String urlEncode(const char *msg) {
    const char *hex = "0123456789ABCDEF";
    String encodedMsg = "";

    while (*msg != '\0') {
        if (('a' <= *msg && *msg <= 'z')
            || ('A' <= *msg && *msg <= 'Z')
            || ('0' <= *msg && *msg <= '9')
            || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 0xf];
        }
        msg++;
    }
    return encodedMsg;
}

String jsonEncode(const DynamicJsonDocument &jsonDoc) {
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);
    return jsonStr;
}

std::vector<std::string> splitString(
        const std::string &str, const std::string &delimiter, bool includeDelimiter = false) {
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0, end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        size_t len = end - start;
        if (includeDelimiter) len += delimiter.length();
        token = str.substr(start, len);
        if (!token.empty()) tokens.push_back(token);
        start = end + delimiter.length();
    }
    token = str.substr(start);
    if (!token.empty()) tokens.push_back(token);

    return tokens;
}

std::vector<std::string> splitString(
        const std::string &str, const std::vector<std::string> &delimiters, bool includeDelimiter = false) {
    std::vector<std::string> tokens = {str};

    for (const auto &delimiter: delimiters) {
        std::vector<std::string> temp_tokens;
        for (const auto &token: tokens) {
            std::vector<std::string> split_tokens = splitString(token, delimiter, includeDelimiter);
            temp_tokens.insert(temp_tokens.end(), split_tokens.begin(), split_tokens.end());
        }
        tokens = temp_tokens;
    }

    return tokens;
}

std::vector<std::string> splitLines(const std::string &str) {
    return splitString(str, std::vector<std::string>{"\r", "\n"});
}

std::vector<std::string> splitSentence(const std::string &str) {
    return splitString(str, std::vector<std::string>{"\r", "\n", ".", "。", "?", "？", "!", "！"}, true);
}
