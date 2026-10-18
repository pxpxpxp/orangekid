#include "gfx.h"
#include "view.h"
#include "regs.h"
#include "buffers.h"
#include "stack_pad.h"
#include "play_state.h"
extern int printf(const char*,...);extern void abort(void);
#define CHECK(x) do {if(!(x)){printf("FAIL %d: %s\n",__LINE__,#x);abort();}}while(0)
static RegEditor editor;RegEditor* gRegEditor=&editor;
s32 gScreenWidth=320,gScreenHeight=240,gTransitionTileState;
u16 gZBuffer[H2X_FB_HEIGHT][H2X_FB_WIDTH];
static Gfx sFillSetupDL[1];
static int authoredBars;
s32 Letterbox_GetSize(void){return authoredBars;}
#include "fullscreen_current.inc"
static GraphicsContext ctx;static View view;static Gfx opa[256],xlu[256],overlay[256];
int main(void){int bars,i,fills,total=0;Gfx expected[1];
 for(bars=0;bars<=120;bars++){
  authoredBars=bars;ctx.polyOpa.p=opa;ctx.polyXlu.p=xlu;ctx.overlay.p=overlay;
  view.gfxCtx=&ctx;view.viewport.leftX=0;view.viewport.topY=0;view.viewport.rightX=320;view.viewport.bottomY=240;
  View_ApplyLetterbox(&view);gDPSetScissor(expected,G_SC_NON_INTERLACE,0,0,320,240);
  CHECK(opa[1].words.w0==expected[0].words.w0&&opa[1].words.w1==expected[0].words.w1);
  CHECK(xlu[1].words.w0==expected[0].words.w0&&xlu[1].words.w1==expected[0].words.w1);CHECK(authoredBars==bars);
  ctx.polyOpa.p=opa;ctx.polyXlu.p=xlu;ctx.overlay.p=overlay;
  Gfx_SetupFrame(&ctx,1,2,3);gDPFillRectangle(expected,0,0,319,239);
  for(i=0,fills=0;opa+i<ctx.polyOpa.p;i++)if((opa[i].words.w0>>24)==G_FILLRECT){CHECK(opa[i].words.w0==expected[0].words.w0&&opa[i].words.w1==expected[0].words.w1);fills++;}
  CHECK(fills==2);for(i=0;overlay+i<ctx.overlay.p;i++)CHECK((overlay[i].words.w0>>24)!=G_FILLRECT);CHECK(authoredBars==bars);total++;
 }
 printf("PASS fullscreen: 121 authored letterbox sizes; full 640x480 scissors and color/depth clears; no bar draws; authored state retained\n");
 return 0;}
