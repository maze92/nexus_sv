# -*- coding: utf-8 -*-
import glob
import os
import shutil
import subprocess
import time

archiver_exe = "Archiver.exe"
input_folder = "xml_data"
output_folder = "xml_output"
pack_output_folder = "pack_output"  # DESTINO FINAL

def ensure_dir(p):
    if not os.path.exists(p):
        os.makedirs(p)

def wait_for_file(path, timeout=180):
    t0 = time.time()
    while time.time() - t0 < timeout:
        if os.path.exists(path):
            return True
        time.sleep(0.2)
    return False

ensure_dir(output_folder)
ensure_dir(pack_output_folder)

files = glob.glob(os.path.join(input_folder, "*_data.xml"))

if not files:
    print "[Erro] Nenhum ficheiro XML encontrado em: %s" % input_folder
else:
    total = len(files)
    print "Iniciando processamento de %d packs...\n" % total

    for index, xml_data in enumerate(sorted(files)):
        base_name = os.path.basename(xml_data).replace("_data.xml", "").lower()

        path_data = os.path.abspath(xml_data)
        path_output = os.path.abspath(os.path.join(output_folder, base_name + "_output.xml"))
        ipk_expected = os.path.abspath(os.path.join(pack_output_folder, base_name + ".ipk"))

        print "[%d/%d] Processando: %s" % (index + 1, total, base_name)

        # limpa output antigo
        if os.path.exists(path_output):
            try: os.remove(path_output)
            except: pass

        # apaga pack antigo no destino final
        if os.path.exists(ipk_expected):
            try: os.remove(ipk_expected)
            except: pass

        # Passo 1: gerar instruções
        subprocess.call([archiver_exe, path_data])

        if not wait_for_file(path_output, timeout=180):
            print "  [Aviso] Nao apareceu o output.xml para %s (%s)" % (base_name, path_output)
            print "--------------------------------------------------"
            continue

        # Passo 2: compactar (isto vai criar o .ipk no ArchivePath do XML)
        print "  -> Compactando para .ipk..."
        subprocess.call([archiver_exe, path_output])

        if os.path.exists(ipk_expected):
            print "  -> OK: %s" % ipk_expected
        else:
            print "  [WARN] Nao encontrei o .ipk em pack_output. Confirma se o teu XML tem ArchivePath='pack_output/%s.ipk'." % base_name

        print "--------------------------------------------------"

# se existir pasta pack/, remove (para não te “baralhar”)
if os.path.isdir("pack"):
    try:
        shutil.rmtree("pack")
        print "\n[INFO] Pasta 'pack/' removida (nao e usada)."
    except:
        print "\n[WARN] Nao consegui remover a pasta 'pack/'."

print "\nProcesso concluido!"