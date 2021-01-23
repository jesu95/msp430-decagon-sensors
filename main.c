#include "decagon.h"

void main(void)
{
	uint8_t data[20];

	WDT_A_hold(WDT_A_BASE);
    Decagon_Init();
	PMM_unlockLPM5();
    __bis_SR_register(GIE);

    while(1)
    {
    	Decagon_Start();
    	Decagon_Stop();
        Decagon_Get_Data(data);
    }
}
