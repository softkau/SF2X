#include "DirectSoundHandle.h"
#include "endian.h"

#define BYTE_4_SPLIT(X) ((X) >> 24u) & 0xFFu, ((X) >> 16u) & 0xFFu, ((X) >> 8u) & 0xFFu, (X) & 0xFFu

namespace
{
	size_t hash_combine(size_t left, size_t right)
	{
		left ^= right + 0x9E3779B9 + (left << 6) + (left >> 2);
		return left;
	}

	size_t hash_wav(const std::vector<UINT8>& wav)
	{
		size_t h = std::hash<UINT8>()(wav[0]);
		for (size_t i = 0; i < wav.size() - 1; i++)
		{
			h = hash_combine(h, std::hash<UINT8>()(wav[i + 1]));
		}
		return h;
	}

	std::vector<UINT8> cnv_wav(const std::vector<UINT8>& wav, UINT32 bit_depth)
	{
		if (bit_depth == 16)
		{
			std::vector<UINT8> wav8(wav.size() / 2);
			for (size_t i = 0; i < wav8.size(); i++)
			{
				wav8[i] = wav[2 * i + 1];
			}
			return wav8;
		}
		else
		{
			std::vector<UINT8> wav8(wav.size());
			for (UINT32 s = 0; s < wav.size(); s++)
			{
				wav8[s] = wav[s] ^ 0x80;
			}
			return wav8;
		}
	}

	SampleKey getskey(const SampleInfo& info, const std::vector<UINT8>& wav8)
	{
		SampleKey sk{ 0 };
		if (info.loopflag)
		{
			sk.loopflag = 0x40;
			sk.loop_start = info.loop_point - 1;
		}
		sk.freqfix = (UINT32)(1024.0 * info.smpl_rate * pow(2.0, (60.0 - (double)info.overriding_rootkey) / 12.0) * pow(2.0, info.pitch_correction / 1200.0));
		sk.wav_hash = hash_wav(wav8);
		sk.size = wav8.size() - 1;
		return sk;
	}
}

namespace std
{
	template<>
	struct hash<SampleKey>
	{
		size_t operator()(const SampleKey& s) const
		{
			size_t h1 = std::hash<UINT8>()(s.loopflag);
			size_t h2 = std::hash<UINT32>()(s.freqfix);
			size_t h3 = std::hash<UINT32>()(s.loop_start);
			size_t h4 = std::hash<UINT32>()(s.size);

			size_t h5 = hash_combine(h1, h2);
			size_t h6 = hash_combine(h3, h4);

			return hash_combine(hash_combine(h5, h6), s.wav_hash);
		}
	};
}

DirectSoundHandle::DirectSoundHandle(std::ostream& os, UINT32 smpl_pos) : VoiceGroupBase(os), _smpl_pos(smpl_pos)
{
	_smpl_cache.reset(new std::unordered_map<SampleKey, Range>());
}

DirectSoundHandle::~DirectSoundHandle()
{
}

Range DirectSoundHandle::add(const std::vector<UINT8>& wav, const SampleInfo& info)
{
	std::vector<UINT8> wav8 = cnv_wav(wav, info.bit_depth);
	SampleKey sk = getskey(info, wav8);

	Range res;

	auto search_result = _smpl_cache->find(sk);
	if (search_result == _smpl_cache->end())
	{		
		os.seekp(_smpl_pos);
		align();
		_smpl_pos = static_cast<UINT32>(os.tellp());

		UINT32 tab1 = byteswap(sk.freqfix);
		UINT32 tab2 = byteswap(sk.loop_start);
		UINT32 tab3 = byteswap(sk.size);

		unsigned char header[] = { 0, 0, 0, sk.loopflag, BYTE_4_SPLIT(tab1), BYTE_4_SPLIT(tab2), BYTE_4_SPLIT(tab3) };
		os.write((char*)header, 16);
		os.write((const char*)&wav8[0], wav8.size());

		addPaddings();

		res = Range(_smpl_pos, (UINT32)os.tellp());
		(*_smpl_cache)[sk] = res;
		_smpl_pos = res.end;
	}
	else
	{
		res = search_result->second;
	}
	return res;
}
