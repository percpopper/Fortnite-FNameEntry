struct FNameEntryHeader
{
	uint16 bIsWide : 1;
	static constexpr uint32 ProbeHashBits = 5;
	uint16 LowercaseProbeHash : ProbeHashBits;
	uint16 Len : 10;
};

struct FNameEntry 
{
	enum { NAME_SIZE = 1024 };
public:


	FORCEINLINE static int32 GetDataOffset()
	{
		return offsetof(FNameEntry, AnsiName);
	}

	FORCEINLINE bool IsWide() const
	{
		return Header.bIsWide;
	}

	FORCEINLINE int32 GetNameLength() const
	{
		return Header.Len;
	}

	FORCEINLINE FNameEntryHeader GetEntryHeader() const
	{
		return Header;
	}

	FNameEntryHeader Header;

	union
	{
		ANSICHAR	AnsiName[NAME_SIZE];
		WIDECHAR	WideName[NAME_SIZE];
	};
};

void DecryptNonWide(FNameEntry* Entry, char* OutName)
{
	char* v2; // rdi
	int NameLength; // ebx
	int v5; // edx
	__int64 result; // rax
	int v7; // eax
	char v8; // cl

	v2 = OutName;

	NameLength = Entry->GetNameLength();

	memcpy(OutName, Entry + Entry->GetDataOffset(), NameLength);

	v5 = 0;
	result = 42i64;
	if (NameLength)
	{
		do
		{
			v7 = v5 | result;
			++v2;
			++v5;
			v8 = ~(_BYTE)v7;
			result = (unsigned int)(2 * v7);
			*(v2 - 1) ^= v8;
		} while (v5 < NameLength);
	}
}

void DecryptWide(FNameEntry* Entry, wchar* OutName)
{
	wchar* v2; // rdi
	int NameLength; // ebx
	int v5; // edx
	__int64 result; // rax
	int v7; // eax
	char v8; // cl

	v2 = OutName;

	NameLength = Entry->GetNameLength();

	memcpy(OutName, Entry + Entry->GetDataOffset(), 2 * NameLength);

	v5 = 0;
	result = 42i64;
	if (NameLength)
	{
		do
		{
		      v7 = v5++ | result;
		      v8 = v7;
		      result = 2 * v7;
		      *v2++ ^= ~v8;
		} while (v5 < NameLength);
	}
}
