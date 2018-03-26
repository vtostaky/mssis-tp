#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

typedef struct Process Process;
struct Process
{
    DWORD pid;
    TCHAR exeName[MAX_PATH];
    Process *next;
};

typedef struct ProcessList ProcessList;
struct ProcessList
{
    Process *first;
};

ProcessList *initList()
{
    ProcessList *liste = malloc(sizeof(*liste));
    Process *element = malloc(sizeof(*element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->pid = 0;
    element->next = NULL;
    liste->first = element;

    return liste;
}

void insertToList(ProcessList *liste, DWORD pid, TCHAR *exeName)
{
    Process *proc = malloc(sizeof(*proc));

    if (liste == NULL || proc == NULL)
    {
        exit(EXIT_FAILURE);
    }

    proc->pid = pid;
    memcpy(proc->exeName, exeName, MAX_PATH);
    proc->next = liste->first;
    liste->first = proc;
}

void removeFromList(ProcessList *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->first != NULL)
    {
        Process *proc = liste->first;
        liste->first = liste->first->next;
        free(proc);
    }
}

void cleanList(ProcessList *list)
{
    if(list != NULL)
    {
        while(list->first != NULL)
        {
            removeFromList(list);
        }
    }
}

void getProcesses(ProcessList *list){
    HANDLE snapHandle;
    BOOL exist;
    PROCESSENTRY32 ppe;
    int nb_pids = 0;
    printf("Get processes\n");
    snapHandle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);

    exist = Process32First(snapHandle, &ppe);
    while(exist)
    {
        printf("PID %ld : Name %s\n",ppe.th32ProcessID, ppe.szExeFile);
        insertToList(list, ppe.th32ProcessID, ppe.szExeFile);
        exist = Process32Next(snapHandle, &ppe);
        nb_pids++;
    }
    printf("Number of PIDs %d\n",nb_pids);
    CloseHandle(snapHandle);
}

void listAllOpenProcesses(ProcessList *list){
    HANDLE procHandle;
    DWORD dwProcessId;
    int nb_pids = 0;
    HMODULE mod;
    DWORD cbNeeded;
    TCHAR processName[MAX_PATH];

    for(dwProcessId = 0; dwProcessId < 65535; dwProcessId+=4)
    {
        procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
        if(procHandle == INVALID_HANDLE_VALUE)
            continue;

        if(EnumProcessModules(procHandle, &mod, sizeof(HMODULE), &cbNeeded))
        {
            if(GetModuleBaseName(procHandle, mod, processName, MAX_PATH))
            {
                printf("PID %ld : %s\n",dwProcessId, processName);
                insertToList(list, dwProcessId, processName);
                nb_pids++;
            }
        }
        CloseHandle(procHandle);
    }
    printf("Number of PIDs %d\n",nb_pids);
}

BOOL compareLists(ProcessList *list, ProcessList *list2, int *nbpids_not_in2)
{
    Process *currentProc1;
    Process *currentProc2;
    BOOL found = FALSE;
    BOOL same = TRUE;

    *nbpids_not_in2 = 0;

    if(list != NULL && list2 != NULL)
    {
        currentProc1 = list->first;
        while(currentProc1 != NULL)
        {
            currentProc2 = list2->first;
            found = FALSE;
            while(currentProc2 != NULL)
            {
                if(currentProc2->pid == currentProc1->pid)
                {
                    found = TRUE;
                    break;
                }
                currentProc2 = currentProc2->next;
            }
            if(!found)
            {
                printf("PID %ld %s not in list 2\n", currentProc1->pid, currentProc1->exeName);
                (*nbpids_not_in2)++;
                same = FALSE;
            }
            currentProc1 = currentProc1->next;
        }
    }
    return same;
}

void SetDebugPrivilege()
{
    //Declaration des variables
    TOKEN_PRIVILEGES privilege;
    LUID Luid;
    HANDLE handle1;
    HANDLE handle2;

    // On modifie son PROPRE token pour avoir le droit de sortir de notre session,
    // pour se faire, il faut d'avoir ouvrir notre propore processus
    handle1 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    // Ensuite, on modifie notre token
    OpenProcessToken(handle1, TOKEN_ALL_ACCESS, &handle2);

    // SE_DEBUG_NAME est le privilege que l'on veut activer. Même si on est admin, il n'est pas active par défaut
    // comme on l'a vu dans Process Explorer
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid);
    privilege.PrivilegeCount = 1;
    privilege.Privileges[0].Luid = Luid;
    privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(handle2, FALSE, &privilege, sizeof(privilege), NULL, NULL);
    CloseHandle(handle2);
    CloseHandle(handle1);
}

int main()
{
    ProcessList *list = initList();
    ProcessList *list2 = initList();
    int nbpids_not_in2 = 0;
    printf("TaskManager\n");
    SetDebugPrivilege();
    getProcesses(list);
    listAllOpenProcesses(list2);
    if(!compareLists(list, list2, &nbpids_not_in2))
    {
        printf("%d PIDs in list from snapshot, not in list of open processes\n",nbpids_not_in2);
    }
    if(!compareLists(list2, list, &nbpids_not_in2))
    {
        printf("%d PIDs in list of open processes, not in list from snapshot\n",nbpids_not_in2);
    }

    cleanList(list);
    cleanList(list2);
    system("pause");
    return 0;
}
