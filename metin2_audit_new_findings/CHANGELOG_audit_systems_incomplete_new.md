# Changelog — Auditoria (novos achados)
Data: 2026-02-21

Patch: `metin2_audit_systems_incomplete_new.patch`

- game/src/db.cpp: Escapa a string `country` antes de inserir em SQL (previne injeção/quote-break em UPDATE account durante login).
- game/src/auth_brazil.cpp: Troca `sprintf` por `snprintf` na geração de MD5 hex (hardening, evita escrita fora do buffer em alterações futuras).
- game/src/char_item.cpp: Troca `sprintf` por `snprintf` em mensagens de log de DragonSoul time-charge (hardening contra overflow).
- game/src/cube.cpp: Troca `sprintf` por `snprintf` em buffers temporários usados para serialização de materiais/rewards (hardening).
- game/src/DragonSoul.cpp: Troca `sprintf` por `snprintf` em várias mensagens de log (hardening, evita overflow de buf).
- game/src/threeway_war.cpp: Troca `sprintf` por `snprintf` na construção de filenames de regen (hardening).
- db/src/ClientManagerBoot.cpp: Troca query-build por `snprintf` com limites e guards básicos para evitar overflow de `szQuery` em loops (mailbox/gem shop).
- db/src/ClientManager.cpp: Troca `sprintf` por `snprintf` em queries de cash/mileage (hardening, evita overflow de `szQuery`).
- libthecore/src/log.c: Remove dependência de shell para limpeza de logs em Linux: usa fork+execl('/bin/rm', ...). Também troca `sprintf` por `snprintf` em buffers auxiliares.
