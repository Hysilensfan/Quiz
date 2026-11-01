#include <iostream>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

void print_flag(string &flag){
    for (auto& character : flag){
            cout << character << flush;   // flush: 直接印出，不管緩衝區，確保字元立刻顯示在螢幕上
            this_thread::sleep_for(chrono::milliseconds(100));  // 停頓時間為 200 毫秒（0.2 秒）
    }
    cout << endl;
    
    return;
}

int main(){
    string data, flag {"Hysilensfan{+h3_3Imqle_trYs!}"};
    
    cout << "Say 'Hello world!', you can get the flag!" << endl;
    getline(cin, data);
    
    if (data == "Hello world!"){  # 字串比較
        print_flag(flag);
    }
    else{
        cout << "Don't you wanna get the flag?";
    }
    
    return 0;
}
