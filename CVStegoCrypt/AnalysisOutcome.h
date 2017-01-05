#pragma once

namespace CVStegoCrypt
{
	using namespace System;

	public ref class AnalysisOutcome : public IComparable
	{
		public:
			Int32 potentialSize;
			Int32 potentialPosition;

			AnalysisOutcome() : potentialSize(0), potentialPosition(0) {}
			AnalysisOutcome(Int32 size, Int32 position) : potentialSize(size), potentialPosition(position) {}

			void set(Int32 size, Int32 position)
			{
				potentialSize = size;
				potentialPosition = position;
			}

			virtual int CompareTo(Object^ obj)
			{
				if(obj->GetType() != GetType()) return 1;
				return _compareTo((AnalysisOutcome^)obj);
			}

			virtual String^ ToString() override
			{
				return _toString();
			}

		protected:
			int _compareTo(AnalysisOutcome^ other)
			{
				int result = potentialSize.CompareTo(other->potentialSize);
				return (result == 0) ? potentialPosition.CompareTo(other->potentialPosition) : result;
			}

			String^ _toString()
			{
				return "Size: " + potentialSize + ", Position: " + potentialPosition;
			}
	};
}