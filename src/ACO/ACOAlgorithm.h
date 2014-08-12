#pragma once
#include <map>
#include <utility>  
#include "../IAlgorithm.h"
#include "../Util.h"
#include "../FitnessCalculator.h"
#include "../WorkerLocator.h"

/*#define ANTS 5
#define ALPHA 1
#define BETA 1
#define Q 1.0
#define RO 0.5
#define STEPS 5*/
namespace SwarmIntelligence
{
	/**
	 *	A Pheromone definition used by ants
	 */
	template<typename T>
	struct Pheromone
	{
		typedef std::map<std::pair<T, T>, double> Type;
	};
	
	/**
	 *	Ant agent implementation
	 */
	template<typename _Input>
	struct Ant
	{
		typedef typename Pheromone<_Input>::Type PheromoneMap;
		typedef std::function<double(const _Input&, const _Input&)> CostFn;
		typedef std::pair<_Input, _Input> Step;

		std::map<_Input, double> m_mpChances;
		double m_dChancesSum;
		_Input m_Start;
		_Input m_End;
		double m_Cost;
		Step m_pairMove;
		std::vector<Step> m_vPath;

		Ant()
		{
			Reset();
		}

		/**
		 *	Resets Ant state
		 */
		void Reset()
		{
			m_Cost = 0;
			m_Start = Util::Default<_Input>();
			m_End = Util::Default<_Input>();
			m_dChancesSum = 0;
			m_vPath.clear();
		}

		/**
		 *	Search for possible move
		 *	@param p_crMove Possible moves
		 *	@param p_rPheromone Pheromone Matrix
		 *	@param p_fnCost Cost function
		 */
		void Search(const std::vector<_Input>& p_crMove, PheromoneMap& p_rPheromone, CostFn p_fnCost)
		{
			m_dChancesSum = 0;
			m_mpChances.clear();
			for(const _Input& crItem : p_crMove)
			{
				const double cdCost = p_fnCost(m_Start, crItem);
				const double cdPheromone = p_rPheromone[std::make_pair(m_Start, crItem)];
				const double cdChance = pow(cdPheromone, ALPHA) * pow((double)1/cdCost, BETA);
				m_mpChances[crItem] = cdChance;
				m_dChancesSum += cdChance;
			}

			
		}

		/**
		 *	Does ant move
		 *	@param p_fnCost Cost Function
		 */
		void Move(CostFn p_fnCost)
		{
			long lUpper = static_cast<long>(m_dChancesSum);
			const unsigned long ulRandom = lUpper == 0? 0 : Util::GetRandom<long>(0, lUpper);
			double dSum = 0;
			for(auto& rIter : m_mpChances)
			{
				dSum += rIter.second;
				if(ulRandom < dSum)
				{
					m_End = rIter.first;
					break;
				}
			}
			

			m_Cost += p_fnCost(m_Start, m_End);
			m_pairMove = std::make_pair(m_Start, m_End);

			m_Start = m_End;
			m_vPath.push_back(m_pairMove);
		}

		std::vector<_Input> GetPath()
		{
			
			std::vector<_Input> vResult;
			bool bOdd = true;
			for(auto& rStep : m_vPath)
			{
				if(bOdd)
					vResult.push_back(rStep.first);
				else
					vResult.push_back(rStep.second);

				bOdd = !bOdd;
			}
			return vResult;
		}
	};


	/**
	 *	A Basic standart, Swarm Intelligence class, Ant Colony Optimization algorithm implementation
	 *	
	 */
	template<typename _Input, typename _Output>
	class CACOAlgorithm : public Algorithm::IAlgorithm<_Input, _Output>
	{
	private:
		typedef typename Algorithm::DataSet<_Input>::Type InputDataSet;
		typedef typename Algorithm::DataSet<_Output>::Type OutputDataSet;
		typedef typename Algorithm::Fitness<InputDataSet, OutputDataSet>::Function FittnessFn;
		typedef typename Algorithm::Stop::Function StopFn;
		typedef typename Pheromone<_Input>::Type PheromoneMap;

		PheromoneMap m_mpPheromone;
		std::vector<Ant<_Input>> m_vAnts;
	public:
		virtual ~CACOAlgorithm(){};

