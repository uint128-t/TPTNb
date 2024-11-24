#include "simulation/ElementCommon.h"
#include "simulation/Audio.h"
#include "PowderToySDL.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_NBLK()
{
	Identifier = "DEFAULT_PT_NBLK";
	Name = "NBLK";
	Colour = 0xaf7e3d_rgb;
	MenuVisible = 1;
	MenuSection = SC_ELEC;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 0;
	Description = "Noteblock, plays a note when sparked.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	bool snd = false;
	for (auto rx = -1; rx <= 1; rx++)
	{
		for (auto ry = -1; ry <= 1; ry++)
		{
			if ((rx || ry) && abs(rx) + abs(ry) != 2)
			{
				auto r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (TYP(r)==PT_SPRK && parts[ID(r)].life==3)
				{
					snd = true;
				}
			}
		}
	}
	if (snd){
		int instr = parts[i].ctype; // todo
		int ptch = parts[i].temp - 273;
		int note = ptch/10;
		int frames = parts[i].life;
		if (frames<=0){
			frames = 5000;
		}
		double freq = 440 * pow(2.0, note / 12.0);
		// Main thread is only reading from audt and writing to sounds
		for (int sn = 0; sn<SND_MAX; sn++){
			if (sndtime[sn]<=audt){ // Replacable
				sndfreq[sn] = freq;
				sndtime[sn] = audt + frames; 
				break;
			}
		}
		// don't play a sound if all playing
		SDL_PauseAudio(0);
	}
	return 0;
}
