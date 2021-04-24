typedef BOOLEAN(WINAPI* LPRTLGR)(PVOID RandomBuffer, ULONG RandomBufferLength);

BOOL AdvapiLoad();
BOOL AdvapiFree();
BOOLEAN WINAPI AdvapiRtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
