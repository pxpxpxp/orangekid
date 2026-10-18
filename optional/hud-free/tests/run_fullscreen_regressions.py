#!/usr/bin/env python3
"""Test the current fullscreen View_ApplyLetterbox and Gfx_SetupFrame bodies."""
from pathlib import Path
import argparse,tempfile,subprocess,sys,shutil,json
from run_regressions import definition

def main():
 ap=argparse.ArgumentParser();ap.add_argument('source',type=Path);ap.add_argument('--report',type=Path);args=ap.parse_args();src=args.source.resolve();here=Path(__file__).resolve().parent
 view=definition((src/'src/code/z_view.c').read_text(),'View_ApplyLetterbox');frame=definition((src/'src/code/z_rcp.c').read_text(),'Gfx_SetupFrame')
 result={'scope':'32-bit host function test; no emulator gameplay','cases':121,'mutations':{}}
 with tempfile.TemporaryDirectory(prefix='hudfree-fullscreen-') as temp:
  temp=Path(temp)
  for name in ['fullscreen_test.c','runtime32.c','start32.S','run_elf32.py']:shutil.copyfile(here/name,temp/name)
  flags=['-m32','-malign-double','-O1','-ffreestanding','-fno-builtin','-fno-pie','-no-pie','-nostdinc','-nostdlib','-static']
  defs=['_LANGUAGE_C','F3DEX_GBI_2','PLATFORM_N64=1','PLATFORM_GC=0','PLATFORM_IQUE=0','OOT_VERSION=NTSC_1_0','OOT_REGION=REGION_US','DEBUG_FEATURES=0','NON_MATCHING','AVOID_UB','NDEBUG']
  def run(a,b):
   (temp/'fullscreen_current.inc').write_text(a+'\n'+b)
   cmd=['gcc',*flags,*['-D'+x for x in defs],*['-I'+str(src/p) for p in ['include','include/libc','.']],str(temp/'fullscreen_test.c'),str(temp/'runtime32.c'),str(temp/'start32.S'),'-o',str(temp/'test')]
   r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stderr
   return subprocess.run([sys.executable,str(temp/'run_elf32.py'),str(temp/'test')],capture_output=True,text=True)
  r=run(view,frame);assert r.returncode==0,r.stdout+r.stderr;print(r.stdout,end='');result['passed']=True
  for name,a,b in [('clipped_viewport',view.replace('letterboxSize = 0;','letterboxSize = Letterbox_GetSize();',1),frame),('drawn_bars',view,frame.replace('letterboxSize = 0;','/* injected missing fullscreen override */',1))]:
   r=run(a,b);assert r.returncode!=0 and 'FAIL' in r.stdout,(name,r.stdout,r.stderr);result['mutations'][name]={'detected':True,'failure':r.stdout.strip()};print('PASS mutation rejected:',name)
 if args.report:args.report.write_text(json.dumps(result,indent=2))
if __name__=='__main__':main()
