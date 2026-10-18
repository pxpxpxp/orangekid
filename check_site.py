from pathlib import Path
from html.parser import HTMLParser
from urllib.parse import urlsplit
from PIL import Image
r=Path(__file__).resolve().parent/'docs'
class Check(HTMLParser):
 def __init__(self):super().__init__();self.ids=[];self.refs=[];self.sections=[];self.alt=[]
 def handle_starttag(self,tag,attrs):
  a=dict(attrs)
  if 'id' in a:self.ids.append(a['id'])
  if tag=='section' and 'chapter' in a.get('class',''):self.sections.append(a['id'])
  if tag=='img':assert a.get('alt'),a;self.alt.append(a['src'])
  for k in ['href','src']:
   if k in a:self.refs.append(a[k])
p=Check();p.feed((r/'index.html').read_text());assert len(p.ids)==len(set(p.ids));assert len(p.sections)==10
for ref in p.refs:
 if ref.startswith('#'):assert ref[1:] in p.ids,ref
 elif not urlsplit(ref).scheme:assert (r/ref).is_file(),ref
for im in p.alt:Image.open(r/im).verify()
assert '<title>Zelda 64: Orange Kid' in (r/'index.html').read_text()
print(f'PASS: 10 chapters, unique IDs, all links/assets/downloads resolve, {len(p.alt)} images readable with alt text')
