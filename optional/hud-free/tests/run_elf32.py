import sys
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_INTR
from unicorn.x86_const import *
u=Uc(UC_ARCH_X86,UC_MODE_32);pages=set()
with open(sys.argv[1],'rb') as f:
 e=ELFFile(f);entry=e.header.e_entry
 for s in e.iter_segments():
  if s.header.p_type!='PT_LOAD':continue
  start=s.header.p_vaddr;end=start+s.header.p_memsz
  for p in range(start&~4095,(end+4095)&~4095,4096):
   if p not in pages:u.mem_map(p,4096);pages.add(p)
  u.mem_write(start,s.data())
u.mem_map(0x70000000,0x800000);u.reg_write(UC_X86_REG_ESP,0x707ffff0)
status=None
def intr(u,n,data):
 global status
 assert n==128,n
 op=u.reg_read(UC_X86_REG_EAX)
 if op==1:status=u.reg_read(UC_X86_REG_EBX);u.emu_stop()
 elif op==4:
  size=u.reg_read(UC_X86_REG_EDX);sys.stdout.buffer.write(u.mem_read(u.reg_read(UC_X86_REG_ECX),size));u.reg_write(UC_X86_REG_EAX,size)
 else:raise RuntimeError(op)
u.hook_add(UC_HOOK_INTR,intr);u.emu_start(entry,0,count=100000000)
assert status is not None,'instruction limit';sys.exit(status)
