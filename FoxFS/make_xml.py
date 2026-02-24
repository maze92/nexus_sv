# -*- coding: utf-8 -*-
import os
import shutil
import time

input_dir = "pack_data"
xml_data_dir = "xml_data"
pack_output = "pack_output"
xml_output = "xml_output"

# Pastas que NÃO devem virar .ipk
EXCLUDE_FOLDERS = set([
    "property_clean",
    "__pycache__",
    "xml_data",
    "xml_output",
    "pack_output",
])

def _safe_mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)

def _to_posix(path):
    return path.replace("\\", "/")

def _safe_case_rename(src, dst):
    # Em Windows, rename só de case pode falhar -> usa tmp
    if src.lower() == dst.lower():
        tmp = dst + ".__tmp__%d" % int(time.time())
        os.rename(src, tmp)
        os.rename(tmp, dst)
    else:
        os.rename(src, dst)

def force_lowercase(directory):
    print "-> Converter para minusculas em '%s'..." % directory
    for root_path, dirs, files in os.walk(directory, topdown=False):
        for name in files:
            lower = name.lower()
            if name != lower:
                src = os.path.join(root_path, name)
                dst = os.path.join(root_path, lower)

                if os.path.exists(dst):
                    print "   [WARN] Colisao (ficheiro ja existe): %s -> %s" % (src, dst)
                    continue

                try:
                    _safe_case_rename(src, dst)
                except:
                    print "   [WARN] Falha rename: %s -> %s" % (src, dst)

        for name in dirs:
            lower = name.lower()
            if name != lower:
                src = os.path.join(root_path, name)
                dst = os.path.join(root_path, lower)

                if os.path.exists(dst):
                    print "   [WARN] Colisao (pasta ja existe): %s -> %s" % (src, dst)
                    continue

                try:
                    _safe_case_rename(src, dst)
                except:
                    print "   [WARN] Falha rename: %s -> %s" % (src, dst)

def fix_duplicate_inner_folder(pack_folder):
    """
    Corrige: pack_data/sound/sound/... -> pack_data/sound/...
            pack_data/ui/ui/...       -> pack_data/ui/...
    Detecção case-insensitive.
    """
    base = os.path.basename(pack_folder.rstrip("\\/")).lower()

    inner = None
    try:
        for d in os.listdir(pack_folder):
            p = os.path.join(pack_folder, d)
            if os.path.isdir(p) and d.lower() == base:
                inner = p
                break
    except:
        return False

    if not inner or not os.path.isdir(inner):
        return False

    print "   [FIX] Duplicado detectado: '%s' -> mover conteudo para '%s'" % (inner, pack_folder)

    for item in os.listdir(inner):
        src = os.path.join(inner, item)
        dst = os.path.join(pack_folder, item)

        if os.path.exists(dst):
            if os.path.isdir(src) and os.path.isdir(dst):
                for rp, ds, fs in os.walk(src):
                    rel = os.path.relpath(rp, src)
                    target_root = os.path.join(dst, rel) if rel != "." else dst
                    if not os.path.exists(target_root):
                        os.makedirs(target_root)
                    for fn in fs:
                        sfile = os.path.join(rp, fn)
                        dfile = os.path.join(target_root, fn)
                        if not os.path.exists(dfile):
                            try:
                                shutil.move(sfile, dfile)
                            except:
                                pass
                try:
                    shutil.rmtree(src)
                except:
                    pass
            else:
                print "      [WARN] Conflito, destino existe (ignorado): %s" % dst
        else:
            try:
                shutil.move(src, dst)
            except:
                print "      [WARN] Falha move: %s -> %s" % (src, dst)

    try:
        shutil.rmtree(inner)
    except:
        pass

    return True

def generate_xmls():
    print "-> Gerar XMLs..."
    _safe_mkdir(xml_data_dir)
    _safe_mkdir(xml_output)
    _safe_mkdir(pack_output)

    folders = []
    for f in os.listdir(input_dir):
        p = os.path.join(input_dir, f)
        if not os.path.isdir(p):
            continue
        fl = f.lower()
        if fl in EXCLUDE_FOLDERS:
            continue
        folders.append(f)

    folders.sort(key=lambda x: x.lower())

    total = 0
    for folder in folders:
        folder_lower = folder.lower()
        folder_path = os.path.join(input_dir, folder)

        fix_duplicate_inner_folder(folder_path)

        xml_file_name = "%s_data.xml" % folder_lower
        xml_full_path = os.path.join(xml_data_dir, xml_file_name)

        if folder_lower == "root":
            virtual_path = ""
        else:
            virtual_path = folder_lower + "/"

        input_spec = "%s/%s:%s" % (_to_posix(input_dir), folder_lower, virtual_path)
        archive_path = "%s/%s.ipk" % (_to_posix(pack_output), folder_lower)
        out_xml = "%s/%s_output.xml" % (_to_posix(xml_output), folder_lower)

        with open(xml_full_path, "wb") as fp:
            fp.write("<ScriptFile>\n")
            fp.write('\t<CreateEterPackXml Input="%s" ArchivePath="%s" XmlPath="%s"></CreateEterPackXml>\n'
                     % (input_spec, archive_path, out_xml))
            fp.write("</ScriptFile>\n")

        total += 1

    print "   [OK] %d XMLs gerados." % total

if __name__ == "__main__":
    print "=================================================="
    print "   LOWERCASE + GERADOR XML (FOXFS)"
    print "=================================================="

    # 1) corrige duplicação antes do lowercase
    try:
        for f in os.listdir(input_dir):
            p = os.path.join(input_dir, f)
            if os.path.isdir(p):
                fix_duplicate_inner_folder(p)
    except:
        pass

    # 2) força tudo para minúsculas
    force_lowercase(input_dir)

    # 3) gerar XMLs (packs vão para pack_output)
    generate_xmls()

    print "\nConcluido."
