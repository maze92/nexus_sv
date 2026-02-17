Aplicar os patches por ordem numérica (diff_001 → diff_024).
Recomendado: `git apply diff_001_...patch` em sequência, ou `git apply diff_*.patch` se o glob respeitar ordem lexicográfica.

Aplicação dos patches (ordem recomendada):
- Aplicar por ordem numérica diff_### (os nomes já estão ordenáveis).
- Ferramenta recomendada: git apply (ou patch -p1).

Nota sobre line endings:
- Alguns ficheiros na source original podem estar em CRLF. Se o patch reclamar de line endings,
  normalize para LF (dos2unix) antes de aplicar, ou use git com autocrlf desativado.

Conteúdo:
- *.patch: diffs seguros (aplicam clean ao baseline testado).
- CHANGELOG_ONE_LINE.txt: changelog no formato pedido.
