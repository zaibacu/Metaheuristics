#pragma once

#include "../IAlgorithm.h"
#include "../EA/IEAAlgorithm.h"
#include "../LogLocator.h"
#include "../FitnessCalculator.h"
#include "../PtrWrapper.h"

namespace EvolutionaryAlgorithms
{

	enum class EExpression
	{
		eLiteral,
		eAdd,
		eSub,
		eDiv,
		eMult,

		eSize
	};

	/**
	 *	Base Expression interface
	 */
	class IExpression
	{
	public:
		
		virtual ~IExpression(){};

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const = 0;
	};

	/**
	 *	Literal expression. It means that that this expression can provide an actual value
	 */
	template<typename T>
	class CLiteral : public IExpression
	{
	private:
		T m_Value;
	public:
		CLiteral(const T& p_crValue)
			: m_Value(p_crValue)
		{

		}
		virtual ~CLiteral(){};

		/**
		 *	A Basic mutator: Value Getter
		 *	@return Value
		 */
		T Get() const
		{
			return m_Value;
		}

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const override
		{
			return EExpression::eLiteral;
		}
	};

	/**
	 *	Mathematic operation expression. Does addition
	 */
	template<typename T>
	class CAdd : public IExpression
	{
	public:
		CAdd(){};
		virtual ~CAdd(){};

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const override
		{
			return EExpression::eAdd;
		}
	};

	/**
	 *	Mathematic operation expression. Does substitution
	 */
	template<typename T>
	class CSub : public IExpression
	{
	public:
		CSub(){};
		virtual ~CSub(){};

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const override
		{
			return EExpression::eSub;
		}
	};

	/**
	 *	Mathematic operation expression. Does multiplication
	 */
	template<typename T>
	class CMult : public IExpression
	{
	public:
		CMult(){};
		virtual ~CMult(){};

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const override
		{
			return EExpression::eMult;
		}
	};

	/**
	 *	Mathematic operation expression. Does division
	 */
	template<typename T>
	class CDiv : public IExpression
	{
	public:
		CDiv(){};
		virtual ~CDiv(){};

		/**
		 *	Returns expression type
		 *	@return Type
		 */
		virtual EExpression GetType() const override
		{
			return EExpression::eDiv;
		}
	};

	typedef std::vector<Util::CPtrWrapper<IExpression>> Schema;


	/**
	 *	Virtual stack based machine
	 */
	template<typename T>
	class GPVM
	{
	private:
		std::vector<T> m_Stack;
	public:
		/**
		 *	Converts stack values to result
		 *	@param p_crSchema
		 *	@return Result of calculation
		 */
		double Compute(const Schema& p_crSchema)
		{
			m_Stack.clear();
			for(auto& rExpression : p_crSchema)
			{
				Interpret(rExpression.Get());
			}
			while(m_Stack.size() > 0)
			{
				return pop();
			}
			return 0;
		}
	
	private:
		/**
		 *	Evaluates Expressions
		 *	@param Expression
		 */
		void Interpret(const IExpression* p_cpExp)
		{
			switch(p_cpExp->GetType())
			{
			case EExpression::eLiteral:
				{
					const CLiteral<T>* cpLiteral = dynamic_cast<const CLiteral<T>*>(p_cpExp);
					push(cpLiteral->Get());
				}
				break;
			case EExpression::eAdd:
				{
					T Val1 = pop();
					T Val2 = pop();
					push(Val1 + Val2);
				}
				break;
			case EExpression::eSub:
				{
					T Val1 = pop();
					T Val2 = pop();
					push(Val1 - Val2);
				}
				break;
			case EExpression::eMult:
				{
					T Val1 = pop();
					T Val2 = pop();
					push(Val1 * Val2);
				}
				break;
			case EExpression::eDiv:
				{
					T Val1 = pop();
					T Val2 = pop();
					push(Val2 != 0 ? Val1 / Val2 : 0);
				}
				break;
			default:
				break;
			}	
		}

		/**
		 *	Stack push operator
		 *	@param p_crValue
		 */
		void push(const T& p_crValue)
		{
			m_Stack.push_back(p_crValue);
		}

