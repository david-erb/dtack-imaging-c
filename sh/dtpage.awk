# ==============================
FILENAME == dtpage && $1 == "product" { product = $2; next }
FILENAME == dtpage && $1 == "title1"  { title1[product]   = $2; next }
FILENAME == dtpage && $1 == "title2"  { title2[product]   = $2; next }
FILENAME == dtpage && $1 == "part"    { part[product]     = $2; next }
FILENAME == dtpage && $1 == "lic_fee" { lic_fee[product]  = $2; next }
FILENAME == dtpage && $1 == "doc_fee" { doc_fee[product]  = $2; next }
FILENAME == dtpage && $1 == "doc"     { next }
FILENAME == dtpage && NF == 0         { next }
FILENAME == dtpage && substr($1, 1, 1) == "#" { next }
FILENAME == dtpage                    { error("bad line " $0) }

