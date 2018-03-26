asmlinkage int (*real_open)(const char* __user, int, int);

unsigned long* sys_call_table = (unsigned long*) 0xffffffff8164f400;


asmlinkage int custom_open(const char* __user file_name, int flags, int mode)
{


}

int make_rw(unsigned long address)
{
  unsigned int level;
  pte_t *pte = lookup_address(address, &level);
  if(pte->pte &~ _PAGE_RW)
    pte->pte |= _PAGE_RW;
  return 0;
}

/* Make the page write protected */
int make_ro(unsigned long address)
{
  unsigned int level;
  pte_t *pte = lookup_address(address, &level);
  pte->pte = pte->pte &~ _PAGE_RW;
  return 0;
}



IOCTL_START:
         make_rw((unsigned long)sys_call_table);
         real_open = (void*)*(sys_call_table + __NR_open);
         *(sys_call_table + __NR_open) = (unsigned long)custom_open;
         make_ro((unsigned long)sys_call_table);

IOCTL_STOP:         
	make_rw((unsigned long)sys_call_table);
         *(sys_call_table + __NR_open) = (unsigned long)real_open;
         make_ro((unsigned long)sys_call_table)

