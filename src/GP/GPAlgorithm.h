#pragma once
#include "../IAlgorithm.h"
#include "../EA/IEAAlgorithm.h"
#include "../ObjectFactory.h"
#include "../FitnessCalculator.h"
#include "../PtrWrapper.h"

/*#define POPULATION_SIZE 25
#define INPUT_SIZE 2*/

namespace EvolutionaryAlgorithms
{
	//Nodes
	enum class ENodeType
	{
		eFunction,
		eTerminal,
		eConstant
	};

	/*
		Base class for representing GP Node

	*/
	template<typename _Input, typename _Output>
	struct INode : public Util::IObject
	{
		INode<_Input, _Output>* m_pParent;
		/**
		 * @brief Returns parent node
		 */
		virtual INode<_Input, _Output>* GetParent()
		{
			return m_pParent;
		}

		typedef std::vector<_Input> InputDataSet;
		/*
			Node type getter
			@return Node type
		*/
		virtual ENodeType GetType() const = 0;
		/*
			Method to calculate value
			@param p_crInput
			@return Value of _Output type
		*/
		virtual _Output Calculate(const InputDataSet& p_crInput) const = 0;
		/*
			Get random type node for be a child
			@return Random node
		*/
		virtual INode<_Input, _Output>* GetRandom() = 0;
	};

	/*
		Function node class. Has two children, returns result by applying operator to them

	*/
	template<typename _Input, typename _Output>
	struct FunctionNode : INode<_Input, _Output>
	{
		FunctionNode()
		{
			m_fnCalc = [](const _Input, const _Input)->_Output{
				return _Output();
			};
		}

		/*
			Node type getter
			@return Node type
		*/
		virtual ENodeType GetType() const { return ENodeType::eFunction; };

		typedef std::vector<_Input> InputDataSet;
		std::vector<INode<_Input, _Output>*> m_Children;
		std::function<_Output(const _Input, const _Input)> m_fnCalc;
		/*
			Method to calculate value
			@param p_crInput
			@return Value of _Output type
		*/
		virtual _Output Calculate(const InputDataSet& p_crInput) const
		{
			/*
			 * Function is const because we promise, that we'll not change the inner state.
			 * Howerver, vectors' operator [] is non-const, so we need to cast it.
			 */
			return m_fnCalc(const_cast<INode<_Input, _Output>*>(m_Children[0])->Calculate(p_crInput), 
							const_cast<INode<_Input, _Output>*>(m_Children[1])->Calculate(p_crInput));
		}

		/*
			Get random type node for be a child
			@return Random node
		*/
		virtual INode<_Input, _Output>* GetRandom() 
		{
			const int ciRand = Util::GetRandom<int>(0, 3);

			switch(ciRand)
			{
				case 0: //We give self
					return this;
				case 1: //Responsability to 1st child
					return m_Children[0]->GetRandom();
				case 2: //Responsability to 2nd child
					return m_Children[1]->GetRandom();
				default:
					return nullptr;
			}
		}
	};

	/*
		Constant node class. Returns a constant value

	*/
	template<typename _Input, typename _Output>
	struct ConstantNode : INode<_Input, _Output>
	{
		ConstantNode()
		{
			m_Constant = _Output();
		}

		/*
			Node type getter
			@return Node type
		*/
		virtual ENodeType GetType() const { return ENodeType::eConstant; };

		typedef std::vector<_Input> InputDataSet;
		_Output m_Constant;
		/*
			Method to calculate value
			@param p_crInput
			@return Value of _Output type
		*/
		virtual _Output Calculate(const InputDataSet& p_crInput) const
		{
			return m_Constant;
		}

		/*
			Get random type node for be a child
			@return Random node
		*/
		virtual INode<_Input, _Output>* GetRandom()
		{
			return this;
		}
	};

	/*
		Terminal node. Returns one of the inputs

	*/
	template<typename _Input, typename _Output>
	struct TerminalNode : INode<_Input, _Output>
	{
		TerminalNode()
		{
			m_ciIndex =  0;
		}

		virtual ENodeType GetType() const { return ENodeType::eTerminal; };

		typedef std::vector<_Input> InputDataSet;
		int m_ciIndex;
		virtual _Output Calculate(const InputDataSet& p_crInput) const
		{
			return p_crInput[m_ciIndex];
		}

		virtual INode<_Input, _Output>* GetRandom()
		{
			return this;
		}
	};

