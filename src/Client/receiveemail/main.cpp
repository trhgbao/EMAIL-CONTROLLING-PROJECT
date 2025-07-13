#include "stdafx.h"
#include "receiveemail.h"
#include "screenshot.h"
#include "client.h"
#include "Take_Del.h"

using namespace std;


int main(int argc, _TCHAR* argv[]) {
    setup_console();
    run(argc, argv);
    if (!client_run()) {
        return 0;
    }
    sendEmail();
    return 0;
}
