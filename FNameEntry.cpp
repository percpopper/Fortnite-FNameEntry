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
	__int64 result; // rax
	int v6; // r8d
	unsigned int v7; // ecx
	char v8; // dl

	v2 = OutName;

	NameLength = Entry->GetNameLength();

	memcpy(OutName, Entry + Entry->GetDataOffset(), NameLength);

	static uint64 BaseAddress = 0;
	if (!BaseAddress) BaseAddress = reinterpret_cast<uint64>(GetModuleHandleA(NULL));

	unsigned int Key = *(unsigned int*)(BaseAddress + 0xEB41A40);

	result = (unsigned int)Key >> 5;

	v6 = 0;
	if (NameLength)
	{
		do
		{
			v7 = *v2++;
			v8 = result ^ (16 * v7) ^ (result ^ (v7 >> 4)) & 0xF;
			result = (unsigned int)(result + 4 * v6++);
			*(v2 - 1) = v8;
		} while (v6 < NameLength);
	}
	return result;
}

void DecryptWide(FNameEntry* Entry, wchar* OutName)
{
	wchar* v2; // rdi
	int NameLength; // ebx
	__int64 result; // rax
	int v6; // r8d
	__int16 v7; // dx

	v2 = OutName;

	memcpy(OutName, Entry + Entry->GetDataOffset(), 2 * NameLength);

	NameLength = *a1 >> 6;

	static uint64 BaseAddress = 0;
	if (!BaseAddress) BaseAddress = reinterpret_cast<uint64>(GetModuleHandleA(NULL));

	unsigned int Key = *(unsigned int*)(BaseAddress + 0xEB41A40);

	result = (unsigned int)Key >> 5;

	v6 = 0;
	if (NameLength)
	{
		do
		{
			v7 = result ^ (*v2 << 8);
			result = (unsigned int)(result + 4 * v6++);
			*v2 = HIBYTE(*v2) | v7 & 0xFF00;
			++v2;
		} while (v6 < NameLength);
	}
}
