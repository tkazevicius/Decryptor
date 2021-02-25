#include <iostream>
#include <fstream>
using namespace std;
typedef struct {
    char komanda;
    char cons;
}VM;
int main()
{
    char data[256];
    fstream decryptor;
    decryptor.open("decryptor.bin", ios::in | ios::binary);
    for (int i = 0;i < 256;i++) 
    {
        decryptor >> noskipws >> data[i];
        if (decryptor.eof())
            break;
    };
    decryptor.close();
    unsigned char regs[16];
    bool running = true;
    bool flag = false;
    bool eofflag = false;
    int Rx,Ry;
    VM* strukt;
    strukt = (VM*)data;
    ifstream enc("q1_encr.txt");
    ofstream rf("rez.txt");
    while (running)
    {
        Rx = strukt->cons & 0x0F;
        Ry = (strukt->cons & 0xF0) >> 4;
        switch (strukt->komanda) 
        {
        case 0x01:
            regs[Rx] = regs[Rx] + 1;
            break;
        case 0x02:
            regs[Rx] = regs[Rx] - 1;
            break;
        case 0x03:
            regs[Rx] = regs[Ry];
            break;
        case 0x04:
            regs[Rx] = strukt->cons;
            break;
        case 0x05:
            regs[Rx] = regs[Rx] << 1;
            break;
        case 0x06:
            regs[Rx] = regs[Rx] >> 1;
            break;
        case 0x07:
            strukt = (VM*)((char*)strukt + strukt->cons);
            strukt--;
            break;
        case 0x08:
            if (flag == true)
            {
                strukt = (VM*)((char*)strukt + strukt->cons);
                strukt--;
            }
            break;
        case 0x09:
            if (flag == false)
            {
                strukt = (VM*)((char*)strukt + strukt->cons);
                strukt--;
            }
            break;
        case 0x0A:
            if (eofflag == true)
            {
                strukt = (VM*)((char*)strukt + strukt->cons);
                strukt--;
            }
            break;
        case 0x0B:
            enc.close();
            running = false;
            break;
        case 0x0C:
            regs[Rx] = regs[Rx] + regs[Ry];
            break;
        case 0x0D:
            regs[Rx] = regs[Rx] - regs[Ry];
            break;
        case 0x0E:
            regs[Rx] = regs[Rx] ^ regs[Ry];
            break;
        case 0x0F:
            regs[Rx] = regs[Rx] | regs[Ry];
            break;
        case 0x10:
            enc >> regs[Rx];
            if (enc.eof())
                eofflag = true;
            break;
        case 0x11:
            rf << (char)regs[Rx];
            break;
        }
        strukt++;
    }
    rf.close();
}