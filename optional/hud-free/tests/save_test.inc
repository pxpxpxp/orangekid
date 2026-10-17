#include "sram.h"
#include "memory_utils.h"
#include "ocarina.h"
static u8 saveBytes[0x8000];
static SramContext sram;
#define SLOT_SIZE (sizeof(SaveContext)+0x28)
#define SLOT_OFFSET(index) (SRAM_HEADER_SIZE+0x10+(index)*SLOT_SIZE)
#include "sram_slot_offsets.inc"
static OcarinaNote longSong[108];
static u8 spawnSong[128];
OcarinaNote* gScarecrowLongSongPtr=longSong;
u8* gScarecrowSpawnSongPtr=spawnSong;
void* MemCopy(void* d,void* s,s32 n){return memcpy(d,s,n);}
#define SLOT_SIZE (sizeof(SaveContext)+0x28)
#define CHECKSUM_SIZE (sizeof(Save)/2)
static u8 writtenSram[0x8000];
static int writes;
static void writeSram(u32 addr,void* d,u32 n){unsigned off=addr-0x08000000;CHECK(off+n<=sizeof(writtenSram));memcpy(writtenSram+off,d,n);writes++;}
#define SRAM_WRITE(a,d,n) writeSram(a,d,n)
#include "sram_current.inc"
static void saveTests(void){
 int age,j;u32 quest=0x1fffff;
 static s16 endScenes[]={SCENE_GANONDORF_BOSS,SCENE_GANON_BOSS,SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR,SCENE_GANONS_TOWER_COLLAPSE_INTERIOR,SCENE_INSIDE_GANONS_CASTLE_COLLAPSE};
 sram.readBuff=saveBytes;
 for(age=0;age<2;age++){
  for(j=0;j<14;j++){
   reset();gSaveContext.save.linkAge=age;gSaveContext.save.info.playerData.savedSceneId=j;
   gSaveContext.save.info.inventory.questItems=quest;gSaveContext.save.info.inventory.dungeonItems[j]=7;
   memcpy(saveBytes+gSramSlotOffsets[0],&gSaveContext,sizeof(Save));
   Sram_OpenSave(&sram);CHECK(gSaveContext.save.entranceIndex==sDungeonEntrances[j]);CHECK(gSaveContext.save.info.inventory.questItems==quest);CHECK(gSaveContext.save.info.inventory.dungeonItems[j]==7);
  }
  for(j=0;j<5;j++){
   reset();gSaveContext.save.linkAge=age;gSaveContext.save.info.playerData.savedSceneId=endScenes[j];
   memcpy(saveBytes+gSramSlotOffsets[0],&gSaveContext,sizeof(Save));Sram_OpenSave(&sram);CHECK(gSaveContext.save.entranceIndex==ENTR_GANONS_TOWER_0);
  }
  reset();gSaveContext.save.linkAge=age;gSaveContext.save.info.playerData.savedSceneId=SCENE_KOKIRI_FOREST;
  memcpy(saveBytes+gSramSlotOffsets[0],&gSaveContext,sizeof(Save));Sram_OpenSave(&sram);
  CHECK(gSaveContext.save.entranceIndex==(LINK_IS_ADULT?ENTR_TEMPLE_OF_TIME_7:ENTR_KOKIRI_FOREST_0));
 }
 printf("PASS actual save loader: 28 dungeon and 10 endgame age/scene cases; quest and dungeon flags preserved; child/adult forest return\n");
 reset();gSaveContext.save.info.playerData.savedSceneId=SCENE_LINKS_HOUSE;gSaveContext.save.info.playerData.health=1;
 SET_EVENTCHKINF(EVENTCHKINF_OBTAINED_ZELDAS_LETTER);
 gSaveContext.save.info.inventory.items[SLOT_TRADE_CHILD]=ITEM_ZELDAS_LETTER;gSaveContext.save.info.equips.buttonItems[2]=ITEM_ZELDAS_LETTER;
 memcpy(saveBytes+gSramSlotOffsets[0],&gSaveContext,sizeof(Save));Sram_OpenSave(&sram);
 CHECK(!GET_EVENTCHKINF(EVENTCHKINF_OBTAINED_ZELDAS_LETTER));CHECK(gSaveContext.save.info.playerData.health==0x30);CHECK(gSaveContext.save.info.equips.buttonItems[2]==ITEM_CHICKEN);
 printf("PASS Zelda letter/song recovery and minimum-health recovery after load\n");
 for(j=0;j<3;j++){
  unsigned k;u16 sum=0;reset();gSaveContext.fileNum=j;writes=0;gSaveContext.save.info.inventory.questItems=0x1fffff;gSaveContext.save.info.playerData.savedSceneId=SCENE_GANON_BOSS;
  Sram_WriteSave(&sram);CHECK(writes==2);
  for(k=0;k<SLOT_SIZE;k++)CHECK(writtenSram[gSramSlotOffsets[j]+k]==writtenSram[gSramSlotOffsets[j+3]+k]);
  gSaveContext.save.info.checksum.value=0;
  for(k=0;k<CHECKSUM_SIZE;k++)sum+=((u16*)&gSaveContext)[k];
  CHECK(((Save*)(writtenSram+gSramSlotOffsets[j]))->info.checksum.value==sum);
  memcpy(saveBytes,writtenSram,sizeof(saveBytes));Sram_OpenSave(&sram);CHECK(gSaveContext.save.info.inventory.questItems==0x1fffff);CHECK(gSaveContext.save.entranceIndex==ENTR_GANONS_TOWER_0);
 }
 printf("PASS all 3 save slots: actual checksum writer, identical main/backup writes and reload with endgame progress\n");
}