		/**
		 *	Init function
		 *	Used to fill required parameters for algorithm to start
		 */
		virtual void Init() override
		{
			//Allocate space in memory
			m_vAnts.reserve(ANTS);
			for(int i = 0; i<ANTS; i++)
				m_vAnts.push_back(Ant<_Input>());
		}

		/**
		 *	Computation method. 
		 *	@param p_crInput gives input variables
		 *	@param p_fnFitness is our defined Fitness function
		 *	@param p_fnStop is our defined Stop function
		 *	@return computation result
		 */
		virtual OutputDataSet Compute(const InputDataSet& p_crInput, const FittnessFn p_fnFitness, const StopFn p_fnStop) override
		{
			OutputDataSet Output;
			Util::ILogger* pLog = Service::CLogLocator::GetLog();
			auto fnCost = [&](const _Input& p_crStart, const _Input& p_crEnd)->double{
					OutputDataSet Path;
					Path.push_back(p_crStart);
					Path.push_back(p_crEnd);
					return p_fnFitness(p_crInput, Path);
			};

			//Fill pheromone map
			for(auto& rOuter : p_crInput)
			{
				for(auto& rInner : p_crInput)
				{
					m_mpPheromone[std::make_pair(rOuter, rInner)] = 1;
				}
			}

			unsigned long ulIteration = 0;
			while(true)
			{
				Util::CFitnessCalculator Calc;

				//Clear Ant memory
				for(auto& rAnt : m_vAnts)
				{
					rAnt.Reset();
				}

				for(unsigned int uiStep = 0; uiStep < STEPS; uiStep++)
				{
					GenerateSolutions(p_crInput, fnCost);
					DaemonActions(fnCost);
					PheromoneUpdate(fnCost);
				}

				for(auto& rAnt : m_vAnts)
				{
					auto&& rrPath = rAnt.GetPath();
					double Fitness = p_fnFitness(p_crInput, rrPath);
					Calc.AddFitness(Fitness);
					if(p_fnStop(ulIteration, Fitness, Calc.GetMin(), Calc.GetMax()))
						return rrPath;
				}

				pLog->Log(Util::IterationData(ulIteration, Calc.GetMin(), Calc.GetMax(), Calc.GetAvg()));
				ulIteration++;
			}

			return Output;
		}
		//ACO specific stuff
	private:
		/**
		 *	Generates Solutions
		 *	@param p_crInput
		 *	@param p_fnCost Cost Function
		 */
		virtual void GenerateSolutions(const InputDataSet& p_crInput, typename Ant<_Input>::CostFn p_fnCost)
		{
			for(auto& rAnt : m_vAnts)
			{
				auto pWorker = Service::CWorkerLocator::GetWorker();
				pWorker->Do([&](){
					rAnt.Search(p_crInput, m_mpPheromone, p_fnCost);
				});
			}

			Util::WaitFor([]()->bool{
				return !Service::CWorkerLocator::IsWorkDone();
			});
		}

		/**
		 *	Simulates Ants movement
		 *	@param p_fnCost Cost Function
		 */
		virtual void DaemonActions(typename Ant<_Input>::CostFn p_fnCost)
		{
			for(auto& rAnt : m_vAnts)
			{
				auto pWorker = Service::CWorkerLocator::GetWorker();
				pWorker->Do([&](){
					rAnt.Move(p_fnCost);
				});
			}

			Util::WaitFor([]()->bool{
				return !Service::CWorkerLocator::IsWorkDone();
			});
		}

		/**
		 *	Updates Pheromone matrix
		 *	@param p_fnCost Cost Function
		 */
		virtual void PheromoneUpdate(typename Ant<_Input>::CostFn p_fnCost)
		{
			for(auto& rIter : m_mpPheromone)
			{
				double dPheromone = (1 - RO) * rIter.second;
				auto& rKey = rIter.first;
				for(auto& rAnt : m_vAnts)
				{
					auto& rAntMove = rAnt.m_pairMove;
					if(rKey.first == rAntMove.first && rKey.second == rAntMove.second)
					{
						dPheromone += Q / p_fnCost(rKey.first, rKey.second);
					}
					
				}
			}
			
		}
	};
}