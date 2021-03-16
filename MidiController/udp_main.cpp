//TCP,UDP通信用ヘッダ
#include <winsock2.h> //windows
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h> 
#include <mmsystem.h> 
//#include "OpenCV.h"
#include <string>
#include <map>
#pragma comment(lib, "winmm.lib")

//TCP,UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")


// inet_addr()関数で警告が出る場合は以下で警告を無効化する。
#pragma warning(disable:4996) 


// midi controller information
#define CONTROLLER_NAME "WORLDE easy control"

#define SLIDER_BIAS 3
#define SLIDER_NUM 9
#define KNOB_BIAS 14
#define KNOB_NUM 9
#define BUTTON_BIAS 23
#define BUTTON_NUM 9
#define SYSTEM_BUTTON_BIAS 44
#define SYSTEM_BUTTON_NUM 6

#define OTHER_BUTTON_NUM 4



class MidiController {
private:
    int button_on_interval = 100;

    std::map<std::string, int> system_buttons_map
        = { {"RECORD", 0}, {"PLAY", 1}, {"STOP", 2},
            {"REWIND", 3}, {"FORWARD", 4}, {"UPDATE", 5} };

    float sliders[SLIDER_NUM] = { -1 };
    float knobs[KNOB_NUM] = { -1 };

    float buttons[BUTTON_NUM] = { -1 };
    bool app_buttons[BUTTON_NUM] = { false };
    unsigned int buttons_lastontimes[BUTTON_NUM] = { 0 };

    float system_buttons[SYSTEM_BUTTON_NUM] = { -1 };
    bool app_system_buttons[SYSTEM_BUTTON_NUM] = { false };
    unsigned int system_buttons_lastontimes[SYSTEM_BUTTON_NUM] = { 0 };

public:

    MidiController() {
        Initializer();
    }

    void Initializer() {
        for (int i = 0; i < SLIDER_NUM; i++) sliders[i] = -1;
        for (int i = 0; i < KNOB_NUM; i++) knobs[i] = -1;
        for (int i = 0; i < BUTTON_NUM; i++) {
            buttons[i] = -1;
            app_buttons[i] = false;
            buttons_lastontimes[i] = 0;
        }
        for (int i = 0; i < SYSTEM_BUTTON_NUM; i++)
        {
            system_buttons[i] = -1;
            app_system_buttons[i] = false;
            system_buttons_lastontimes[i] = 0;
        }
    }

    void DecodeMidiEvent(unsigned char name, unsigned char value, unsigned int time) {
        // slider
        if (name >= SLIDER_BIAS && name < (SLIDER_BIAS + SLIDER_NUM)) {
            sliders[name - SLIDER_BIAS] = value / 127.0;
        }

        // knob
        else if (name >= KNOB_BIAS && name < (KNOB_BIAS + KNOB_NUM)) {
            knobs[name - KNOB_BIAS] = value / 127.0;
        }

        // button
        else if (name >= BUTTON_BIAS && name < (BUTTON_BIAS + BUTTON_NUM)) {
            int idx = name - BUTTON_BIAS;
            buttons[idx] = value / 127.0;
            if (value == 127) {
                if (time - buttons_lastontimes[idx] > button_on_interval) {
                    app_buttons[idx] = true;
                    buttons_lastontimes[idx] = time;
                }
                //else {
                //    printf("chattering発生?\n");
                //}
            }
        }

        // system_button
        else if (name >= SYSTEM_BUTTON_BIAS && name < (SYSTEM_BUTTON_BIAS + SYSTEM_BUTTON_NUM)) {
            int idx = name - SYSTEM_BUTTON_BIAS;
            system_buttons[idx] = value / 127.0;
            if (value == 127) {
                if (time - system_buttons_lastontimes[idx] > button_on_interval) {
                    app_system_buttons[idx] = true;
                    system_buttons_lastontimes[idx] = time;
                }
                //else {
                //    printf("chattering発生?\n");
                //}
            }
        }

        else {
            printf("this program does not handle the buttons\nbutton id = %u, button_value = %u\n", name, value);
        }
    }

    bool GetSliderValue(float* out_array, int first_idx = -1, int last_idx = -1) {
        if (first_idx == -1 && last_idx == -1) {
            // 全てのスライダーの情報を提供する
            first_idx = 0;
            last_idx = SLIDER_NUM;
        }
        for (int i = first_idx; i < last_idx; i++) out_array[i] = sliders[i];
        return true;
    }
    bool GetKnobValue(float* out_array, int first_idx = -1, int last_idx = -1) {
        if (first_idx == -1 && last_idx == -1) {
            // 全てのノブの情報を提供する
            first_idx = 0;
            last_idx = KNOB_NUM;
        }
        for (int i = first_idx; i < last_idx; i++) out_array[i] = knobs[i];
        return true;
    }
    bool GetButtonState(int* out_array, int first_idx = -1, int last_idx = -1) {
        if (first_idx == -1 && last_idx == -1) {
            // 全てのボタンの情報を提供する
            first_idx = 0;
            last_idx = BUTTON_NUM;
        }
        for (int i = first_idx; i < last_idx; i++) {
            out_array[i] = (app_buttons[i]) ? 1 : 0;
            app_buttons[i] = false;
        }
        return true;
    }
    bool GetSystemButtonState(int* out_array, int first_idx = -1, int last_idx = -1) {
        if (first_idx == -1 && last_idx == -1) {
            // 全てのシステムボタンの情報を提供する
            first_idx = 0;
            last_idx = SYSTEM_BUTTON_NUM;
        }
        for (int i = first_idx; i < last_idx; i++) {
            out_array[i] = (app_system_buttons[i]) ? 1 : 0;
            app_system_buttons[i] = false;
        }
        return true;
    }
    bool GetSystemButtonState(int* out_array, const char* name) {
        if (system_buttons_map.count(name)) {
            int idx = system_buttons_map[name];
            *out_array = (app_system_buttons[idx]) ? 1 : 0;
            app_system_buttons[idx] = false;
            return true;
        }
        return false;
    }
    bool SetButtonOnInterval(int msec) {
        button_on_interval = msec;
        return true;
    }