	/*
		Genetic Programming algorithm implementation


	*/
	template<typename _Input, typename _Output>
	class CGPAlgorithm : public Algorithm::IAlgorithm<_Input, _Output>, 
						 public EA::IEAAlgorithm<Util::CPtrWrapper<INode<_Input, _Output>>, _Output>
	{
	private:
		typedef typename Algorithm::DataSet<_Input>::Type InputDataSet;
		typedef typename Algorithm::DataSet<_Output>::Type OutputDataSet;
		typedef typename Algorithm::Fitness<InputDataSet, OutputDataSet>::Function FittnessFn;
		typedef typename Algorithm::Stop::Function StopFn;
		//GP Stuff
		typedef std::function<_Output(const _Input, const _Input)> fnFunction;
		std::vector<fnFunction> m_vFunctions;

		typedef INode<_Input, _Output> Node;
		typedef Util::CPtrWrapper<Node> NodePtr;
		typedef std::pair<double, NodePtr> ResultPair;
		typedef FunctionNode<_Input, _Output> FnNode;
		typedef ConstantNode<_Input, _Output> CNode;
		typedef TerminalNode<_Input, _Output> TNode;

		std::vector<NodePtr> m_vPopulation;
	

	public:
		CGPAlgorithm(){}
		virtual ~CGPAlgorithm(){};
		/*
			Init function
			Used to fill required parameters for algorithm to start
		*/
		virtual void Init() override
		{
			//Fill possible functions
			m_vFunctions.push_back([](const _Input p_cLeft, const _Input p_cRight)->_Output{
				return p_cLeft + p_cRight;
			});

			m_vFunctions.push_back([](const _Input p_cLeft, const _Input p_cRight)->_Output{
				return p_cLeft - p_cRight;
			});

			m_vFunctions.push_back([](const _Input p_cLeft, const _Input p_cRight)->_Output{
				return p_cLeft * p_cRight;
			});

			m_vFunctions.push_back([](const _Input p_cLeft, const _Input p_cRight)->_Output{
				return p_cRight != 0 ? p_cLeft / p_cRight : 0;
			});

			//Fill population
			for(int i = 0; i<POPULATION_SIZE; i++)
			{
				m_vPopulation.push_back(GenerateTree());
			}

		}

		/*
			Computation method. 
			@param p_crInput gives input variables
			@param p_fnFitness is our defined Fitness function
			@param p_fnStop is our defined Stop function

			@return computation result
		*/
		virtual OutputDataSet Compute(const InputDataSet& p_crInput, const FittnessFn p_fnFitness, const StopFn p_fnStop) override
		{
			Util::ILogger* pLog = Service::CLogLocator::GetLog();
			
			unsigned long ulIteration = 0;
			while(true)
			{
				Util::CFitnessCalculator Calc;
				std::vector<ResultPair> vResult;
				for(auto& rNode : m_vPopulation)
				{
					Prune(rNode.Get(), 0);

					double Fitness = 0;
					std::vector<_Output> vResults;
					for(unsigned int uiIndex = 0; uiIndex < p_crInput.size(); uiIndex += INPUT_SIZE)
					{
						auto first = p_crInput.begin() + uiIndex;
						auto last = p_crInput.begin() + uiIndex + INPUT_SIZE;
						std::vector<_Input> vInput(first, last);
						std::vector<_Output> vOutput;
						auto Result = rNode->Calculate(vInput);
						vOutput.push_back(Result);
						vResults.push_back(Result);
						Fitness += p_fnFitness(vInput, vOutput);
					}
					
					Calc.AddFitness(Fitness);
					if(p_fnStop(ulIteration, Fitness, Calc.GetMin(), Calc.GetMax()))
						return vResults;

					vResult.push_back(std::make_pair(Fitness, rNode));
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
							Mutate(m_vPopulation.back());
					}
				}

				ulIteration++;
				pLog->Log(Util::IterationData(ulIteration, Calc.GetMin(), Calc.GetMax(), Calc.GetAvg()));
			}
		}

		//GP specific methods
	protected:
		virtual void Prune(Node* p_pSource, const long p_clDepth)
		{
			switch(p_pSource->GetType())
			{
			case ENodeType::eConstant:
			case ENodeType::eTerminal:
				break;
			case ENodeType::eFunction:
				if(p_clDepth > MAX_DEPTH)
				{
					auto pParent = dynamic_cast<FnNode*>(p_pSource->GetParent());
					auto& rvSiblings = pParent->m_Children;
					if(rvSiblings[0] == p_pSource)
					{
						*(pParent->m_Children[0]) = *GenerateLeaf(false).Get();
					}
					else
					{
						*(pParent->m_Children[1]) = *GenerateLeaf(false).Get();
					}
				}
				else //We pass Pruning process to our children
				{
					auto fnNode = dynamic_cast<FnNode*>(p_pSource);
					Prune(fnNode->m_Children[0], p_clDepth + 1);
					Prune(fnNode->m_Children[1], p_clDepth + 1);
				}
				break;
			}
		}
		/*
			Generates random tree of Nodes
			@return Tree of Nodes
		*/
		virtual FunctionNode<_Input, _Output>* GenerateTree() const
		{
			FnNode* pRoot = new FnNode();
			pRoot->m_Children.push_back(GenerateLeaf().Get());
			pRoot->m_Children.push_back(GenerateLeaf().Get());
			//Show children whom their father is
			pRoot->m_Children[0]->m_pParent = pRoot;
			pRoot->m_Children[1]->m_pParent = pRoot;
			int iRandFn = Util::GetRandom<int>(0, m_vFunctions.size());
			pRoot->m_fnCalc = m_vFunctions[iRandFn];
			return pRoot;
		}

