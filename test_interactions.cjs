const fs=require('fs'),vm=require('vm'),assert=require('assert');
class Element{constructor(id){this.id=id;this.hidden=false;this.attributes={};this.events={};this.classList={values:new Set(),toggle:(k,v)=>v?this.classList.values.add(k):this.classList.values.delete(k)};this.style={setProperty:(k,v)=>this.style[k]=v};this.dataset={};this.textContent='';}setAttribute(k,v){this.attributes[k]=v}removeAttribute(k){delete this.attributes[k]}addEventListener(k,f){this.events[k]=f}scrollIntoView(){}pause(){this.paused=true}}
const ids=['vision','redux','text','hud','world','play','mister','performance','download'],chapters=ids.map(id=>new Element(id)),links=ids.map(id=>new Element(id));
const elements={};for(const id of ['prev-chapter','next-chapter','page-count','text-slider','copy-hash','base-hash','copy-status','orange-capture','stock-capture'])elements[id]=new Element(id);elements['base-hash'].textContent='5bd1fe107bf8106b2ab6650abecd54d6';const comparison=new Element('comparison');elements['text-slider'].closest=()=>comparison;
const buttons=['50','100','0'].map(value=>{const x=new Element(value);x.dataset.value=value;return x}),captureButtons=['detail','scene'].map(mode=>{const x=new Element(mode);x.dataset.mode=mode;return x}),audio=[new Element('audio1'),new Element('audio2')],events={};
const context={document:{querySelectorAll:q=>({'.chapter':chapters,'nav [data-page]':links,'audio':audio,'.text-view':buttons,'.capture-view':captureButtons})[q],getElementById:id=>elements[id],title:''},location:{hash:''},window:{addEventListener:(k,f)=>events[k]=f,scrollTo(){}},navigator:{clipboard:{writeText:async()=>{}}}};
vm.createContext(context);vm.runInContext(fs.readFileSync('docs/app.js','utf8'),context);
for(let i=0;i<ids.length;i++){context.location.hash='#'+ids[i];events.hashchange();assert.deepStrictEqual(chapters.map(x=>x.hidden),ids.map((_,j)=>j!==i));assert.equal(links[i].attributes['aria-current'],'page');assert.equal(elements['page-count'].textContent,`${String(i+1).padStart(2,'0')} / 09`);}
context.location.hash='#unknown';events.hashchange();assert.equal(chapters[0].hidden,false);
for(const button of buttons){button.events.click();assert.equal(comparison.style['--split'],button.dataset.value+'%');}
audio[0].events.play();assert.equal(audio[1].paused,true);
console.log('PASS: all chapter routes and unknown-hash fallback, comparison presets, and exclusive audio playback');

for(const b of captureButtons){b.events.click();assert.equal(elements['orange-capture'].src,'assets/dialogue-orange'+(b.dataset.mode==='scene'?'':'-detail')+'.png');assert.equal(b.attributes['aria-pressed'],'true');}console.log('PASS: native scene/detail capture switching');
