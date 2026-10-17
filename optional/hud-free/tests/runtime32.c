/* Freestanding Linux/i386 harness: execute the game's headers with 32-bit long/pointers. */
static int write_bytes(const char* p,unsigned n){int result;__asm__ volatile("int $0x80":"=a"(result):"0"(4),"b"(1),"c"(p),"d"(n):"memory");return result;}
int fflush(void* p){return 0;}
void abort(void){__asm__ volatile("int $0x80"::"a"(1),"b"(3):"memory");__builtin_unreachable();}
int printf(const char* format,...){
 __builtin_va_list args;__builtin_va_start(args,format);
 while(*format){
  if(*format!='%'){write_bytes(format++,1);continue;}
  format++;if(*format=='l')format++;
  if(*format=='s'){const char* s=__builtin_va_arg(args,const char*);unsigned n=0;while(s[n])n++;write_bytes(s,n);}
  else if(*format=='d'||*format=='u'||*format=='x'){
   unsigned value=__builtin_va_arg(args,unsigned),base=*format=='x'?16:10;char buf[16];unsigned n=0;
   if(*format=='d'&&(int)value<0){write_bytes("-",1);value=0-value;}
   do{unsigned digit=value%base;buf[n++]=digit<10?'0'+digit:'a'+digit-10;value/=base;}while(value);
   while(n)write_bytes(&buf[--n],1);
  }else write_bytes(format,1);
  if(*format)format++;
 }
 __builtin_va_end(args);return 0;
}
void* memset(void* dest,int c,unsigned n){unsigned char* p=dest;while(n--)*p++=c;return dest;}
void* memcpy(void* dest,const void* src,unsigned n){unsigned char* d=dest;const unsigned char* s=src;while(n--)*d++=*s++;return dest;}
