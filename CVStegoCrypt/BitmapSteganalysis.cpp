#include "BitmapSteganalysis.h"

using namespace CVStegoCrypt;

BitmapSteganalysis::BitmapSteganalysis(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue)
	: BitmapStegoBase(red, green, blue)
{
	clear();
}

List<AnalysisPotentialOutcome^>^ BitmapSteganalysis::compareBitmaps(UInt32 startPosition, UInt32 endPosition, UInt32 maxUnchangedBits, UInt32% out_firstDiffPos, UInt32% out_lastDiffPos)
{
	if(_originalBitmap == nullptr || _carryBitmap == nullptr || (OriginalBitmapPixelCount != CarryBitmapPixelCount))
		return gcnew List<AnalysisPotentialOutcome^>(0);

	if(!maxUnchangedBits) maxUnchangedBits = _originalBmpData->Length;

	_initReadOnlyBitmapData(_originalBitmap, _originalBmpData);
	_initReadOnlyBitmapData(_carryBitmap, _carryBmpData);

	AnalysisBuffer^ buffer = gcnew AnalysisBuffer();
	TwinBitmapIterator^ i = gcnew TwinBitmapIterator(_carryBmpData, _originalBmpData, startPosition, endPosition);
	i->initializeColorMasks(_rBits, _gBits, _bBits);

	// scan for first diff bit
	while(!i->isDone) {
		bool currentCarryBit = i->CurrentBit1;
		bool currentOriginalBit = i->CurrentBit2;
		buffer->setNext(i->Iterator, currentCarryBit);
		i->iterate();
		if(currentCarryBit != currentOriginalBit) break;
	}
	
	if(i->isDone) return gcnew List<AnalysisPotentialOutcome^>(0);

	out_firstDiffPos = i->iByte;

	// get next 32 bits to check possible sizes
	if(_analizeNext32Bits(i->Iterator, buffer) != 32)
		return gcnew List<AnalysisPotentialOutcome^>(0);

	buffer->swapBits();

	array<UInt64>^ sizes = gcnew array<UInt64>((buffer->cycles > 0) ? 32 : (buffer->current - 32));

	for(Byte j = 0; j < sizes->Length; ++j)
		sizes[j] = buffer->getIntStartingFrom(j);

	// scan for last diff bit
	out_lastDiffPos = out_firstDiffPos;
	UInt32 nUnchanged = 0;

	while(!i->isDone) {
		if(i->CurrentBit1 != i->CurrentBit2) {
			nUnchanged = 0;
			out_lastDiffPos = i->iByte;
		}
		else if(++nUnchanged == maxUnchangedBits) break;
		i->iterate();
	}

	Int32 byteDiff = out_lastDiffPos - out_firstDiffPos;
	Int64 bitsPerPixel = BitsPerPixel;

	// calc end positions from possible sizes
	array<UInt64>^ endPositions = gcnew array<UInt64>(sizes->Length);
	for(int j = 0; j < sizes->Length; ++j)
		endPositions[j] = (UInt64)buffer->positions[j] + _getPixelsNeeded((UInt32)sizes[j] + 4) * 3;

	List<AnalysisPotentialOutcome^>^ outcomes = gcnew List<AnalysisPotentialOutcome^>(32);
	for(int j = 0; j < sizes->Length; ++j) {
		if(endPositions[j] < _originalBmpData->Length) {
			AnalysisPotentialOutcome^ outcome = gcnew AnalysisPotentialOutcome();
			outcome->potentialPosition = buffer->positions[j];
			outcome->potentialSize = (Int32)sizes[j];
			outcome->guessAccuracy = ((out_lastDiffPos > endPositions[j]) ? (endPositions[j] / (float)out_lastDiffPos) : ((float)out_lastDiffPos / endPositions[j])) * 100.0f;
			outcomes->Add(outcome);
		}
	}

	outcomes->Sort();
	outcomes->Reverse();
	_originalBmpData = gcnew array<Byte>(0);
	_carryBmpData = gcnew array<Byte>(0);
	return outcomes;
}

