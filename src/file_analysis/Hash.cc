#include <string>

#include "Hash.h"
#include "util.h"

using namespace file_analysis;

Hash::Hash(RecordVal* args, Info* info, HashVal* hv, const char* field)
	: Action(args, info), hash(hv), fed(false)
	{
	using BifType::Record::FileAnalysis::ActionResults;
	if ( (result_field_idx = ActionResults->FieldOffset(field)) < 0 )
		reporter->InternalError("Missing ActionResults field: %s", field);
	hash->Init();
	}

Hash::~Hash()
	{
	Unref(hash);
	}

bool Hash::DeliverStream(const u_char* data, uint64 len)
	{
	if ( ! hash->IsValid() ) return false;

	if ( ! fed )
		fed = len > 0;

	hash->Feed(data, len);
	return true;
	}

bool Hash::EndOfFile()
	{
	Finalize();
	return false;
	}

bool Hash::Undelivered(uint64 offset, uint64 len)
	{
	return false;
	}

void Hash::Finalize()
	{
	if ( ! hash->IsValid() || ! fed ) return;

	StringVal* sv = hash->Get();
	info->GetResults(args)->Assign(result_field_idx, sv);
	}