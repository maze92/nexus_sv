# -*- coding: utf-8 -*-
import os

INPUT_PROPERTY_DIR = os.path.join("pack_data", "property")
OUTPUT_XML = os.path.join("pack_data", "root", "property.xml")

EXTS = (".prb", ".prd", ".prt", ".pre")

def to_vfs_path(abs_path):
    # caminho “virtual” dentro do VFS: property/...
    rel = os.path.relpath(abs_path, os.path.abspath("pack_data"))
    rel = rel.replace("\\", "/")
    while rel.startswith("./"):
        rel = rel[2:]
    return rel.lower()

def ensure_dir(path):
    d = os.path.dirname(path)
    if d and not os.path.exists(d):
        os.makedirs(d)

def generate_property_xml():
    base = os.path.abspath(INPUT_PROPERTY_DIR)
    if not os.path.isdir(base):
        print "[ERRO] Nao existe:", base
        return

    files = []
    for root, dirs, fns in os.walk(base):
        for fn in fns:
            low = fn.lower()

            # ignora lixo comum
            if low.endswith(".tmp") or low.endswith(".bak"):
                continue

            if low.endswith(EXTS):
                files.append(os.path.join(root, fn))

    vfs = sorted(set([to_vfs_path(p) for p in files]))

    ensure_dir(OUTPUT_XML)
    with open(OUTPUT_XML, "wb") as f:
        f.write('<?xml version="1.0" encoding="utf-8"?>\n')
        f.write("<PropertyList>\n")
        for p in vfs:
            f.write('  <Property filename="%s" />\n' % p)
        f.write("</PropertyList>\n")

    print "[OK] property.xml gerado:"
    print "     ", os.path.abspath(OUTPUT_XML)
    print "[OK] Total properties:", len(vfs)

if __name__ == "__main__":
    generate_property_xml()