    //// OpenCVを使えるのであれば、下の関数で可視化できる
    //    void ShowSlider() {
    //    using namespace cv;
    //    int width = 100;
    //    int height = 500;
    //    Mat img = Mat(Size(width * SLIDER_NUM, height), CV_8UC3, Scalar(255, 255, 255));
    //    for (int i = 0; i < SLIDER_NUM; i++) {
    //        rectangle(img, Rect(i * width + 2, (int)(height - sliders[i] * (height - 1) - 1), width - 2, sliders[i] * (height - 1)), Scalar(0, 0, 255), -1);
    //    }
    //    imshow("midi slider", img);
    //    waitKey(1);
    //}
};

class UDP {
    WSAData wsaData;
    SOCKET sock;
    struct sockaddr_in addr;

public:
    UDP(std::string address = "100.80.145.173", int port = 52525) {
        WSAStartup(MAKEWORD(2, 0), &wsaData);
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.S_un.S_addr = inet_addr(address.c_str());
    }
    void send(std::string msg) {
        sendto(sock, msg.c_str(), msg.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
    ~UDP() {
        closesocket(sock);
        WSACleanup();
    }
};



MidiController* midi_controller;
HMIDIIN midi_in_handle;
UDP* udp;

void CALLBACK MidiInProc(
    HMIDIIN midi_in_handle,
    UINT wMsg,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2
)
{
    switch (wMsg)
    {
    case MIM_OPEN:
        printf("MIDI device was opened\n");
        break;
    case MIM_CLOSE:
        printf("MIDI device was closed\n");
        break;
    case MIM_DATA:
    {
        unsigned char status_byte = (dwParam1 & 0x000000ff);
        if (status_byte != 0xb0) break;

        unsigned char name = (dwParam1 & 0x0000ff00) >> 8;
        unsigned char value = (dwParam1 & 0x00ff0000) >> 16;
        unsigned int time = dwParam2;
        midi_controller->DecodeMidiEvent(name, value, time);
    }
    break;
    case MIM_LONGDATA:
    case MIM_ERROR:
    case MIM_LONGERROR:
    case MIM_MOREDATA:
    default:
        break;
    }
}


bool OpenController() {

    unsigned int target_dev_id = -1;
    unsigned int num_devices = midiInGetNumDevs();

    MMRESULT result;
    MIDIINCAPS midi_in_caps;
    char device_name_buff[32];

    for (unsigned int dev_id = 0; dev_id < num_devices; ++dev_id)
    {
        UINT cbMidiInCaps;
        result = midiInGetDevCaps(
            dev_id,
            &midi_in_caps,
            sizeof(midi_in_caps)
        );
        if (result != MMSYSERR_NOERROR) { continue; }

        errno_t error = wcstombs_s(
            NULL, device_name_buff, 32,
            midi_in_caps.szPname, sizeof(midi_in_caps.szPname));
        if (error != 0) { continue; }

        if (strcmp(device_name_buff, CONTROLLER_NAME) == 0) {
            target_dev_id = dev_id;
        }
    }

    if (target_dev_id == -1) {
        printf("\nDevice is not connected!!!\n");
        return false;
    }

    MMRESULT res;
    WCHAR errmsg[MAXERRORLENGTH];
    char errmsg_buff[MAXERRORLENGTH];

    res = midiInOpen(&midi_in_handle, target_dev_id, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
    if (res != MMSYSERR_NOERROR) {
        printf("Cannot open MIDI input device %u", target_dev_id);
        return false;
    }

    //    printf("Successfully opened a MIDI input device %u.\n", target_dev_id);
    return true;
}




int main(int argc, char** argv)
{
    if (!OpenController()) {
        return -1;
    }

    midi_controller = new MidiController();
    udp = new UDP("100.80.145.91", 52525);

    midiInStart(midi_in_handle);


    while (true) {

        // ここに処理を書く

        // データ受信部
        float slider[SLIDER_NUM];
        int play;
        midi_controller->GetSliderValue(slider);
        midi_controller->GetSystemButtonState(&play);

        // プレイボタンが押されていたらudpを送る
        if (play) {

            // 送信部
            std::string msg = "";
            msg += "p,9";
            for (int i = 0; i < SLIDER_NUM; i++) {
                msg += ",";
                msg += slider[i];
            }
            udp->send(msg);
        }

    }

    midiInStop(midi_in_handle);
    midiInReset(midi_in_handle);
    midiInClose(midi_in_handle);

    delete midi_controller;
    delete udp;

    return 0;
}