#include "library.h"
#include <windows.h>
#include <iostream>
#include<vector>
// Generated using ReClass 2016


uintptr_t addressFinder(uintptr_t* base, uintptr_t offsets[]){
    int num_elements = sizeof( offsets ) / sizeof( offsets[0] );
    uintptr_t address = *base;
    for(int i = 0; i < num_elements; i++){
        address = (address + offsets[i]);
    }
    return address;
}

struct Vector3
{
    float x, y, z;
};

struct Vector2
{
    float x, y;
};
// Generated using ReClass 2016

class soldier;

class soldier
{
public:
    char pad_0x0000[0xA0]; //0x0000
    Vector3 position; //0x00A0
    char pad_0x00AC[0x44]; //0x00AC
    __int32 team; //0x00F0
    __int32 team2; //0x00F4
    char pad_0x00F8[0x4]; //0x00F8
    __int32 health; //0x00FC
    __int32 crouch; //0x0100
    char pad_0x0104[0x2B0]; //0x0104
    Vector2 aimCoords; //0x03B4
    char pad_0x03BC[0x5C0]; //0x03BC
    __int32 visible; //0x097C

}; //Size=0x0980



//class soldier
//{
//public:
//    char pad_0x0000[0xA0]; //0x0000
//    Vector3 position; //0x00A0
//    char pad_0x00AC[0x44]; //0x00AC
//    __int32 team; //0x00F0
//    __int32 team2; //0x00F4
//    char pad_0x00F8[0x4]; //0x00F8
//    __int32 health; //0x00FC
//    __int32 crouch; //0x0100
//    char pad_0x0104[0x2B0]; //0x0104
//    Vector2 aimCoords; //0x03B4
//    char pad_0x03BC[0x484]; //0x03BC
//
//}; //Size=0x0840


bool IsValidEnt(soldier* ent)
{
//    if (ent)
//    {
//        if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0)//not 100% sure what this is doing
//        {
//            return true;
//        }
//    }
    return false;
}

float distance3D(float pX, float pY, float pZ, float eX, float eY, float eZ) {
    return sqrt(pow(pX - eX, 2.0) + pow(pY - eY, 2.0) + pow(pZ - eZ, 2.0));
}

uintptr_t aimYOffs[] = {0x4D10};
uintptr_t aimXOffs[] = {0x4D14};

DWORD dwEngine	= (DWORD)GetModuleHandle("engine.dll");
DWORD dwClient	= (DWORD)GetModuleHandle("client_panorama.dll");
DWORD dwServer	= (DWORD)GetModuleHandle("server.dll");

uintptr_t* add = (uintptr_t *) (dwEngine + 0x00586A74);
uintptr_t* aimY = (uintptr_t*)addressFinder(add,{ aimYOffs });
uintptr_t* aimX = (uintptr_t*)addressFinder(add,{ aimXOffs });
uintptr_t numOfPlayersBaseAdd = dwServer+0x9E48D0;
uintptr_t* numOfPlayersAdd = (uintptr_t*) numOfPlayersBaseAdd;
uintptr_t numPlayers = *numOfPlayersAdd;




