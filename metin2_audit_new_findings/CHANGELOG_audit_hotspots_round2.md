# Changelog — Audit Hotspots (Round 2)
Data: 2026-02-21

Este patch contém apenas achados novos/extra (hardening e correções seguras) identificados numa varredura completa do `server.zip`.

## Alterações
- Corrigidas 4 ocorrências de **format string vulnerável** em `sys_log(0, var)` para `sys_log(0, "%s", var)`:
  - `char_item.cpp`, `char_manager.cpp`, `cmd_gm.cpp`.
- Substituídos múltiplos `sprintf()` por `snprintf()` em buffers fixos (prevenção de overflow/UB):
  - `DragonSoul.cpp`, `char_item.cpp`, `cube.cpp`, `threeway_war.cpp`.
- Hardening de `auth_brazil.cpp`: geração do MD5 hex em buffer fixo agora usa `snprintf()` com **controle de espaço restante** (evita overflow se houver regressão futura).
- Mitigação de overflow em parsing de texto via `sscanf("%s")` sem largura:
  - `motion.cpp`: `%1023s` em tokens para buffers de 1024.
  - `threeway_war.cpp`: `%127s` para nomes de mapa em buffers de 128.
- Hardening adicional (bounds): troca de `strcpy(packet.szSeasonName, season_name)` por `snprintf()` em `battlepass_manager.cpp`.

## Nota
Estas alterações são conservadoras (bounds/formatting) e não alteram a lógica de gameplay; focam-se em eliminar UB, crashes raros e superfícies clássicas de exploração.
