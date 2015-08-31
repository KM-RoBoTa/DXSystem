#ifndef ADDITIONALTOOLS_H
#define ADDITIONALTOOLS_H

#include <string>

using namespace std;

void delay(int msec);
string getTimeStr();
int str2int(const string& s);
int getInt();
bool getYorN();
void handleError(const string& msg, bool throwFlag);

#endif // ADDITIONALTOOLS_H
