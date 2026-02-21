# Changelog - Auditoria (novos achados)

Data: 2026-02-21

Este changelog descreve o patch `metin2_audit_new_findings.patch` (novos achados) gerado após uma varredura completa dos zips atuais (server.zip + svn-client.zip + client.zip).

- Fix: format string em logs (sys_log/sys_err) — convertidos para `sys_log(0, "%s", var)` / `sys_err("%s", var)` em pontos onde a string era variável.
- Fix: overflow em `CHARACTER::SetItemAward_cmd` — troca de `strcpy` para `snprintf` bounded (buffer é 20 bytes); assinatura passou a `const char*` e inclui `<cstdio>`.
- Fix: overflow em BattlePass — troca de `strcpy(packet.szSeasonName, season_name)` por `snprintf(..., sizeof(...))`.
- Fix: overflow em ItemAwardInformer (DB) — troca de `strcpy` para `snprintf` bounded ao copiar `login` e `command` para `TPacketItemAwardInfromer`.
- Fix: overflow em EventFlag (DB) — troca de `std::strcpy(p.szFlagName, flag)` por `snprintf` bounded.
- Fix: overflow em log_dir (libthecore) — `log_dir[32]` passa a ser preenchido com `snprintf` bounded.
- Hardening: desativa `io.popen` por defeito no Lua (`liolib.c`) via `ENABLE_LUA_POPEN` (default 0). Quando desativado, chamadas retornam erro Lua em vez de executar comandos do sistema.

Notas:
- A mudança `ENABLE_LUA_POPEN` pode afetar scripts que dependam de `io.popen`. Por defeito é um ganho de segurança (impede execução de comandos via quests).
