#!/usr/bin/env python3
"""Execute source-derived HUD/progression/save tests. Rendering, audio and I/O use stubs.
Requires gcc with -m32 object support, Python unicorn and pyelftools; no 32-bit libc.
"""
from pathlib import Path
import argparse, re, tempfile, subprocess, sys, shutil, json, hashlib

def definition(text, name):
    match=re.search(r'^([^\n;]*\b'+re.escape(name)+r'\s*\([^;]*?\)\s*\{)',text,re.M)
    if not match: raise ValueError('Function definition not found: '+name)
    start=match.start(); depth=0
    tokens=re.finditer(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]',text[match.start()+len(match.group(1))-1:],re.S)
    begin=match.start()+len(match.group(1))-1
    for token in tokens:
        if token.group()=='{':depth+=1
        elif token.group()=='}':
            depth-=1
            if depth==0:return text[start:begin+token.end()]
    raise ValueError('Unbalanced function: '+name)

def array(text,name):
    match=re.search(r'^[^\n]*\b'+name+r'\[[^=]*=\s*\{.*?^};',text,re.M|re.S)
    if not match:raise ValueError('Array not found: '+name)
    return match.group()

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('source',type=Path);ap.add_argument('--report',type=Path);ap.add_argument('--mutations',action='store_true');args=ap.parse_args()
    src=args.source.resolve();here=Path(__file__).resolve().parent
    parameter=(src/'src/code/z_parameter.c').read_text();sram=(src/'src/code/z_sram.c').read_text();inventory=(src/'src/code/z_inventory.c').read_text()
    function=definition(parameter,'Interface_Draw')
    report={'harness':'32-bit host C function tests, graphics/audio/I/O dependencies stubbed','source_sha256':hashlib.sha256((src/'src/code/z_parameter.c').read_bytes()).hexdigest(),'full_playthrough_verified':False,'mutations':{}}
    with tempfile.TemporaryDirectory(prefix='orangekid-regression-') as temp:
        temp=Path(temp)
        for name in ['interface_test.c','save_test.inc','runtime32.c','start32.S','run_elf32.py']:shutil.copyfile(here/name,temp/name)
        (temp/'interface_headers.inc').write_text(parameter[:parameter.index('#pragma increment')])
        (temp/'interface_current.inc').write_text(function)
        (temp/'sram_current.inc').write_text(array(sram,'sDungeonEntrances')+'\n'+definition(sram,'Sram_OpenSave')+'\n'+definition(sram,'Sram_WriteSave'))
        (temp/'sram_slot_offsets.inc').write_text(array(sram,'gSramSlotOffsets'))
        (temp/'inventory_tables.inc').write_text('\n'.join(array(inventory,n) for n in ['gUpgradeMasks','gUpgradeShifts','gUpgradeCapacities','gItemSlots','gEquipShifts','gBitFlags']))
        flags=['-m32','-malign-double','-O1','-ffreestanding','-fno-builtin','-fno-pie','-no-pie','-nostdinc','-nostdlib','-static','-ffunction-sections','-fdata-sections','-Wl,--gc-sections']
        defines=['_LANGUAGE_C','F3DEX_GBI_2','PLATFORM_N64=1','PLATFORM_GC=0','PLATFORM_IQUE=0','OOT_VERSION=NTSC_1_0','OOT_REGION=REGION_US','DEBUG_FEATURES=0','NON_MATCHING','AVOID_UB']
        compile=['gcc',*flags,*['-D'+d for d in defines],*['-I'+str(src/p) for p in ['include','include/libc','.','build/ntsc-1.0','extracted/ntsc-1.0']],str(temp/'interface_test.c'),str(temp/'runtime32.c'),str(temp/'start32.S'),'-o',str(temp/'tests')]
        def run(body):
            (temp/'interface_current.inc').write_text(body)
            built=subprocess.run(compile,capture_output=True,text=True)
            if built.returncode:raise RuntimeError(built.stderr)
            return subprocess.run([sys.executable,str(temp/'run_elf32.py'),str(temp/'tests')],capture_output=True,text=True)
        baseline=run(function);print(baseline.stdout,end='');report['functional_cases']=baseline.stdout.splitlines();report['passed']=baseline.returncode==0
        if baseline.returncode:raise RuntimeError(baseline.stderr+baseline.stdout)
        if args.mutations:
            variants={'early_return':function.replace('{','{ return;',1),'frozen_timer':function.replace('gSaveContext.timerSeconds--;','/* frozen timer */',1),'broken_trade_revert':function.replace('INV_CONTENT(gSpoilingItemReverts[svar1]) = gSpoilingItemReverts[svar1];','/* no revert */',1)}
            for name,body in variants.items():
                assert body!=function,(name,'mutation not applied')
                result=run(body);caught=result.returncode!=0 and 'FAIL line' in result.stdout
                report['mutations'][name]={'detected':caught,'failure':result.stdout.splitlines()[-1] if result.stdout else result.stderr}
                assert caught,(name,result.stdout,result.stderr)
                print('PASS mutation rejected:',name)
    if args.report:args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2))

if __name__=='__main__':main()