		/*
			Generates random Node
			@return Node
		*/
		virtual NodePtr GenerateLeaf(const bool p_cbFunction = true) const
		{
			int iRand = Util::GetRandom(p_cbFunction ? 0 : 1, 2);
			switch(iRand)
			{
				case 0: //Function
				{
					FnNode* pNode = Util::CObjectFactory::GetInstance()->Create<FnNode>();
					pNode->m_Children.push_back(GenerateLeaf().Get());
					pNode->m_Children.push_back(GenerateLeaf().Get());
					//Show children whom their father is
					pNode->m_Children[0]->m_pParent = pNode;
					pNode->m_Children[1]->m_pParent = pNode;

					int iRandFn = Util::GetRandom<int>(0, m_vFunctions.size());
					pNode->m_fnCalc = m_vFunctions[iRandFn];
					return NodePtr(pNode);
				}
				case 1: //Terminal
				{
					TNode* pNode = Util::CObjectFactory::GetInstance()->Create<TNode>();
					pNode->m_ciIndex = Util::GetRandom<int>(0, INPUT_SIZE);
					return NodePtr(pNode);
				}
				case 2: //Constant
				{
					CNode* pNode = Util::CObjectFactory::GetInstance()->Create<CNode>();
					pNode->m_Constant = Util::GetRandom(Util::GetMin<_Output>(), Util::GetMax<_Output>());
					return NodePtr(pNode);
				}
				default:
					return NodePtr();
			}
		}

		/*
			Makes a copy of Node
			@param p_cpSource
			@return a copy of Node
		*/
		virtual NodePtr Copy(const NodePtr p_cpSource) const
		{
			NodePtr pNode;
			switch(p_cpSource->GetType())
			{
				case ENodeType::eFunction:
				{
					pNode.Set(Util::CObjectFactory::GetInstance()->Create<FnNode>());
					auto Left = dynamic_cast<FnNode*>(pNode.Get());
					auto Right = dynamic_cast<const FnNode*>(p_cpSource.Get());
					Left->m_fnCalc = Right->m_fnCalc;
					Left->m_Children.push_back(Copy(Right->m_Children[0]).Get());
					Left->m_Children.push_back(Copy(Right->m_Children[1]).Get());
					break;
				}
				case ENodeType::eTerminal:
				{
					pNode.Set(Util::CObjectFactory::GetInstance()->Create<TNode>());
					dynamic_cast<TNode*>(pNode.Get())->m_ciIndex = dynamic_cast<const TNode*>(p_cpSource.Get())->m_ciIndex;
					break;
				}
				case ENodeType::eConstant:
				{
					pNode.Set(Util::CObjectFactory::GetInstance()->Create<CNode>());
					dynamic_cast<CNode*>(pNode.Get())->m_Constant = dynamic_cast<const CNode*>(p_cpSource.Get())->m_Constant;
					break;
				}
			}

			return pNode;
		}

		/*
			Evolutionary algorithms Selection operator
			@param Population
		*/
		virtual std::vector<NodePtr> Selection(std::vector<ResultPair>& p_rvResults)
		{
			std::vector<NodePtr> vSelected;

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

		/*
			Evolutionary algorithms Cross operator
			@param Mother
			@param Father
			@return Child
		*/
		virtual NodePtr Cross(const NodePtr& p_crLeft, const NodePtr& p_crRight) const
		{
			NodePtr pNode = Copy(p_crLeft.Get());
			//Take random part of tree
			auto pRandomLeaf = pNode->GetRandom();
			//... and change it to random part of other tree
			*pRandomLeaf = *Copy(p_crRight.Get())->GetRandom();
			return NodePtr(pNode);
		}

		/*
			Evolutionary algorithms Mutation operator
			@param Element
		*/
		virtual void Mutate(NodePtr& p_pSource)
		{
			auto pRandomLeaf = p_pSource->GetRandom();
			switch(pRandomLeaf->GetType())
			{
				case ENodeType::eFunction:
					dynamic_cast<FnNode*>(pRandomLeaf)->m_fnCalc = m_vFunctions[Util::GetRandom<int>(0, m_vFunctions.size())];
				break;
				case ENodeType::eConstant:
					dynamic_cast<CNode*>(pRandomLeaf)->m_Constant = Util::GetRandom(Util::GetMin<_Output>(), Util::GetMax<_Output>());
				break;
				case ENodeType::eTerminal:
					dynamic_cast<TNode*>(pRandomLeaf)->m_ciIndex = Util::GetRandom<int>(0, INPUT_SIZE);
				break;
			}
		}
	};
}