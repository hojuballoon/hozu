//�f�t�H���g�ł�memset��memcpy�ɒu���������Ă��܂��̂ŁA�Ӓn�ł�Win32API���Ăяo��
#undef RtlMoveMemory
#undef RtlFillMemory
#undef RtlZeroMemory
EXTERN_C NTSYSAPI VOID NTAPI
RtlMoveMemory(LPVOID UNALIGNED Dst, LPCVOID UNALIGNED Src, SIZE_T Length);
EXTERN_C NTSYSAPI VOID NTAPI
RtlFillMemory(LPVOID UNALIGNED Dst, SIZE_T Length, BYTE Pattern);
EXTERN_C NTSYSAPI VOID NTAPI
RtlZeroMemory(LPVOID UNALIGNED Dst, SIZE_T Length);
