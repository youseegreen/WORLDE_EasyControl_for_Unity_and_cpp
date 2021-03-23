# WORLDE EasyControl for Unity, c++
Library for using a Midi Controller (WORLDE, EasyControl) in Unity and c++ programs.

## Requirement
- Windows (64 bit)
- c++, unity

## How to use in Unity
1. Copy ``MidiController.dll`` to ``YourUnityProject\Assets\Plugins\x86_64\`` 
2. Add ``NativePluginMidiController.cs`` to the unity project
3. The controller's states can be use as following
```cs:Test.cs
public class Test : MonoBehaviour
    void Start(){
        NativePluginLibrary.MidiController.OpenMidiController();
        ...
    }
    void Update(){
        bool play_button = NativePluginLibrary.MidiController.Get1SystemButtonState("PLAY");
        ...
    }
}
```

## How to use in c++
1. Add ``MidiControllerAPI.h``, ``MidiControllerAPI.c++`` to your c++ project
2. Replace ``UNITY_MODE`` to ``// UNITY_MODE`` at the first statement of ``MidiControllerAPI.h``
3. The controller's state can be use as following...
```cpp
int main(){
    if (!OpenMidiController()) 
        std::cout << "Midi Controller is not available!" << std::endl;
    float slider_values[9];
    GetSliderValues(slider_values);
    for (int i = 0; i < 9; i++) 
        std::cout << "Slider" << i + 1 << " : " << (slider_values[i] != -1)? slider_values[i] : "undefined" << std::endl;
    CloseMidiController();
    return 1;
}
```

## API
``bool OpenMidiController()``  
引数：なし  
戻値：true : オープン成功、false : オープン失敗  
説明：プログラムの初めに、一回だけ呼び出してください。

``bool CloseMidiController()``  
引数：なし  
戻値：true : クローズ成功、false : クローズ失敗  
説明：MidiControllerを使わなくなった時に、呼び出してください。

``bool GetSliderValues(float slider[9])``  
引数：slider[9]（呼び出し側でメモリ確保しておく必要あり）  
戻値：true : 正常、false : コントローラが利用不可状態  
説明：slider[]にSliderバーの値を0~1で格納します。MIDIの仕様上、バーの値が変化した時のみしか状態を把握できません。状態を把握できない時（初期時）には、-1が入ります。

``bool GetKnobValues(float knob[9])``  
引数：knob[9]（呼び出し側でメモリ確保しておく必要あり）  
戻値：true : 正常、false : コントローラが利用不可状態  
説明：knob[]にKnobの値を0~1で格納します。状態を把握できていない時は-1が入ります。

``bool GetButtonStates(bool button[9])``  
引数：button[9]（呼び出し側でメモリ確保しておく必要あり）  
戻値：true : 正常、false : コントローラが利用不可状態  
説明：button[]にButtonの値をtrue or falseで格納します。
呼び出すまでに一度でもボタンがONになっていると、現在のボタンの状態がOFFであっても、trueが格納されます。

``bool GetSystemButtonStates(bool sys_button[6])``  
引数：sys_button[6]（呼び出し側でメモリ確保しておく必要あり）  
戻値：true : 正常、false : コントローラが利用不可状態  
説明：sys_button[]にSystemButtonの値をtrue or falseで格納します。
呼び出すまでに一度でもボタンがONになっていると、現在のボタンの状態がOFFであっても、trueが格納されます。  
( 0: "PLAY", 1: "")

``float Get1SliderValue(int index)``  
引数：index (0～8)  
戻値：slider[index]の値 (0～1 or -1)

``float Get1KnobValue(int index)``  
引数：index (0～8)  
戻値：knob[index]の値 (0～1 or -1)

``bool Get1ButtonState(int index)``  
引数：index (0～8)  
戻値：button[index]の値 (true or false)  

``bool Get1SystemButtonState(const char* name)``  
引数：name ("PLAY", "STOP", "UPDATE", "REWIND", "FF", "RECORD")  
戻値：system_button[name]の値 (true or false)  

``bool SetButtonTimeInterval(int msec)``  
引数：msec  
戻値：true : セット成功、false : セット失敗  
説明：チャタリング対策。ボタンが物理的にOn⇒Off⇒Onとなる時に、2度目のOnを、ソフト上では押されていないと認識する時間間隔。
``msec=10000``等とすると、一度Onになってから10秒間の間は、何度ボタンを押そうが、プログラム上ではOffとして扱う。デフォルト値は100（0.1秒）。


## Others
Please check ``MidiController.sln``=>``sample.cpp`` to use UDP or graphically display the states of the controller.


## LICENSE
MIT License