soldier* Aimbot(soldier *player, std::vector<soldier *> ents){
//    config
    float distAllow = 20;
    float fovAllow = 20;
    uintptr_t smoothNum = 10000;//higher is slower
//    ====================

//    range tracking
    float rangeBot0 = 1;
    float rangeBot1 = 1;
    float rangeTop0 = 1;
    float rangeTop1 = 1;
    bool inFovX = false;
    bool inFovY = false;
//    ====================

    float angleX = 100;
    float angleY = 1;
    float cloAngleX, cloAngleY = 0.0;

    bool found = false;

    soldier* closest = ents[1];
    soldier* enemy;
    float minDist = 9999999.0;
    float enDist = minDist;

    numPlayers =  *numOfPlayersAdd;



//    system("CLS");
//    std::cout << "mY: " << *(float*)aimY << "\n";
//    std::cout << "mX: " << *(float*)aimX << "\n";
//    std::cout << "aY: " << cloAngleY  << "\n";
//    std::cout << "aX: " << cloAngleX  << "\n";

    for (int i = 1; i < numPlayers; i++)
    {
//        if (IsValidEnt(ents.at(i))) {//won't go in yet because isvalid should always be returning false
        boolean inFov;



        enemy = ents[i];
        if(enemy->visible != 0 ) {
            if (enemy->health > 0 && enemy->health <= 100) {//check if alive
                if (enemy->team != player->team) {
                    enDist = distance3D(player->position.x, player->position.y, player->position.z,
                                        enemy->position.x, enemy->position.y, enemy->position.z);

                    Vector3 Pos;

                    Pos.x = enemy->position.x - player->position.x;
                    Pos.y = enemy->position.y - player->position.y;
                    Pos.z = enemy->position.z - player->position.z;

                    float Magnitude = sqrt(Pos.x * Pos.x + Pos.y * Pos.y + Pos.z * Pos.z);

                    angleX = (float) atan2(Pos.y, Pos.x) * 180.0 / 3.14159265358979323846;
                    angleY = -1 * (atan2(Pos.z, Magnitude) * 180.0 / 3.14159265358979323846);

                    rangeBot0 = *(float*)aimX - fovAllow;
                    rangeBot1 = *(float*)aimX;
                    rangeTop0 = *(float*)aimX;
                    rangeTop1 = *(float*)aimX + fovAllow;


                    if((*(float*)aimX <= rangeTop1 && *(float*)aimX >= rangeTop0 ) || (*(float*)aimX <= rangeBot1 && *(float*)aimX >= rangeBot0 )) { inFovX = true;}


                    if(rangeTop1 > 180){
                        rangeTop1 = -180 + (rangeTop1 - 180);
                        rangeTop0 = -180;
                        if((*(float*)aimX <= rangeTop0 && *(float*)aimX >= rangeTop1 )){ inFovX = true;}
                    }

                    if(rangeBot0 < -180){
                        rangeBot0 = 180 + (rangeTop1 + 180);
                        rangeBot1 = 0;
                        if((*(float*)aimX <= rangeBot0 && *(float*)aimX >= rangeBot1 )) {inFovX = true;}
                    }

//                    if (!(abs(angleX - player->aimCoords.x) > fovAllow ||
//                          abs(angleY - player->aimCoords.y) > fovAllow)) {
                    if(inFovX) {
                        if (enDist < minDist) {
                            closest = enemy;
                            minDist = enDist;

                            found = true;
                            cloAngleX = angleX;
                            cloAngleY = angleY;


                        }
                    }
                }
            }
        }
        numPlayers = *numOfPlayersAdd - 1;//dwServer+0x9E48E0;//I know you probably don't need two, but honestly, fuck it
    }
//        }

    if (found) {

//        cloAngleX = player->aimCoords.x + (cloAngleX - player->aimCoords.x) / smoothNum;
//        player->aimCoords.x = cloAngleX;

//
//        cloAngleY = player->aimCoords.y + (cloAngleY - player->aimCoords.y) / smoothNum;
//        player->aimCoords.y = cloAngleY;

        *(float*)aimX = cloAngleX;
        *(float*)aimY = cloAngleY;
//
//        system("CLS");
//        std::cout << "mY: " << *(float*)aimY << "\n";
//        std::cout << "mX: " << *(float*)aimX << "\n";
//        std::cout << "aY: " << cloAngleY  << "\n";
//        std::cout << "aX: " << cloAngleX  << "\n";
//        std::cout << "rBot: " << rangeBottom0 << " <-> " << rangeBottom1 << "\n";
//        std::cout << "rTop: " << rangeTop0 << " <-> " << rangeTop1 << "\n";



    }
    return player;
}

uintptr_t baseEntity = dwClient + 0x04C380EC - 0x10;//0x04A923f4;
uintptr_t baseAdd = baseEntity;//dwClient + 0x04C380EC - 0x10; //0x00AB6D9C;
soldier * localPlayer = *(soldier**)baseAdd;
std::vector<soldier *> ents;



DWORD __stdcall hackthread(void* param)
{
    uintptr_t numPlayers =  *numOfPlayersAdd;
    FILE *pFile = nullptr;
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "                Kip's CSGO internal aimbot\n";
    std::cout << "            mouse 5 or left alt activates aimbot\n";
    std::cout << "                         f3 to quit\n";
    std::cout << "----------------------------------------------------------------------\n";
//    uintptr_t* testVar = (uintptr_t*) dwServer+0x9E48E0;
//    std::cout << numPlayers << "\n";
//    std::cout << std::hex << numOfPlayersAdd << "\n";
//    std::cout << std::hex << numOfPlayersBaseAdd << "\n";
//    std::cout << *testVar << "\n";
//    std::cout << dwServer << "\n";
//    std::cout << 29460000 << "\n";

    numPlayers = *numOfPlayersAdd;

    for(uintptr_t i = 1; i < numPlayers; i++) {
        ents.push_back(*(soldier **) (baseEntity + (0x10 * i))); //this is the player
    }
    ents.resize(numPlayers);

    while (!GetAsyncKeyState(VK_F3))
    {
        if (GetAsyncKeyState(VK_XBUTTON1) || GetAsyncKeyState(VK_LCONTROL)) {

            Aimbot(localPlayer, ents);

//            if (aimY != 0) {        // safe to dereference
//                *aimY = 20;
//                *aimX = 20;
//                std::cout << "start" << "\n";
//                std::cout << localPlayer->health << "\n";
//                std::cout << *(float*)aimY << "\n";
//                std::cout << *(float*)aimX << "\n";
//
//            }
        }
//        Sleep(10);
    }
//    system ("CLS");
    std::cout << "good to exit" << std::endl;
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)param, NULL);
    return NULL;
}


BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, hackthread, hModule, 0, 0);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}