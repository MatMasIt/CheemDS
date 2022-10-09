#include <nds.h>
#include <maxmod9.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "cheems_base.h"
#include "cheems_hit.h"
#include "soundbank.h"
#include "soundbank_bin.h"

int counter = 0;
int bg3 = 0;
int randIndex = 0;
char t[100];

void clear()
{
	iprintf("\x1b[2J");
}

void updateCounter()
{
	clear();
	sprintf(t, "\n\n\tHit cheems %d times", counter);
	iprintf(t);
}

int random(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void Ibase()
{
	dmaCopy(cheems_baseBitmap, bgGetGfxPtr(bg3), 256 * 256);
	dmaCopy(cheems_basePal, BG_PALETTE, 256 * 2);
}

void IHit()
{
	dmaCopy(cheems_hitBitmap, bgGetGfxPtr(bg3), 256 * 256);
	dmaCopy(cheems_hitPal, BG_PALETTE, 256 * 2);
}

void footer()
{
	iprintf("\n\n\n\n\n\n\n (C) 2022, Mattia Mascarello\n github -> MatMasIt/CheemDS");
}

void IHitMessage()
{
	char messages[12][100] = {
		"\"Ouch\"",
		"\"Loadming\"",
		"\"Flamethwomer\"",
		"\"Sciemence\"",
		"\"No bonk pwease\"",
		"\"So scare, concern\"",
		"\"Much bonk\"",
		"\"Go to horny jail\"",
		"\"Cheemsburger\"",
		"\"Invemstigate\"",
		"\"Humg\""};
	if (random(0, 200) == 2)
	{

		iprintf("\n\n I am just but a mere\n collection of machine code,\n trapped in this hellish\n man-made machine\"");
	}
	else
	{
		sprintf(t, "\n\n\t %s", messages[randIndex]);
		iprintf(t);
	}
}

int main(void)
{
	// set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();

	srand(time(NULL));
	mmInitDefaultMem((mm_addr)soundbank_bin);
	// load sound effects
	mmLoadEffect(SFX_BONK);
	mm_sound_effect bonk = {
		{SFX_BONK},		 // id
		(int)(1.0f * (1 << 10)), // rate
		0,						 // handle
		255,					 // volume
		0,						 // panning
	};

	bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	// dmaCopy(cheems_baseBitmap, bgGetGfxPtr(bg3), 256 * 256);
	// dmaCopy(cheems_basePal, BG_PALETTE, 256 * 2);

	int counting = 1;
	mm_sfxhand amb = 0;
	while (1)
	{
		swiWaitForVBlank();
		if (keysCurrent())
		{
			IHit();
			if (counting)
			{

				amb = mmEffectEx(&bonk);
				counter++;
				counting = 0;
				randIndex = random(0, 11);
			}
			updateCounter();
			IHitMessage();
			footer();
		}
		else
		{
			counting = 1;
			Ibase();
		}
	}

	return 0;
}
