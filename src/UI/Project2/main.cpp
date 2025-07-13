#include "stdafx.h"
#include "receiveemail.h"
#include "screenshot.h"
#include "client.h"

using namespace std;


int main(int argc, _TCHAR* argv[]) {

    //const char* filePath = "screenshot.bmp";
    //cout << "Chup man hinh va luu tai " << filePath << endl;
    //CaptureScreen(filePath);
    //cout << "Hoan thanh!" << endl;
    run(argc, argv);
    client_run();
    return 0;
}
