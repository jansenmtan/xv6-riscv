#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "elf.h"

int aslr_flag = 1;

//add implementation for function get_random_min_max (min, max)
unsigned int g_random_seed = 0;

int get_random_min_max(int min,int max) 
{
  int randomNum = 0;
 // use g_random_seed as seed
 if(!g_random_seed)
	 g_random_seed = 0xF4123;
 
 // logic to create a number between min and max values passed using g_random_seed
 // and assign it to randomNum
 randomNum = (g_random_seed % (max-min)) + min;
 return randomNum;
}


static int loadseg(pde_t *pgdir, uint64 addr, struct inode *ip, uint offset, uint sz);

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint64 argc, sz = 0, sp, ustack[MAXARG+1], stackbase;
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pagetable_t pagetable = 0, oldpagetable;
  struct proc *p = myproc();
  
  //add flag to turn aslr on/off

  begin_op();

  if((ip = namei(path)) == 0){
    end_op();
    return -1;
  }
  ilock(ip);

  // Check ELF header
  if(readi(ip, 0, (uint64)&elf, 0, sizeof(elf)) != sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pagetable = proc_pagetable(p)) == 0)
    goto bad;

  //If aslr flag is on, create variable base_pointer_offset and assign it value returned from get_random_min_max(0,16), if flag is off then assign 0
  int base_pointer_offset = 0;
  if(aslr_flag)
	  base_pointer_offset = get_random_min_max(0, 16);
  
  //If aslr flag is on, create variable text_seg_pages_offset and assign its value as base_pointer_offset * PGSIZE, else assign 0
  int text_seg_pages_offset = 0;
  if(aslr_flag)
	  text_seg_pages_offset = base_pointer_offset * PGSIZE;
	

  // Load program into memory.
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if(ph.vaddr + ph.memsz < ph.vaddr)
      goto bad;
    uint64 sz1;

	// add text_seg_pages_offset to newsz parameter of uvmalloc to allocate additional space
    if((sz1 = uvmalloc(pagetable, sz, ph.vaddr + ph.memsz + text_seg_pages_offset)) == 0)
      goto bad;
    sz = sz1;
    if(ph.vaddr % PGSIZE != 0)
      goto bad;

	//add text_seg_pages_offset to ph.vaddr to include offset while loading pagetable
    if(loadseg(pagetable, ph.vaddr + text_seg_pages_offset, ip, ph.off, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  end_op();
  ip = 0;

  p = myproc();
  uint64 oldsz = p->sz;

  // Allocate two pages at the next page boundary.
  // Use the second as the user stack.
  sz = PGROUNDUP(sz);
  uint64 sz1;

  //Change the heap address if aslr flag is set*/
  //create variable num_pages and initialise its value as 2
  int num_pages = 2;

  //replace 2 by variable num_pages in below uvmalloc statement
  // create variable add_pages if aslr flag is on and assign num_pages +  get_random_min_max(0,8) to it
  int add_pages = 0;
  if(aslr_flag)
	  add_pages = num_pages + get_random_min_max(0, 8);

  //add add_pages to num_pages and replace 2 with num_pages
  num_pages += add_pages;
  
  if((sz1 = uvmalloc(pagetable, sz, sz + num_pages*PGSIZE)) == 0) // all the above steps should be done before this call
    goto bad;
  sz = sz1;

  //replace 2 with num_pages to increase stack size
  uvmclear(pagetable, sz-num_pages*PGSIZE);

  //if aslr flag is on, create variable stack_offset and assign it value returned from get_random_min_max(0,16)
  int stack_offset = get_random_min_max(0, 16);

  //change stack pointer sp by substracting stack_offset*64 from sz
  sp = sz - stack_offset*64;
  //if aslr flag is on, substract num_pages * PGSIZE from sp and assign it to stackbase
  stackbase = sp - num_pages*PGSIZE;

  // Push argument strings, prepare rest of stack in ustack.
  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp -= strlen(argv[argc]) + 1;
    sp -= sp % 16; // riscv sp must be 16-byte aligned
    if(sp < stackbase)
      goto bad;
    if(copyout(pagetable, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[argc] = sp;
  }
  ustack[argc] = 0;

  // push the array of argv[] pointers.
  sp -= (argc+1) * sizeof(uint64);
  sp -= sp % 16;
  if(sp < stackbase)
    goto bad;
  if(copyout(pagetable, sp, (char *)ustack, (argc+1)*sizeof(uint64)) < 0)
    goto bad;

  // arguments to user main(argc, argv)
  // argc is returned via the system call return
  // value, which goes in a0.
  p->trapframe->a1 = sp;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(p->name, last, sizeof(p->name));
    
  // Commit to the user image.
  oldpagetable = p->pagetable;
  p->pagetable = pagetable;
  p->sz = sz;
  
  // add text_seg_pages_offset to elf.entry to change address of main
  elf.entry += text_seg_pages_offset;
  
  p->trapframe->epc = elf.entry;  // initial program counter = main
  p->trapframe->sp = sp; // initial stack pointer
  proc_freepagetable(oldpagetable, oldsz);

  return argc; // this ends up in a0, the first argument to main(argc, argv)

 bad:
  if(pagetable)
    proc_freepagetable(pagetable, sz);
  if(ip){
    iunlockput(ip);
    end_op();
  }
  return -1;
}

// Load a program segment into pagetable at virtual address va.
// va must be page-aligned
// and the pages from va to va+sz must already be mapped.
// Returns 0 on success, -1 on failure.
static int
loadseg(pagetable_t pagetable, uint64 va, struct inode *ip, uint offset, uint sz)
{
  uint i, n;
  uint64 pa;

  if((va % PGSIZE) != 0)
    panic("loadseg: va must be page aligned");

  for(i = 0; i < sz; i += PGSIZE){
    pa = walkaddr(pagetable, va + i);
    if(pa == 0)
      panic("loadseg: address should exist");
    if(sz - i < PGSIZE)
      n = sz - i;
    else
      n = PGSIZE;
    if(readi(ip, 0, (uint64)pa, offset+i, n) != n)
      return -1;
  }
  
  return 0;
}
