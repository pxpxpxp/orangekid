'use strict';
const chapters=[...document.querySelectorAll('.chapter')];
const links=[...document.querySelectorAll('nav [data-page]')];
const titles=['The vision','Redux additions','A clearer conversation','HUD & menus','Original Hyrule','Play & sound','Targeting MiSTer','Performance & fixes','Choose your version','About Orange Kid'];
function showChapter(){
 let id=location.hash.slice(1)||'vision';let index=chapters.findIndex(el=>el.id===id);if(index<0){index=0;id='vision';}
 chapters.forEach((el,i)=>{el.hidden=i!==index;});
 links.forEach((el,i)=>{el.classList.toggle('active',i===index);if(i===index)el.setAttribute('aria-current','page');else el.removeAttribute('aria-current');});
 const prev=document.getElementById('prev-chapter'),next=document.getElementById('next-chapter');
 prev.setAttribute('aria-disabled',String(index===0));prev.href='#'+chapters[Math.max(0,index-1)].id;
 next.setAttribute('aria-disabled',String(index===chapters.length-1));next.href='#'+chapters[Math.min(chapters.length-1,index+1)].id;next.textContent=(titles[index+1]||'')+' →';
 document.getElementById('page-count').textContent=String(index+1).padStart(2,'0')+' / '+String(chapters.length).padStart(2,'0');document.title='Zelda 64: Orange Kid | '+titles[index];
 document.querySelectorAll('audio').forEach(el=>el.pause());
 window.scrollTo({top:0,behavior:'instant'});
 const active=links[index];active.scrollIntoView({block:'nearest',inline:'nearest',behavior:'instant'});
}
window.addEventListener('hashchange',showChapter);showChapter();
const slider=document.getElementById('text-slider');
function setSplit(value){slider.value=value;slider.closest('.comparison').style.setProperty('--split',value+'%');slider.setAttribute('aria-valuetext',value+' percent original text revealed');document.querySelectorAll('.text-view').forEach(button=>button.classList.toggle('active',button.dataset.value===String(value)));}
slider.addEventListener('input',()=>setSplit(slider.value));document.querySelectorAll('.text-view').forEach(button=>button.addEventListener('click',()=>setSplit(button.dataset.value)));setSplit(50);
document.querySelectorAll('audio').forEach(audio=>audio.addEventListener('play',()=>document.querySelectorAll('audio').forEach(other=>{if(other!==audio)other.pause();})));
document.getElementById('copy-hash').addEventListener('click',async()=>{const text=document.getElementById('base-hash').textContent;const status=document.getElementById('copy-status');try{await navigator.clipboard.writeText(text);status.textContent='Base ROM MD5 copied.';}catch{status.textContent='Select the MD5 above and copy it manually.';const range=document.createRange();range.selectNodeContents(document.getElementById('base-hash'));const selection=window.getSelection();selection.removeAllRanges();selection.addRange(range);}});

function setCaptureMode(mode){const scene=mode==='scene';slider.closest('.comparison').classList.toggle('scene',scene);for(const [id,font] of [['orange-capture','orange'],['stock-capture','original']]){const img=document.getElementById(id);img.src='assets/dialogue-'+font+(scene?'':'-detail')+'.png';img.width=scene?640:400;img.height=scene?480:88;}document.querySelectorAll('.capture-view').forEach(button=>{const active=button.dataset.mode===mode;button.classList.toggle('active',active);button.setAttribute('aria-pressed',String(active));});}document.querySelectorAll('.capture-view').forEach(button=>button.addEventListener('click',()=>setCaptureMode(button.dataset.mode)));
