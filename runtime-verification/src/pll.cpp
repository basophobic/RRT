
#include <stdio.h>
#include <fstream>
#include "pll.h"
vector<double>  PLL::simulate(double* nodeset, vector<double> variation, vector<string> settings, double dt){
	vector<double> result;
	//	double dt = 5e-6;
	result.push_back(dt);
	FILE * ic;
	string aString;
	double rise, fall, delay;
	char nextString[80];
	ic = fopen("pll.ic0", "w");

	fprintf(ic, ".option \n");
	fprintf(ic, "	+ gmindc=   1.0000p       \n");
	fprintf(ic, "	.nodeset	\n");
	fprintf(ic, "	+ e =  %f   \n", nodeset[pll_e]);
	fprintf(ic, "	+ eb =  %f       \n", nodeset[pll_eb]);
	fprintf(ic, "	+ in =   %f            \n", nodeset[pll_in]);
	fprintf(ic, "	+ inb =   %f            \n", nodeset[pll_inb]);
	fprintf(ic, "	+ mout =  %f        \n", nodeset[pll_mout]);
	fprintf(ic, "	+ moutb =  %f        \n", nodeset[pll_moutb]);
	fprintf(ic, "	+ osc = %f       \n", nodeset[pll_osc]);
	fprintf(ic, "	+ oscb =  %f        \n", nodeset[pll_oscb]);
	fprintf(ic, "	+ out =  %f        \n", nodeset[pll_out]);
	fprintf(ic, "	+ outb =  %f        \n", nodeset[pll_outb]);
	fprintf(ic, "	+ xvco.c =   %f        \n", nodeset[pll_xvco_c]);
	fprintf(ic, "	+ xvco.s =  %f       \n", nodeset[pll_xvco_s]);
	fprintf(ic, "	+ xvco.s_clip =  %f       \n", nodeset[pll_xvco_s_clip]);
	fprintf(ic, "	+ xpd.clip1 =   %f            \n", nodeset[pll_xpd_clip1]);
	fprintf(ic, "	+ xpd.clip2 = %f       \n", nodeset[pll_xpd_clip2]);
	fprintf(ic, "	+ xpd.n1 =   %f            \n", nodeset[pll_xpd_n1]);

	fclose(ic);
	ic = fopen("param.sp", "w");
	fprintf(ic, ".param var=%f\n", variation);
	fclose(ic);

	system("hspice pll.sp > Sim.txt");

	string line;
	ifstream simResult("pll.ic0");

	if (simResult.good()){
		for (int i = 0; i<12; i++)
			getline(simResult, line);

		for (int i = 0; i<16; i++){
			getline(simResult, line);
			line = line.substr(line.find_first_of("=") + 1, line.length());
			double d;
			stringstream ss(line);
			ss >> d;
			line.erase(line.find_last_not_of(" \n\r\t") + 1);
			char c = line.c_str()[line.length() - 1];
			d = d*System::unit(c);
			result.push_back(d);
		}
	}
	simResult.close();

	return result;
}



void  PLL::setInitialPLLState(double* state){
	/*
	*
	* .nodeset
	+ e =  -3.5000
	+ eb =  -3.3000
	+ in =   0.
	+ inb =   0.
	+ mout =  -3.5000
	+ moutb =  -3.5000
	+ osc =-999.7550m
	+ oscb =  -1.0002
	+ out =  -3.5000
	+ outb =  -3.3000
	+ xvco.c =   1.0000
	+ xvco.s =  61.2612u
	+ xvco.s_clip =  81.6816u
	+ xpd.clip1 =   0.
	+ xpd.clip2 = 653.4528u
	+ xpd.n1 =   0.
	*/

	state[pll_e] = -3.5000;
	state[pll_eb] = -3.3000;
	state[pll_in] = 0;
	state[pll_inb] = 0;
	state[pll_mout] = -3.5000;
	state[pll_moutb] = -3.5000;
	state[pll_osc] = -999.7550e-3;
	state[pll_oscb] = -1.0002;
	state[pll_out] = -3.5000;
	state[pll_outb] = -3.3000;
	state[pll_xvco_c] = 1.0000;
	state[pll_xvco_s] = 61.2612e-6;
	state[pll_xvco_s_clip] = 81.6816e-6;
	state[pll_xpd_clip1] = 0;
	state[pll_xpd_clip2] = 653.4528e-6;
	state[pll_xpd_n1] = 0;
	state[pll_time] = 0;
}
