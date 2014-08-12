#include "Param.h"
#include "Eg1.h"

const double g_cdPenality = 100000; //Extra weight if we going through same route again

double Fitness(const Algorithm::DataSet<short>::Type& p_crInput, const Algorithm::DataSet<short>::Type& p_crOutput)
{
	
	double dCost = 0;
	int iLast = -1;
	for(auto& rPoint : p_crOutput)
	{
		if(iLast == -1)
		{
			iLast = rPoint;
			continue;
		}

		dCost += Coord<double>::Distance(g_Coords[iLast], g_Coords[rPoint]);
		iLast = rPoint;
	}
	const unsigned int cuiSize = p_crOutput.size();
	for(unsigned int i = 0; i < cuiSize; i++)
	{
		for(unsigned int j = 0; j < cuiSize; j++)
		{
			if(i == j)
				continue;

			if(p_crOutput[i] == p_crOutput[j])
				dCost += g_cdPenality;
		}
	}

	return dCost;
}

bool Stop(const int p_ciIteration, const double p_cdFitness, const double p_cdMin, const double p_cdMax)
{
	return p_ciIteration > 10000 || abs(p_cdFitness) <= 40;
}