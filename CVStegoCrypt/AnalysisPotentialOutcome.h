#pragma once

#include "AnalysisOutcome.h"

namespace CVStegoCrypt
{
	using namespace System;

	public ref class AnalysisPotentialOutcome : public AnalysisOutcome
	{
		public:
			float guessAccuracy;

			AnalysisPotentialOutcome() : AnalysisOutcome(), guessAccuracy(0) {}
			AnalysisPotentialOutcome(float accuracy, Int32 size, Int32 position) : AnalysisOutcome(size, position), guessAccuracy(accuracy) {}

			void set(float accuracy, Int32 size, Int32 position)
			{
				guessAccuracy = accuracy;
				potentialSize = size;
				potentialPosition = position;
			}

			virtual int CompareTo(Object^ obj) override
			{
				if(obj->GetType() != GetType())
					return 1;

				AnalysisPotentialOutcome^ other = (AnalysisPotentialOutcome^)obj;
				int result = guessAccuracy.CompareTo(other->guessAccuracy);
				return (result == 0) ? _compareTo(other) : result;
			}

			virtual String^ ToString() override
			{
				return "Accuracy: " + String::Format("{0:0.00}", guessAccuracy) + ", " + _toString();
			}
	};
}