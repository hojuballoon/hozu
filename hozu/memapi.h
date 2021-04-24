//デフォルトではmemsetやmemcpyに置き換えられてしまうので、意地でもWin32APIを呼び出す
#undef RtlMoveMemory
#undef RtlFillMemory
#undef RtlZeroMemory
EXTERN_C NTSYSAPI VOID NTAPI
RtlMoveMemory(LPVOID UNALIGNED Dst, LPCVOID UNALIGNED Src, SIZE_T Length);
EXTERN_C NTSYSAPI VOID NTAPI
RtlFillMemory(LPVOID UNALIGNED Dst, SIZE_T Length, BYTE Pattern);
EXTERN_C NTSYSAPI VOID NTAPI
RtlZeroMemory(LPVOID UNALIGNED Dst, SIZE_T Length);
