#include "BitmapSteganalysis.h"

using namespace CVStegoCrypt;

BitmapSteganalysis::AnalysisBuffer::AnalysisBuffer()
	: current(0), cycles(0), bits(0), positions(gcnew array<UInt32>(64)), _posBuffer(gcnew array<UInt32>(64))
{
	for(int i = 0; i < positions->Length; ++i) positions[i] = 0;
}
					
inline void BitmapSteganalysis::AnalysisBuffer::setNext(BitmapIterator^ i, bool value)
{
	if(value) bits |= (1ULL << current);
	else bits &= ~(1ULL << current);
	positions[current] = i->iByte + 1;
	if(++current >= 64) {
		current = 0;
		++cycles;
	}
}

inline void BitmapSteganalysis::AnalysisBuffer::swapBits()
{
	if(current > 0) {
		Byte invCurrent = 64 - current;
		UInt64 firstBits = bits & ((1ULL << current) - 1);
		(bits >>= current) |= (firstBits << invCurrent);

		for(Int32 i = 0; i < current; ++i) _posBuffer[i] = positions[i];
		for(Int32 i = current; i < positions->Length; ++i) positions[i - current] = positions[i];
		for(Int32 i = 0; i < current; ++i) positions[i + invCurrent] = _posBuffer[i];
	}
}

inline UInt32 BitmapSteganalysis::AnalysisBuffer::getIntStartingFrom(Byte bit)
{
	static const UInt64 mask = (1ULL << 32) - 1;
	return (bits >> bit) & mask;
}