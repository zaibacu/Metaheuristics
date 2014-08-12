#pragma once
namespace EA
{
	template<typename _Structure, typename _ResultType>
	class IEAAlgorithm
	{
	public:
		virtual ~IEAAlgorithm(){};
		
	protected:
		/*
			Evolutionary algorithms Selection operator
			@param Population
		*/
		virtual std::vector<_Structure> Selection(std::vector<std::pair<double, _Structure>>&) = 0;
		/*
			Evolutionary algorithms Cross operator
			@param Mother
			@param Father
			@return Child
		*/
		virtual _Structure Cross(const _Structure&, const _Structure&) const = 0;
		/*
			Evolutionary algorithms Mutation operator
			@param Element
		*/
		virtual void Mutate(_Structure&) = 0;
	};
}
