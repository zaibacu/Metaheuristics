#pragma once
#include "../IAlgorithm.h"
#include "../EA/IEAAlgorithm.h"
#include "../ObjectFactory.h"
#include "../LogLocator.h"
#include "../FitnessCalculator.h"
/*#define SIZE 5
#define POPULATION_SIZE 25*/

namespace EvolutionaryAlgorithms
{
	typedef std::vector<int> Schema;


	/**
	 *	A Standart Evolutionary Algorithms class algorithm, Genetic Algorithm implementation
	 *
	 */
	template<typename _Input, typename _Output>
	class CGAAlgorithm : public Algorithm::IAlgorithm<_Input, _Output>, public EA::IEAAlgorithm<Schema, double>
	{
	private:
		typedef typename Algorithm::DataSet<_Input>::Type InputDataSet;
		typedef typename Algorithm::DataSet<_Output>::Type OutputDataSet;
		typedef typename Algorithm::Fitness<InputDataSet, OutputDataSet>::Function FittnessFn;
		typedef typename Algorithm::Stop::Function StopFn;

		std::vector<Schema> m_vPopulation;
	public:
		/**
		 *	Init function
		 *	Used to fill required parameters for algorithm to start
		 */
		virtual void Init() override
		{
			for(int i = 0; i<POPULATION_SIZE; i++)
				m_vPopulation.push_back(GenerateSchema());
		}

		/**
		 *	Computation method. 
		 *	@param p_crInput gives input variables
		 *	@param p_fnFitness is our defined Fitness function
		 *	@param p_fnStop is our defined Stop function
		 *	
		 *	@return computation result
		 */
		virtual OutputDataSet Compute(const InputDataSet& p_crInput, const FittnessFn p_fnFitness, const StopFn p_fnStop) override
		{
			Util::ILogger* pLog = Service::CLogLocator::GetLog();
			typedef std::pair<double, Schema> ResultPair;
			unsigned long ulIteration = 0;
			while(true)
			{
				std::vector<ResultPair> vResult;
				Util::CFitnessCalculator Calc;
				for(auto& rSchema : m_vPopulation)
				{
					OutputDataSet vOutput = Convert(rSchema, p_crInput);
					
					double Fitness = p_fnFitness(p_crInput, vOutput);
					Calc.AddFitness(Fitness);

					if(p_fnStop(ulIteration, Fitness, Calc.GetMin(), Calc.GetMax()))
						return vOutput;

					vResult.push_back(std::make_pair(Fitness, rSchema));
				}

				auto vSelected = Selection(vResult);

				const int ciSize = vSelected.size();
				m_vPopulation.clear();
				m_vPopulation.insert(m_vPopulation.begin(), vSelected.begin(), vSelected.end());
				for(int i = 0; i<ciSize; i++)
				{
					for(int j = 0; j<ciSize; j++)
					{
						if(i == j)
							continue;

						m_vPopulation.push_back(Cross(vSelected[i], vSelected[j]));
						const int iRand = Util::GetRandom<int>(0, 100);
						if(iRand == 0)
						{
							Mutate(m_vPopulation.back());
						}
					}
				}

				ulIteration++;
				pLog->Log(Util::IterationData(ulIteration, Calc.GetMin(), Calc.GetMax(), Calc.GetAvg()));
			}
		}

	private:
		//GA
		/**
		 *	Evolutionary algorithms Mutation operator
		 *	@param Element
		 */
		virtual void Mutate(Schema& p_rSchema) override
		{
			int iPoint = Util::GetRandom(0, SIZE);
			p_rSchema[iPoint] = Util::GetRandom(0, SIZE);
		} 

		/**
		 *	Evolutionary algorithms Cross operator
		 *	@param Mother
		 *	@param Father
		 *	@return Child
		 */
		virtual Schema Cross(const Schema& p_crLeft, const Schema& p_crRight) const override
		{
			Schema Child;
			int iPoint = Util::GetRandom(0, SIZE);
			for(int i=0; i<SIZE; i++)
			{
				if(i<iPoint)
					Child.push_back(p_crLeft[i]);
				else
					Child.push_back(p_crRight[i]);
			}

			return Child;
		}

		/**
		 *	Evolutionary algorithms Selection operator
		 *	@param Population
		 */
		virtual std::vector<Schema> Selection(std::vector<std::pair<double, Schema>>& p_rvResults) override
		{
			typedef std::pair<double, Schema> ResultPair;
			std::vector<Schema> vSelected;

			std::sort(p_rvResults.begin(), p_rvResults.end(), [](const ResultPair& crLeft, const ResultPair& crRight)->bool{
					return crLeft.first < crRight.first;
			});

			const int ciSelectionCount = sqrt(POPULATION_SIZE);

			for(int i = 0; i<ciSelectionCount; i++)
			{
				vSelected.push_back(p_rvResults[i].second);
			}

			return vSelected;
		}

		/**
		 *	Generates random schema
		 *	@return Schema
		 */
		virtual Schema GenerateSchema()
		{
			Schema NewSchema;
			for(int i = 0; i<SIZE; i++)
			{
				NewSchema.push_back(Util::GetRandom(0, SIZE));
			}

			return NewSchema;
		}

		/**
		 *	Converts schema to output
		 *	@param p_crSchema
		 *	@param p_crInput
		 *	@return Output
		 */
		virtual OutputDataSet Convert(const Schema& p_crSchema, const InputDataSet& p_crInput)
		{
			OutputDataSet Out;
			for(int iIndex : p_crSchema)
			{
				Out.push_back(p_crInput[iIndex]);
			}

			return Out;
		}
	};
}