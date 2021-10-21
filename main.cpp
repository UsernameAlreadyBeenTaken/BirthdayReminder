#include <ctime>
#include <string>
#include <windows.h>
#include <fstream>
#include <map>

const int sec_in_day = 86400;

int diff_in_days(std::tm first, std::tm second)
{
    time_t t1 = mktime(&first);
    time_t t2 = mktime(&second);
    time_t diff;
    if (t1 <= t2)
        diff = abs(t1 - t2) / sec_in_day;
    else
        diff = -1;
    return diff;
}

std::wstring s2ws(const std::string& str)
{
    int len;
    int slen = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, buf, len);
    std::wstring result(buf);
    delete[] buf;
    return result;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    std::ifstream file("data.txt");
    if (!file.is_open())
    {
        MessageBox(NULL, (LPCWSTR)L"Data file was not open.", (LPCWSTR)L"Birthday reminder", MB_OK | MB_ICONERROR);
        return 1;
    }

    std::time_t time = std::time(0);
    std::tm* now = new std::tm;
    localtime_s(now, &time);

    std::string data_string;
    std::map<std::string, std::tm> data;
    while (std::getline(file, data_string))
    {
        std::string username    = data_string.substr(0, data_string.find_first_of(' '));
        std::string parced_date = data_string.substr(data_string.find_first_of(' ') + 1);
        std::tm tm = *now;
        tm.tm_mon  = std::stoi(parced_date.substr(0, parced_date.find_first_of('.'))) - 1;
        tm.tm_mday = std::stoi(parced_date.substr(parced_date.find_first_of('.') + 1));
        data.insert(std::make_pair(username, tm));
    }
    file.close();

    std::string message = "Hey, bro!\n";
    for (auto item : data)
    {
        if (diff_in_days(*now, item.second) == 0)
        {
            message += item.first + " have a Birthday today!\n";
        }
        else if (diff_in_days(*now, item.second) == 3)
        {
            message += item.first + " have a Birthday in a 3 days!\n";
        }
        else if (diff_in_days(*now, item.second) == 14)
        {
            message += item.first + " have a Birthday in a 14 days!\n";
        }
        else if (diff_in_days(*now, item.second) == 28)
        {
            message += item.first + " have a Birthday in a 28 days!\n";
        }
    }

    if (message != "Hey, bro!\n")
    {
        std::wstring temp_str = s2ws(message);
        LPCWSTR message_text = temp_str.c_str();
        MessageBox(NULL, message_text, (LPCWSTR)L"Birthday reminder", MB_OK | MB_ICONWARNING);
    }

    delete now;
    return 0;
}