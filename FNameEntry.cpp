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
	char* v2; // rbx
	unsigned int NameLength; // edi
	unsigned int v5; // eax
	int v6; // edx
	unsigned int v7; // r8d
	__int64 v8; // rax

	v2 = OutName;

	NameLength = Entry->GetNameLength();

	memcpy(OutName, Entry + Entry->GetDataOffset(), NameLength);

	v5 = *(unsigned int*)(BaseAddress + 0xEC6EEB0);

	v6 = (v5 << 8) | (v5 >> 8);

	v7 = v5 >> 7;

	if (NameLength)
	{
		v8 = NameLength;
		do
		{
			v6 += v7;
			*v2++ ^= v6;
			--v8;
		} while (v8);
	}
}

void DecryptWide(FNameEntry* Entry, char* OutName)
{
	char* v2; // rdi
	unsigned int NameLength; // ebx
	unsigned int v5; // eax
	int v6; // edx
	unsigned int v7; // r8d
	__int64 v8; // rax

	v2 = OutName;

	NameLength = Entry->GetNameLength();

	memcpy(OutName, Entry + Entry->GetDataOffset(), 2 * NameLength);

	v5 = *(unsigned int*)(BaseAddress + 0xEC6EEB0);

	v6 = (v5 << 8) | (v5 >> 8);

	v7 = v5 >> 7;

	if (NameLength)
	{
		v8 = NameLength;
		do
		{
			v6 += v7;
			*v2++ ^= v6;
			--v8;
		} while (v8);
	}
}
