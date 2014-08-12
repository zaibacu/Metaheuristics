#include "../MetaHeuristic.h"

#include <time.h>
#include <ctime>
#include "ILogger.h"
#include "LogLocator.h"
#include "WorkerLocator.h"
#include "StaticWorker.h"
#include "ThreadedWorker.h"

#include "ConsoleLogger.h"

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

using namespace std::placeholders;

int main()
{
	srand (time(NULL));
	
	Util::CConsoleLoger Logger();
	Service::CLogLocator::ProvideLog(&Logger);
	Service::CStaticWorker Worker;
	Service::CWorkerLocator::ProvideWorker(&Worker);
	
	Service::CThreadedWorker t0;
	Service::CWorkerLocator::ProvideWorker(&t0);
	
	Service::CThreadedWorker t1;
	Service::CWorkerLocator::ProvideWorker(&t1);
	
	Service::CThreadedWorker t2;
	Service::CWorkerLocator::ProvideWorker(&t2);
	
	Service::CThreadedWorker t3;
	Service::CWorkerLocator::ProvideWorker(&t3);
	
	EvolutionaryAlgorithms::CGAAlgorithm<short, short> ME;
	ME.Init();	

	Algorithm::DataSet<short>::Type vInput;
	
	vInput.push_back(0);
	
	vInput.push_back(1);
	
	vInput.push_back(2);
	
	vInput.push_back(3);
	
	vInput.push_back(4);
	
	vInput.push_back(5);
	
	vInput.push_back(6);
	
	vInput.push_back(7);
	
	vInput.push_back(8);
	
	vInput.push_back(9);
	
	vInput.push_back(10);
	
	vInput.push_back(11);
	
	vInput.push_back(12);
	
	vInput.push_back(13);
	

	try
	{
		clock_t begin = clock();
		auto vResult = ME.Compute(vInput, std::bind(Fitness, _1, _2), std::bind(Stop, _1, _2, _3, _4));

		clock_t end = clock();
		double dMsecs = double(end - begin) / CLOCKS_PER_SEC * 1000;
		Logger.Log("Time: ");
		Logger.Log(dMsecs);
		Logger.Log("---------");

		for(auto Item : vResult)
		{
			Logger.Log(Item);
		}
	}
	catch(const char* p_chError)
	{
		Logger.Log(p_chError);
	}
	catch(...)
	{
		Logger.Log("Unknown exception");
	}
}
