#include "interface_headers.inc"
extern void* memset(void*,int,unsigned);
extern void* memcpy(void*,const void*,unsigned);
extern void abort(void);
extern int printf(const char*,...);
#undef ASSERT
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n",__LINE__,#x); abort(); } } while(0)
SaveContext gSaveContext;
RegEditor sInfo;
RegEditor* gRegEditor = &sInfo;
s32 gScreenWidth=320,gScreenHeight=240;
Mtx gIdentityMtx;
s16 gSpoilingItems[]={ITEM_ODD_MUSHROOM,ITEM_EYEBALL_FROG,ITEM_EYE_DROPS};
s16 gSpoilingItemReverts[]={ITEM_COJIRO,ITEM_PRESCRIPTION,ITEM_PRESCRIPTION};
static s16 sEnvHazardActive;
static s16 sHBAScoreDigits[4];
static Gfx sSetupDL_80125A60[1];
static int sounds,texts,damages,icons,actions,msgState,switchFlag,csMode;
static PlayState play;
static Player player;
static GraphicsContext gfx;
static Gfx overlay[0x1800];
static Gfx opa[0x10000];
static Vtx actionVtx[16];
static void emit(GraphicsContext* g,unsigned tag) {g->overlay.p->words.w0=0;g->overlay.p->words.w1=tag;g->overlay.p++;}
void Interface_InitVertices(PlayState* p){p->interfaceCtx.actionVtx=actionVtx;}
void func_8008A994(InterfaceContext* p){}
void Health_DrawMeter(PlayState* p){emit(p->state.gfxCtx,0x1111);}
void Magic_DrawMeter(PlayState* p){emit(p->state.gfxCtx,0x2222);}
void Minimap_Draw(PlayState* p){emit(p->state.gfxCtx,0x3333);}
void Attention_Draw(Attention* a,PlayState* p){emit(p->state.gfxCtx,0x4444);}
void Interface_DrawItemButtons(PlayState* p){emit(p->state.gfxCtx,0x5555);}
void Interface_DrawItemIconTexture(PlayState* p,void* t,s16 b){emit(p->state.gfxCtx,0x6666);}
void Interface_DrawAmmoCount(PlayState* p,s16 b,s16 a){}
void func_8008A8B8(PlayState* p,s32 a,s32 b,s32 c,s32 d){}
void Interface_DrawActionButton(PlayState* p){}
void Interface_DrawActionLabel(GraphicsContext* g,void* t){emit(g,0x7777);}
void Gfx_SetupDL_39Overlay(GraphicsContext* g){}
void Gfx_SetupDL_42Overlay(GraphicsContext* g){}
Gfx* Gfx_TextureIA8(Gfx* g,void* t,s16 w,s16 h,s16 x,s16 y,s16 rw,s16 rh,u16 s,u16 v){g->words.w0=0;g->words.w1=0x8888;return g+1;}
Gfx* Gfx_TextureI8(Gfx* g,void* t,s16 w,s16 h,s16 x,s16 y,s16 rw,s16 rh,u16 s,u16 v){g->words.w0=0;g->words.w1=0x9999;return g+1;}
void Matrix_Translate(f32 x,f32 y,f32 z,u8 m){}
void Matrix_Scale(f32 x,f32 y,f32 z,u8 m){}
void Matrix_RotateX(f32 x,u8 m){}
Mtx* Matrix_NewMtx(GraphicsContext* g,char* f,s32 l){return &gIdentityMtx;}
int Play_InCsMode(PlayState* p){return csMode;}
s32 Flags_GetSwitch(PlayState* p,s32 f){return switchFlag;}
u8 Message_GetState(MessageContext* m){return msgState;}
void Message_StartTextbox(PlayState* p,u16 t,Actor* a){texts++;}
s32 Player_SetCsActionWithHaltedActors(PlayState* p,Actor* a,u8 c){actions++;return 1;}
void Interface_LoadItemIcon1(PlayState* p,u16 b){icons++;}
void Sfx_PlaySfxCentered(u16 id){sounds++;}
static s32 damage(PlayState* p,s32 n){damages++;return 1;}
Vec3f gSfxDefaultPos;
f32 gSfxDefaultFreqAndVolScale=1;
s8 gSfxDefaultReverb;
void Audio_PlaySfxGeneral(u16 id,Vec3f* p,u8 token,f32* f,f32* v,s8* r){sounds++;}
u64 gArcheryScoreIconTex[96],gCarrotIconTex[128],gMagicArrowEquipEffectTex[128];
void* gItemIcons[0x82];
Mtx* Matrix_Finalize(GraphicsContext* g){return &gIdentityMtx;}
#include "inventory_tables.inc"
#include "interface_current.inc"
static void reset(void){
 memset(&play,0,sizeof(play));memset(&player,0,sizeof(player));memset(&gSaveContext,0,sizeof(gSaveContext));memset(&sInfo,0,sizeof(sInfo));
 play.state.gfxCtx=&gfx;play.actorCtx.actorLists[ACTORCAT_PLAYER].head=&player.actor;play.damagePlayer=damage;
 play.sceneId=SCENE_HYRULE_FIELD;gSaveContext.save.info.playerData.health=0x30;
 gSaveContext.save.info.playerData.healthCapacity=0x30;
 gSaveContext.save.info.equips.buttonItems[0]=ITEM_NONE;gSaveContext.save.info.equips.buttonItems[1]=ITEM_NONE;gSaveContext.save.info.equips.buttonItems[2]=ITEM_NONE;gSaveContext.save.info.equips.buttonItems[3]=ITEM_NONE;
 sounds=texts=damages=icons=actions=0;msgState=TEXT_STATE_NONE;switchFlag=csMode=sEnvHazardActive=0;
}
static void frame(void){gfx.overlay.p=overlay;gfx.polyOpa.d=opa+0x10000;Interface_Draw(&play);CHECK(gfx.overlay.p>=overlay&&gfx.overlay.p<overlay+0x1800);}
static void frames(int n){while(n--)frame();}
static int tag(unsigned t){Gfx* p;for(p=overlay;p<gfx.overlay.p;p++)if(p->words.w0==0&&p->words.w1==t)return 1;return 0;}
#include "save_test.inc"
int main(void){int i,j;
 saveTests();
 reset();frame();CHECK(!tag(0x1111)&&!tag(0x2222)&&!tag(0x3333)&&!tag(0x5555)&&!tag(0x7777));CHECK(tag(0x4444));
 play.pauseCtx.state=PAUSE_STATE_MAIN;frame();CHECK(tag(0x1111)&&tag(0x5555));printf("PASS HUD suppressed in gameplay; targeting retained; pause HUD retained\n");
 reset();gSaveContext.timerState=TIMER_STATE_DOWN_INIT;gSaveContext.timerSeconds=3;frames(120);CHECK(gSaveContext.timerSeconds==0);CHECK(gSaveContext.timerState==TIMER_STATE_OFF);printf("PASS countdown progresses and expires\n");
 reset();gSaveContext.timerState=TIMER_STATE_UP_INIT;frames(100);CHECK(gSaveContext.timerSeconds>=3);printf("PASS count-up timer advances\n");
 reset();gSaveContext.timerState=TIMER_STATE_ENV_HAZARD_INIT;sEnvHazardActive=1;gSaveContext.save.info.playerData.health=2;frames(100);CHECK(damages==1);CHECK(gSaveContext.save.info.playerData.health==0);printf("PASS environmental hazard expiration applies damage\n");
 reset();gSaveContext.timerState=TIMER_STATE_DOWN_INIT;gSaveContext.timerSeconds=3;frames(42);i=gSaveContext.timerSeconds;play.pauseCtx.state=PAUSE_STATE_MAIN;frames(100);CHECK(gSaveContext.timerSeconds==i);play.pauseCtx.state=PAUSE_STATE_OFF;frames(100);CHECK(gSaveContext.timerSeconds==0);printf("PASS pause suspends timer; resume continues\n");
 for(j=0;j<8;j++){
  reset();gSaveContext.timerState=TIMER_STATE_DOWN_INIT;gSaveContext.timerSeconds=5;frames(42);i=gSaveContext.timerSeconds;
  switch(j){case 0:play.gameOverCtx.state=GAMEOVER_DEATH_START;break;case 1:play.msgCtx.msgMode=MSGMODE_TEXT_DISPLAYING;break;case 2:player.stateFlags2|=PLAYER_STATE2_24;break;case 3:play.transitionTrigger=TRANS_TRIGGER_START;break;case 4:play.transitionMode=1;break;case 5:csMode=1;break;case 6:gSaveContext.minigameState=1;break;case 7:play.shootingGalleryStatus=2;break;}
  frames(100);CHECK(gSaveContext.timerSeconds==i);
 }
 printf("PASS timers respect all 8 dialogue/game-over/transition/cutscene/minigame gates\n");
 for(j=0;j<3;j++){
  reset();gSaveContext.subTimerState=SUBTIMER_STATE_DOWN_INIT;gSaveContext.subTimerSeconds=1;
  gSaveContext.save.info.inventory.items[SLOT_TRADE_ADULT]=gSpoilingItems[j];gSaveContext.save.info.equips.buttonItems[1]=gSpoilingItems[j];
  frames(100);CHECK(texts==1&&actions==1);CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_RESPAWN);
  msgState=TEXT_STATE_EVENT;frame();CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_OFF);CHECK(play.transitionTrigger==TRANS_TRIGGER_START);
  CHECK(gSaveContext.save.info.inventory.items[SLOT_TRADE_ADULT]==gSpoilingItemReverts[j]);CHECK(gSaveContext.save.info.equips.buttonItems[1]==gSpoilingItemReverts[j]);CHECK(icons==1);
 }
 printf("PASS all 3 timed trade items expire, revert, restore equipped item and respawn\n");
 for(j=0;j<4;j++){
  static s16 scenes[]={SCENE_GANON_BOSS,SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR,SCENE_GANONS_TOWER_COLLAPSE_INTERIOR,SCENE_INSIDE_GANONS_CASTLE_COLLAPSE};
  reset();play.sceneId=scenes[j];switchFlag=1;gSaveContext.subTimerState=SUBTIMER_STATE_DOWN_INIT;gSaveContext.subTimerSeconds=1;frames(100);CHECK(texts==0);CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_OFF);
  reset();play.sceneId=scenes[j];gSaveContext.subTimerState=SUBTIMER_STATE_DOWN_INIT;gSaveContext.subTimerSeconds=1;frames(100);CHECK(texts==1);CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_RESPAWN);
 }
 printf("PASS final escape timeout/switch handling in all 4 endgame scenes\n");
 reset();gSaveContext.timerState=TIMER_STATE_DOWN_INIT;gSaveContext.timerSeconds=2;gSaveContext.subTimerState=SUBTIMER_STATE_DOWN_INIT;gSaveContext.subTimerSeconds=2;frames(240);CHECK(gSaveContext.timerState==TIMER_STATE_OFF);CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_RESPAWN);CHECK(gSaveContext.timerSeconds==0&&gSaveContext.subTimerSeconds==0);CHECK(texts==1);printf("PASS main timer hands off to queued sub-timer without freezing\n");
 reset();SET_EVENTINF(EVENTINF_MARATHON_ACTIVE);gSaveContext.subTimerState=SUBTIMER_STATE_UP_INIT;gSaveContext.subTimerSeconds=MARATHON_TIME_LIMIT-1;frames(100);CHECK(!GET_EVENTINF(EVENTINF_MARATHON_ACTIVE));CHECK(gSaveContext.subTimerState==SUBTIMER_STATE_OFF);CHECK(texts==1);printf("PASS running-man race cutoff clears event flag\n");
 reset();play.interfaceCtx.unk_244=255;frame();for(i=0,j=0;overlay+i<gfx.overlay.p;i++)if((overlay[i].words.w0>>24)==G_FILLRECT)j++;CHECK(j==1);printf("PASS interface fade command retained\n");
 return 0;
}
