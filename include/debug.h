#ifndef DEBUG_H
#define DEBUG_H

void inspect_memory(char const* straddr);
void inspect_reg(char const* regname);
int dbg_parse_hex(char const* str);
void puthexint(uint32_t i);

#endif
