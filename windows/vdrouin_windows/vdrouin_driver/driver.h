#ifndef _DRIVER_
#define _DRIVER_

#include <ntifs.h>
#include <ntddk.h>

NTSTATUS Hook_Function();
void Unhook_fonction();

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
        unsigned int *ServiceTableBase;
        unsigned int *ServiceCounterTableBase; //Used only in checked build
        unsigned int NumberOfServices;
        unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]

PMDL  g_pmdlSystemCall;
PVOID *MappedSystemCallTable;


// Relatif a NtCreateFile
NTSYSAPI
NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE phFile,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);

typedef NTSTATUS (*NTCREATEFILE)(
            PHANDLE FileHandle,
            ACCESS_MASK DesiredAccess,
            POBJECT_ATTRIBUTES ObjectAttributes,
            PIO_STATUS_BLOCK IoStatusBlock,
            PLARGE_INTEGER AllocationSize OPTIONAL,
            ULONG FileAttributes,
            ULONG ShareAccess,
            ULONG CreateDisposition,
            ULONG CreateOptions,
            PVOID EaBuffer OPTIONAL,
            ULONG EaLength
);

NTCREATEFILE OldNtCreateFile;

NTSTATUS NewNtCreateFile(
                    PHANDLE FileHandle,
                    ACCESS_MASK DesiredAccess,
                    POBJECT_ATTRIBUTES ObjectAttributes,
                    PIO_STATUS_BLOCK IoStatusBlock,
                    PLARGE_INTEGER AllocationSize OPTIONAL,
                    ULONG FileAttributes,
                    ULONG ShareAccess,
                    ULONG CreateDisposition,
                    ULONG CreateOptions,
                    PVOID EaBuffer OPTIONAL,
                    ULONG EaLength
);

typedef NTSTATUS (*NTOPENPROCESS)(
	PHANDLE            ProcessHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID         ClientId
);

NTOPENPROCESS OldNtOpenProcess;

NTSTATUS NewNtOpenProcess(
	PHANDLE            ProcessHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID         ClientId
);

// fin de Relatif a NtCreateFile



#endif
