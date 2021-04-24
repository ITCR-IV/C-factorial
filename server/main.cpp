
#include "mserver.h"
#include "UpdateInfo.h"
#include "iostream"


using namespace std;


int main(int argc, char *argv[])
{
    UpdateInfo* tmp = UpdateInfo::getInstance();
    printf("funciona");
    mserver(9999, 10000);
    return 0;
}
