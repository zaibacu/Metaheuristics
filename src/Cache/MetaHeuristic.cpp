#include "../MetaHeuristic.h"

#include <time.h>
#include <ctime>
#include "../ILogger.h"
#include "../LogLocator.h"
#include "../WorkerLocator.h"
#include "../StaticWorker.h"
#include "../ThreadedWorker.h"

#include "../PythonLogger.h"

#include "Param.h"


using namespace std::placeholders;

extern "C" 
{
	void Run(FnTunnel p_fnTunnel)
	{
		srand (time(NULL));
		
		Util::CPythonLogger Logger(p_fnTunnel);
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
}