		/**
		 *	Stack pop operator
		 *	@return Result
		 */
		T pop()
		{
			if(m_Stack.size() == 0)
				return 0;

			T Value = m_Stack.back();
			m_Stack.pop_back();

			return Value;
		}

	};

	/**
	 *	Genetic Programming Algorithm based on Virtual Stack Machine implmentation
	 */
	template<typename _Input, typename _Output>
	class CVMGPAlgorithm : public Algorithm::IAlgorithm<_Input, _Output>, 
						   public EA::IEAAlgorithm<Schema, _Output> 
	{
	private:
		typedef typename Algorithm::DataSet<_Input>::Type InputDataSet;
		typedef typename Algorithm::DataSet<_Output>::Type OutputDataSet;
		typedef typename Algorithm::Fitness<InputDataSet, OutputDataSet>::Function FittnessFn;
		typedef typename Algorithm::Stop::Function StopFn;

		GPVM<_Input> m_VM;
		std::vector<Schema> m_vPopulation;

	public:
		CVMGPAlgorithm(){};
		virtual ~CVMGPAlgorithm(){};

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
					double Fitness = 0;
					std::vector<_Output> vResults;
					for(unsigned int uiIndex = 0; uiIndex < p_crInput.size(); uiIndex += INPUT_SIZE)
					{
						auto first = p_crInput.begin() + uiIndex;
						auto last = p_crInput.begin() + uiIndex + INPUT_SIZE;
						std::vector<_Input> vInput(first, last);
						std::vector<_Output> vOutput;
						auto Result = m_VM.Compute(rSchema);
						vOutput.push_back(Result);
						vResults.push_back(Result);
						Fitness += p_fnFitness(vInput, vOutput);
					}

					Calc.AddFitness(Fitness);

					if(p_fnStop(ulIteration, Fitness, Calc.GetMin(), Calc.GetMax()))
						return vResults;

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

	protected:
		/**
		 *	Evolutionary algorithms Selection operator
		 *	@param Population
		 */
		virtual std::vector<Schema> Selection(std::vector<std::pair<double, Schema>>& p_rvResults)
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
		 *	Evolutionary algorithms Cross operator
		 *	@param Mother
		 *	@param Father
		 *	@return Child
		 */
		virtual Schema Cross(const Schema& p_crLeft, const Schema& p_crRight) const
		{
			Schema Child;
			int iSize = p_crLeft.size();
			int iPoint = Util::GetRandom(0, iSize);
			for(int i=0; i<iPoint; i++)
			{
				Child.push_back(p_crLeft[i]);
			}

			for(unsigned int i=iPoint; i<p_crRight.size(); i++)
			{
				Child.push_back(p_crRight[i]);
			}

			return Child;
		}

		/**
		 *	Evolutionary algorithms Mutation operator
		 *	@param Element
		 */
		virtual void Mutate(Schema& p_rSchema)
		{
			int iSize = p_rSchema.size();
			int iPoint = Util::GetRandom(0, iSize);
			p_rSchema[iPoint] = GetRandomExp();
		}

		virtual Schema GenerateSchema()
		{
			Schema NewSchema;
			int iSize = Util::GetRandom(0, MAX_DEPTH);
			for(int i = 0; i<iSize; i++)
			{
				NewSchema.push_back(GetRandomExp());
			}

			return NewSchema;
		}

		virtual Util::CPtrWrapper<IExpression> GetRandomExp()
		{
			auto Exp = static_cast<EExpression>(Util::GetRandom(0, static_cast<int>(EExpression::eSize) - 1));
			switch(Exp)
			{
			case EExpression::eLiteral:
				return Util::CPtrWrapper<IExpression>(new CLiteral<_Input>(Util::GetRandom(Util::GetMin<_Input>(), Util::GetMax<_Input>())));
			case EExpression::eAdd:
				return Util::CPtrWrapper<IExpression>(new CAdd<_Input>());
			case EExpression::eSub:
				return Util::CPtrWrapper<IExpression>(new CSub<_Input>());
			case EExpression::eMult:
				return Util::CPtrWrapper<IExpression>(new CMult<_Input>());
			case EExpression::eDiv:
				return Util::CPtrWrapper<IExpression>(new CDiv<_Input>());
			default:
				throw "Invalid expression";
			}

		} 
	};
}