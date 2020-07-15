#include "SF2Handle.h"

#include "FileHandle.h"

sf::SF2Handle::SF2Handle(std::istream& is) : SF2HandleBase(is)
{
	is.seekg(0, std::ios::end);
	_len = static_cast<UINT32>(is.tellg());
	is.seekg(0);

	_sfbk = searchChunk(Range{ 0, _len }, "RIFF");
	_info = searchChunk(Range{ _sfbk.pos, _sfbk.pos + _sfbk.sz }, "LIST");
	_sdta = searchChunk(Range{ _info.pos + _info.sz, _sfbk.pos + _sfbk.sz }, "LIST");
	_pdta = searchChunk(Range{ _sdta.pos + _sdta.sz, _sfbk.pos + _sfbk.sz }, "LIST");

	preset_handle.reset(new PresetHandle(is, Range{ _pdta.pos, _pdta.pos + _pdta.sz }));
	inst_handle.reset(new InstrumentHandle(is, Range{ preset_handle->range(3).end, _pdta.pos + _pdta.sz }));
	smpl_handle.reset(new SampleHandle(is,  Range{ _sdta.pos, _sdta.pos + _sdta.sz },
											Range{ inst_handle->range(3).end, _pdta.pos + _pdta.sz }));
}

sf::SF2Handle::~SF2Handle()
{

}
