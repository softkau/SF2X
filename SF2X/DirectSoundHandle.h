#pragma once
#include "VoiceGroupBase.h"
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include "FileHandle.h"

struct SampleInfo
{
	UINT32 bit_depth;
	UINT32 loopflag;
	UINT32 loop_point;
	UINT32 smpl_rate;
	UINT32 overriding_rootkey;
	UINT32 pitch_correction;
};

struct SampleKey
{
	UINT8 loopflag;
	UINT32 freqfix;
	UINT32 loop_start;
	UINT32 size;
	size_t wav_hash;

	bool operator==(const SampleKey& other) const
	{
		return loopflag == other.loopflag
			&& freqfix == other.freqfix
			&& loop_start == other.loop_start
			&& size == other.size
			&& wav_hash == other.wav_hash;
	}
};

/*
	<This class is for adding samples into ROM>
	The 1st argument for the ctor is an output stream (= ROM to be modified)
	The 2nd argument for the ctor is the initial *sample insertion point

	It also checks duplicated sample insertion

	(sample insertion point)
	An offset where sample is going to be inserted by add method
*/
class DirectSoundHandle : public VoiceGroupBase
{
public:
	DirectSoundHandle(std::ostream& os, UINT32 smpl_pos);
	~DirectSoundHandle();

	// inserts a wave sample with given info at _smpl_pos(the position automatically handled)
	// insertion fails when the sample has already been inserted (to be exact, exists in _sample_chache)
	// returns a location of added sample as a Range (start ~ end), including the padding bytes when succeeded
	// returns a location of the same sample that exists in ROM as a Range (start ~ end), including the padding bytes when failed
	Range add(const std::vector<UINT8>& wav, const SampleInfo& info);

private:
	UINT32 _smpl_pos = 0;
	std::unique_ptr<std::unordered_map<SampleKey, Range>> _smpl_cache; // checks for duplicated samples
};