List<AnalysisOutcome^>^ BitmapSteganalysis::analizeCarry(UInt32 startPosition, UInt32 endPosition, UInt32 minSize, UInt32 maxSize)
{
	if(_carryBitmap == nullptr || startPosition >= (UInt32)CarryBitmapLength || endPosition >= (UInt32)CarryBitmapLength || !BitsPerPixel)
		return gcnew List<AnalysisOutcome^>(0);

	_initReadOnlyBitmapData(_carryBitmap, _carryBmpData);

	BitmapIterator^ i = _initBitmapIterator(_carryBmpData, startPosition, endPosition);
	AnalysisBuffer^ buffer = gcnew AnalysisBuffer();

	if(_analizeNext32Bits(i, buffer) != 32)
		return gcnew List<AnalysisOutcome^>(0);

	List<AnalysisOutcome^>^ outcomes = gcnew List<AnalysisOutcome^>();
	Byte bitsPerPixel = BitsPerPixel;
	Byte j = 32;
	
	while(j == 32) {
		j = _analizeNext32Bits(i, buffer);

		buffer->swapBits();
		buffer->current = 0;

		for(Byte k = 0; k < j; ++k) {
			UInt64 size = buffer->getIntStartingFrom(k);
			if(size <= _carryBmpData->Length) {
				UInt32 endPos = buffer->positions[k] + _getPixelsNeeded((UInt32)size + 4) * 3;
				if(endPos < (UInt32)CarryBitmapLength && size >= minSize && size <= maxSize)
					outcomes->Add(gcnew AnalysisOutcome((Int32)size, buffer->positions[k]));
			}
		}
	}

	outcomes->Sort();
	outcomes->Reverse();
	_carryBmpData = gcnew array<Byte>(0);
	return outcomes;
}

bool BitmapSteganalysis::guessNomimatedBits()
{
	if(_originalBitmap == nullptr || _carryBitmap == nullptr || (_originalBmpData->Length != _carryBmpData->Length))
		return false;

	_rBits->reset();
	_gBits->reset();
	_bBits->reset();

	_initReadOnlyBitmapData(_originalBitmap, _originalBmpData);
	_initReadOnlyBitmapData(_carryBitmap, _carryBmpData);

	_trySetBitNominators();
	return true;
}

bool BitmapSteganalysis::loadOriginalBitmap(String^ filePath)
{
	clear();
	return _loadBmp(filePath, _originalBmpPath, _originalBitmap);
}

bool BitmapSteganalysis::loadCarryBitmap(String^ filePath)
{
	return _loadBmp(filePath, _carryBmpPath, _carryBitmap);
}

inline Bitmap^ BitmapSteganalysis::getOriginalBitmap()
{ return _originalBitmap; }

inline Bitmap^ BitmapSteganalysis::getCarryBitmap()
{ return _carryBitmap; }

void BitmapSteganalysis::clear()
{
	_originalBmpPath = "";
	_carryBmpPath = "";
	_originalBmpData = gcnew array<Byte>(0);
	_carryBmpData = gcnew array<Byte>(0);
	_originalBitmap = nullptr;
	_carryBitmap = nullptr;
}

inline Byte BitmapSteganalysis::_analizeNext32Bits(BitmapIterator^ i, AnalysisBuffer^ buffer)
{
	for(Byte j = 0; j < 32; i->iterate(), ++j) {
		if(i->isDone) return j;
		buffer->setNext(i, i->CurrentBit);
	}
	return 32;
}

void BitmapSteganalysis::_trySetBitNominators()
{
	for(Int32 i = 0, clr = 0; i < _originalBmpData->Length; ++i) {
		for(Byte b = 0; b < 8; ++b) {
			if(((_originalBmpData[i] >> b) & 1) != ((_carryBmpData[i] >> b) & 1)) {
				if(clr == 0) _bBits->set(b, true);
				else if(clr == 1) _gBits->set(b, true);
				else _rBits->set(b, true);
			}
		}
		if(++clr >= 3) clr = 0;
	}
	_originalBmpData = gcnew array<Byte>(0);
	_carryBmpData = gcnew array<Byte>(0);
}