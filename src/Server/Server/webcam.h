#pragma once
#include "lib.h"

using namespace std;

void RecordScreen(int duration);
void OpenCamera();
void CameraStream();
void CloseCamera();
bool CanWriteToDirectory(const string& path);