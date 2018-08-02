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
    char pad_0x00AC[0x50]; //0x00AC
    __int32 health; //0x00FC
    __int32 crouch; //0x0100
    char pad_0x0104[0x2B0]; //0x0104
    Vector2 aimCoords; //0x03B4
    char pad_0x03BC[0x484]; //0x03BC

}; //Size=0x0840


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
uintptr_t* add = (uintptr_t *) 0x55A86A74;

uintptr_t* aimY = (uintptr_t*)addressFinder(add,{ aimYOffs });

uintptr_t* aimX = (uintptr_t*)addressFinder(add,{ aimXOffs });

soldier* Aimbot(soldier *player, std::vector<soldier *> ents){
    float fovAllow = 20;
    float distAllow = 20;
    uintptr_t smoothNum = 10000;//higher is slower
    soldier* closest = ents[1];
    soldier* enemy;
    float minDist = 9999999.0;
    float enDist = minDist;
    float angleX, angleY, cloAngleX, cloAngleY = 0.0;
    uintptr_t numOfPlayers = ents.size();
    bool found = false;

    for (int i = 0; i < (numOfPlayers - 1); i++)
    {
//        if (IsValidEnt(ents.at(i))) {//won't go in yet because isvalid should always be returning false

            enemy = ents[i];

            if (enemy->health > 0 && enemy->health <= 100){//check if alive
////                if(enemy->team != player->team && (enemy->team == 0 || enemy->team == 1)) {
                    enDist = distance3D(player->position.x, player->position.y, player->position.z, enemy->position.x, enemy->position.y, enemy->position.z);
//                    angleX = (-(float) atan2(enemy->position.x - player->position.x, enemy->position.y - player->position.y)) / 3.14159265358979323846 * 180.0f + 180.0f;
//                    angleY = (atan2(enemy->position.z - player->position.z, enDist)) * 180.0f / 3.14159265358979323846;
//
////                angleX = (-(float) atan2(enemy->head.x - player->head.x, enemy->head.y - player->head.y)) / 3.14159265358979323846 * 180.0f + 180.0f;
////                angleY = (atan2(enemy->head.z - player->head.z, enDist)) * 180.0f / 3.14159265358979323846;
//
//                    if (!(abs(angleX - player->aimCoords.x) > fovAllow || abs(angleY - player->aimCoords.y) > fovAllow)) {
                        if (enDist < minDist) {
                            closest = enemy;
                            minDist = enDist;
//                            system("CLS");
//                            std::cout << "\nhealth of closest" << ": " << enemy->health << "\n";
//                            std::cout << enemy->position.x << "\n";
//                            std::cout << enemy->position.y << "\n";
//                            std::cout << enemy->position.z << "\n";
//                            std::cout << minDist << "\n";
                            found = true;
//                            cloAngleX = angleX;
//                            cloAngleY = angleY;
                        }
                    }
//            }
    }
//        }

    if (found) {

        system("CLS");
        std::cout << "health of closest" << ": " << closest->health << "\n";
        std::cout << closest->position.x << "\n";
        std::cout << closest->position.y << "\n";
        std::cout << closest->position.z << "\n";
        std::cout << minDist << "\n"; //this might be the trouble spot


//        cloAngleX = player->aimCoords.x + (cloAngleX - player->aimCoords.x) / smoothNum;
//        player->aimCoords.x = cloAngleX;
//
//        cloAngleY = player->aimCoords.y + (cloAngleY - player->aimCoords.y) / smoothNum;
//        player->aimCoords.y = cloAngleY;

//
//        *aimY = cloAngleY;
//        *aimX = cloAngleX;
//        std::cout << "start" << "\n";
//        std::cout << "cloY: " << cloAngleY << "mine: " << *(float*)aimY << "\n";
//        std::cout << "cloX: " << cloAngleX << "mine: " << *(float*)aimX << "\n";



    }
    return player;
}

DWORD dwClient	= (DWORD)GetModuleHandle("client.dll");
uintptr_t baseEntity = dwClient + 0x04A923f4;
uintptr_t baseAdd = dwClient + 0x00AB6D9C;
soldier * localPlayer = *(soldier**)baseAdd;
std::vector<soldier *> ents;

//26552404
//22576D9C

// aimY 0x55A86A74 dereferenced the offset with 0x4D10
// aimX is above but with 0x4D14


DWORD __stdcall hackthread(void* param)
{
    uintptr_t numPlayers = 10;
    FILE *pFile = nullptr;
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "                Kip's CSGO internal aimbot\n";
    std::cout << "          right click or left alt activates aimbot\n";
    std::cout << "                         f3 to quit\n";
    std::cout << "----------------------------------------------------------------------\n";

    for(uintptr_t i = 1; i < numPlayers; i++) {
        ents.push_back(*(soldier **) (baseEntity + (0x10 * i))); //this is the player
    }
    ents.resize(numPlayers);

    while (!GetAsyncKeyState(VK_F3))
    {
        if (GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_LCONTROL)) {

            Aimbot(localPlayer, ents);
//            system("CLS");
//            std::cout << localPlayer->health << "\n";
//            std::cout << localPlayer->position.x << "\n";
//            std::cout << localPlayer->position.y << "\n";
//            std::cout << localPlayer->position.z << "\n";
//            std::cout << "0 " << localPlayer->health << "\n";
            for(uintptr_t  i = 1; i < numPlayers - 1; i++){
//                soldier * current =  *(soldier **) (baseEntity + (0x10 * i));
//                std::cout << i << " " << current->health << "\n";

//                std::cout << i << " " << ents[i]->health<< "\n";
            }



//            if (aimY != 0) {        // safe to dereference
//                *aimY = 20;
//                *aimX = 20;
//                std::cout << "start" << "\n";
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